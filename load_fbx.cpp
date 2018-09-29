#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <DirectXMath.h>
#include <fbxsdk.h>
#include "edit_math.h"
#include "load_fbx.h"

using namespace DirectX;

#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pManager->GetIOSettings()))
#endif

// デストラクタ
LoadFBX::~LoadFBX(void)
{
}

bool LoadFBX::Load(std::string fileName, MODEL& model)
{
	fileName_ = fileName;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager_, lScene_);

	if (!LoadScene(lSdkManager_, lScene_, fileName.c_str(), false))
	{
		return false;
	}

	int lSDKMajor, lSDKMinor, lSDKRevision;

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	FbxGeometryConverter geometryConverter(lSdkManager_);
	geometryConverter.Triangulate(lScene_, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(lScene_);

	FbxNode* pRootNode = lScene_->GetRootNode();

	//マテリアル数カウント
	CountMesh(pRootNode, model);

	//メッシュ情報取得
	if (!GetMesh(pRootNode, model))
	{
		return false;
	}

	// Destroy all objects created by the FBX SDK.
	lScene_->Destroy();
	lSdkManager_->Destroy();

	model.bone = boneList_;

	return true;
}

bool LoadFBX::LoadAnimation(std::string fileName, MODEL& model)
{
	fileName_ = fileName;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager_, lScene_);

	if (!LoadScene(lSdkManager_, lScene_, fileName.c_str(), true))
	{
		return false;
	}

	int lSDKMajor, lSDKMinor, lSDKRevision;

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	FbxGeometryConverter geometryConverter(lSdkManager_);
	geometryConverter.Triangulate(lScene_, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(lScene_);

	FbxNode* pRootNode = lScene_->GetRootNode();

	MODEL tempModel;
	//マテリアル数カウント
	CountMesh(pRootNode, tempModel);
	//メッシュ情報取得
	if (!(GetMesh(pRootNode, tempModel)))
	{
		return false;
	}

	const BONE* isAnim = nullptr;
	unsigned int animCnt = 0;
	int size = (int)animationStack_.size();
	for (int i = 0; i < size; ++i)
	{
		GetAnimation(animationStack_[i]);

		if (animBoneList_.size() > 0)
		{
			for (const auto& anim : animBoneList_)
			{
				for (auto& bone : model.bone)
				{
					if (anim.name == bone.name)
					{
						unsigned int s = bone.animMtx.size();
						bone.animMtx.emplace_back(anim.animMtx[0]);
						isAnim = &bone;
						animCnt = bone.animMtx.size() - 1;
						bone.animMtx[s].erase(bone.animMtx[s].begin());
					}
				}
			}
		}
		ReleaseTempAnimObj();
	}
	unsigned int animFrame = (int)isAnim->animMtx[animCnt].size();
	for (auto& bone : model.bone)
	{
		if (bone.animMtx.size() != animCnt + 1)
		{
			std::vector<XMFLOAT4X4> temp;
			for (unsigned int i = 0; i < animFrame; ++i)
			{
				XMFLOAT4X4 t;
				EditMath::Identity(t);
				temp.emplace_back(t);
			}
			bone.animMtx.emplace_back(temp);
		}
	}


	// Destroy all objects created by the FBX SDK.
	lScene_->Destroy();
	lSdkManager_->Destroy();

	return true;
}

// メッシュを数える
void LoadFBX::CountMesh(FbxNode* pNode, MODEL& model)
{
	int childNodeNum = pNode->GetChildCount();
	for (int i = 0; i < childNodeNum; ++i)
	{
		// 子ノードを取得
		FbxNode*			pChild	= pNode->GetChild(i);
		FbxNodeAttribute*	pAttrib = pChild->GetNodeAttribute();

		if (pAttrib == nullptr) { continue; }

		switch (pAttrib->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			materialCount_[0]++;
			break;
		default:
			break;
		}
		CountMesh(pChild, model);
	}
}

// メッシュを取得する
bool LoadFBX::GetMesh(FbxNode* pNode, MODEL& model)
{
	tempNode_.emplace_back(pNode);
	int			childNodeNum = pNode->GetChildCount();
	FbxMesh*	mesh = nullptr;

	for (int i = 0; i < childNodeNum; ++i)
	{
		// 子ノードを取得
		FbxNode*			pChild	= pNode->GetChild(i); 
		FbxNodeAttribute*	pAttrib = pChild->GetNodeAttribute();

		if (pAttrib == nullptr) { continue; }

		switch (pAttrib->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
		{
			mesh = pChild->GetMesh();
			std::string name = pChild->GetName();
			if (name == "BLW_DEF")
			{
				int a = 0;
				a = 1;
			}

			GetPosition(mesh);
			GetVertexNormal(mesh);
			GetVertexTangent(mesh);
			GetVertexUV(mesh);
			GetTextureNames(mesh);
			GetVertexColor(mesh);
			if(!(this->CreateModelData(model)))
			{
				return false;
			}

			materialCount_[1]++;

			//マテリアル数だけ取得したら再帰をぬける
			if (materialCount_[1] >= materialCount_[0])
			{
				tempNode_.emplace_back(pChild);
				materialCount_[0] = materialCount_[1] = 0;
				return true;
			}
			break;
		}
		default:

			break;
		}
		GetMesh(pChild, model);
	}
	return true;
}

// 頂点座標を取得する
void LoadFBX::GetPosition(FbxMesh* mesh)
{
	// メッシュマテリアルリストの取得
	this->GetMeshMaterialList(mesh);

	// 総頂点数
	pointCount_ = mesh->GetControlPointsCount();
	// 総ポリゴン数の取得
	polygonCount_ = mesh->GetPolygonCount();
	// 総インデックス数の取得
	indexCount_   = mesh->GetPolygonVertexCount();

	// 頂点の取得
	FbxVector4* vertex = mesh->GetControlPoints();

	// 頂点の取得
	for (int i = 0; i < pointCount_; ++i)
	{
		//頂点情報をインデックス順で取得
		VERTEXPOINTBONE temp;
		//頂点座標反転
		temp.position = { -(float)vertex[i][0], (float)vertex[i][1], (float)vertex[i][2] };

		for (int j = 0; j < 4; ++j)
		{
			temp.boneData[j].boneIndex = 0;
			temp.boneData[j].weight = 0;
		}
		tempPoint_.emplace_back(temp);
	}

	GetBone(mesh);
}

// メッシュマテリアルリストの取得
void LoadFBX::GetMeshMaterialList(FbxMesh* mesh)
{
	// 中身の解放
	meshMaterialList_.clear();
	// メッシュに付いているマテリアルの数の取得
	int layerCount = mesh->GetLayerCount();
	for (int i = 0; i < layerCount; ++i)
	{
		// マテリアルの取得
		FbxLayerElementMaterial* mat = mesh->GetLayer(i)->GetMaterials();
		if (mat)
		{
			if (mat->GetReferenceMode() == FbxLayerElement::eIndex ||
				mat->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
			{
				// マテリアルのポリゴン数を取得
				int polygonNum = mat->GetIndexArray().GetCount();
				for (int j = 0; j < polygonNum; ++j)
				{
					// ポリゴン数分なのでインデックス分に変える(三角頂点化しているため3倍でいいはず)
					meshMaterialList_.emplace_back(mat->GetIndexArray().GetAt(j));
					meshMaterialList_.emplace_back(mat->GetIndexArray().GetAt(j));
					meshMaterialList_.emplace_back(mat->GetIndexArray().GetAt(j));
				}
			}
		}
	}
}

// ボーンを取得する
void LoadFBX::GetBone(FbxMesh* mesh)
{
	// スキンの数を取得
	int skinMax = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinMax == 0)
	{
		BONE ofBone;
		ofBone.name = mesh->GetNode()->GetName();
		EditMath::Identity(ofBone.initMtx);
		boneList_.emplace_back(ofBone);

		unsigned int cnt = 0;
		for (const auto& bone : boneList_)
		{
			if (bone.name == ofBone.name)
			{
				break;
			}
			cnt++;
		}
		if (cnt == boneList_.size())
		{
			boneList_.emplace_back(ofBone);
		}

		for (int k = 0; k < pointCount_; ++k)
		{
			VERTEXBONE t;
			t.boneIndex = (float)cnt;
			t.weight = 1.0f;

			if (tempPoint_[k].boneData[0].weight == 0)
			{
				tempPoint_[k].boneData[0].boneIndex = t.boneIndex;
				tempPoint_[k].boneData[0].weight = t.weight;
			}
		}
	}

	for (int i = 0; i < skinMax; ++i)
	{
		// i番目のスキンを取得
		FbxSkin* skin = (FbxSkin*)(mesh->GetDeformer(i, FbxDeformer::eSkin));

		// クラスターの数を取得
		int clusterNum = skin->GetClusterCount();

		for (int j = 0; j < clusterNum; ++j)
		{
			// j番目のクラスタを取得 クラスタ ＝ ボーン
			FbxCluster* cluster   = skin->GetCluster(j);
			int			pointNum  = cluster->GetControlPointIndicesCount();
			int*		pointAry  = cluster->GetControlPointIndices();
			double*		weightAry = cluster->GetControlPointWeights();
			// ボーンオフセット行列の取得

			BONE ofBone;
			ofBone.name = cluster->GetLink()->GetName();
			FbxAMatrix initMtx;
			cluster->GetTransformLinkMatrix(initMtx);
			this->CreateMtx(ofBone.initMtx, initMtx);
			unsigned int cnt = 0;
			for (const auto& bone : boneList_)
			{
				if (bone.name == ofBone.name)
				{
					break;
				}
				cnt++;
			}
			if (cnt == boneList_.size()) 
			{
				boneList_.emplace_back(ofBone); 
			}

			// ボーンの影響度の取得
			for (int k = 0; k < pointCount_; ++k)
			{
				for (int l = 0; l < pointNum; ++l)
				{
					// 影響頂点が存在した場合
					if (pointAry[l] == k)
					{
						// 影響度の取得
						VERTEXBONE t;
						t.boneIndex = (float)cnt;
						t.weight    = (float)weightAry[l];

						for (int m = 0; m < 4; ++m)
						{
							if (tempPoint_[k].boneData[m].weight == 0)
							{
								tempPoint_[k].boneData[m].boneIndex = t.boneIndex;
								tempPoint_[k].boneData[m].weight    = t.weight;
								break;
							}
						}
					}
				}
			}
		}
	}

	// インデックスの取得
	int* index = mesh->GetPolygonVertices();

	// インデックス分回す
	for (unsigned int i = 0; i < indexCount_; ++i)
	{
		tempVertex_.emplace_back(tempPoint_[index[i]]);
	}
}

// 法線の取得
void LoadFBX::GetVertexNormal(FbxMesh* mesh)
{
	// 法線セット数を取得
	int normalLayerCount = mesh->GetElementNormalCount();

	// レイヤー数だけ回る
	for (int i = 0; i < normalLayerCount; ++i)
	{
		// 法線セットを取得
		FbxGeometryElementNormal* normal = mesh->GetElementNormal(i);

		// マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = normal->GetMappingMode();
		// リファレンスモードの取得
		FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

		// マッピングモードの判別
		switch (mapping)
		{
		case FbxGeometryElement::eByControlPoint:
			// リファレンスモードの判別
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
			{
				int normalCount = normal->GetDirectArray().GetCount();

				std::vector<XMFLOAT3> directNormal;
				for (int j = 0; j < normalCount; ++j)
				{
					XMFLOAT3 tempNormal;
					//法線の取得
					//法線反転
					tempNormal.x = -(float)normal->GetDirectArray().GetAt(j)[0];
					tempNormal.y = (float)normal->GetDirectArray().GetAt(j)[1];
					tempNormal.z = (float)normal->GetDirectArray().GetAt(j)[2];

					directNormal.emplace_back(tempNormal);
				}

				// 法線をインデックス順で格納
				int* index = mesh->GetPolygonVertices();
				for (unsigned int j = 0; j < indexCount_; j++)
				{
					XMFLOAT3 tempNormal = { directNormal[index[j]].x, directNormal[index[j]].y, directNormal[index[j]].z };
					tempNormal_.emplace_back(tempNormal);
				}
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
				break;

			default:
				break;
			}

			break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			// 法線数を取得
			unsigned int normalCount = normal->GetDirectArray().GetCount();
			unsigned int indexCount  = normal->GetIndexArray().GetCount();

			std::vector<XMFLOAT3> directNormal;
			for (unsigned int j = 0; j < normalCount; ++j)
			{
				//法線の取得
				//法線反転
				XMFLOAT3 tempNormal;
				tempNormal.x = -(float)normal->GetDirectArray().GetAt(j)[0];
				tempNormal.y = (float)normal->GetDirectArray().GetAt(j)[1];
				tempNormal.z = (float)normal->GetDirectArray().GetAt(j)[2];
				
				directNormal.emplace_back(tempNormal);
			}

			// 法線の数がインデックスの数と同じならそのまま
			if (normalCount == indexCount_)
			{
				tempNormal_ = directNormal;
			}
			// 法線インデックスの数が頂点のインデックスと同じ時
			else if (indexCount == indexCount_)
			{
				// インデックスを取得
				int* index = mesh->GetPolygonVertices();
				for (int j = 0; j < normal->GetIndexArray().GetCount(); ++j)
				{
					XMFLOAT3 tempNormal = { directNormal[index[j]].x, directNormal[index[j]].y, directNormal[index[j]].z };
					tempNormal_.emplace_back(tempNormal);
				}
			}
		}
		break;
		default:
			break;
		}
	}
}

// Tangentの取得
void LoadFBX::GetVertexTangent(FbxMesh* mesh)
{
	// Tangentセット数を取得
	int tangentLayerCount = mesh->GetElementTangentCount();
	if (tangentLayerCount <= 0)
	{
		//Tangentが無かった場合生成
		mesh->GenerateTangentsDataForAllUVSets();
	}
	tangentLayerCount = mesh->GetElementTangentCount();

	// レイヤー数だけ回る
	for (int i = 0; i < tangentLayerCount; ++i)
	{
		// Tangent生成
		FbxGeometryElementTangent* tangent = mesh->GetElementTangent(i);

		// マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = tangent->GetMappingMode();

		// リファレンスモードの取得
		FbxGeometryElement::EReferenceMode reference = tangent->GetReferenceMode();

		// マッピングモードの判別
		switch (mapping)
		{
		case FbxGeometryElement::eByControlPoint:
			// リファレンスモードの判別
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
			{
				int tangentCount = tangent->GetDirectArray().GetCount();

				std::vector<XMFLOAT3> directTangent;
				for (int j = 0; j < tangentCount; ++j)
				{
					XMFLOAT3 tempTangent;
					//Tangentの取得
					//Tangent反転
					tempTangent.x = -(float)tangent->GetDirectArray().GetAt(j)[0];
					tempTangent.y = (float)tangent->GetDirectArray().GetAt(j)[1];
					tempTangent.z = (float)tangent->GetDirectArray().GetAt(j)[2];
					directTangent.emplace_back(tempTangent);
				}

				// Tangentをインデックス順で格納
				int* index = mesh->GetPolygonVertices();
				for (unsigned int j = 0; j < indexCount_; j++)
				{
					XMFLOAT3 tempTangent = { directTangent[index[j]].x, directTangent[index[j]].y, directTangent[index[j]].z };
					tempTangent_.emplace_back(tempTangent);
				}
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
				break;

			default:
				break;
			}

			break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			// Tangent数を取得
			unsigned int tangentCount = tangent->GetDirectArray().GetCount();
			unsigned int indexCount = tangent->GetIndexArray().GetCount();

			std::vector<XMFLOAT3> directTangent;
			for (unsigned int j = 0; j < tangentCount; ++j)
			{
				//Tangentの取得
				//Tangent反転
				XMFLOAT3 tempTangent;
				tempTangent.x = -(float)tangent->GetDirectArray().GetAt(j)[0];
				tempTangent.y = (float)tangent->GetDirectArray().GetAt(j)[1];
				tempTangent.z = (float)tangent->GetDirectArray().GetAt(j)[2];
				directTangent.emplace_back(tempTangent);
			}

			// Tangentの数がインデックスの数と同じならそのまま
			if (tangentCount == indexCount_)
			{
				tempTangent_ = directTangent;
			}
			// Tangentインデックスの数が頂点のインデックスと同じ時
			else if (indexCount == indexCount_)
			{
				// インデックスを取得
				int* index = mesh->GetPolygonVertices();
				for (int j = 0; j < tangent->GetIndexArray().GetCount(); ++j)
				{
					XMFLOAT3 tempTangent = { directTangent[index[j]].x, directTangent[index[j]].y, directTangent[index[j]].z };
					tempTangent_.emplace_back(tempTangent);
				}
			}
		}
		break;
		default:
			break;
		}
	}

}

// テクスチャ座標の取得
void LoadFBX::GetVertexUV(FbxMesh* mesh)
{
	// UVセット数を取得
	int UVLayerCount = mesh->GetElementUVCount();

	for (int i = 0; i < UVLayerCount; ++i) {
		// UVバッファを取得
		FbxGeometryElementUV* UV = mesh->GetElementUV(i);
		// マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();
		// リファレンスモードの取得
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();

		// マッピングモードの判別
		switch (mapping) {
		case FbxGeometryElement::eByControlPoint:
			break;

		case FbxGeometryElement::eByPolygonVertex:
			//--- リファレンスモードの判別 ---//
			switch (reference) {
			case FbxGeometryElement::eDirect:
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int indexCount = UV->GetIndexArray().GetCount();

				// UVを保持
				FBXUV tempUV;
				for (int j = 0; j < indexCount; ++j)
				{
					int index = UV->GetIndexArray().GetAt(j);
					
					//テクスチャ座標反転
					XMFLOAT2 temp = { (float)UV->GetDirectArray().GetAt(index)[0] , 1.0f - (float)UV->GetDirectArray().GetAt(index)[1] };
					tempUV.texcoord.emplace_back(temp);
				}
				// UVSet名を取得
				tempUV.uvSetName = std::string(UV->GetName());
				tempTexcoord_.emplace_back(tempUV);
			}
			break;
			default:
				break;
			}
			break;
		case FbxGeometryElement::eByEdge:
			break;
		case FbxGeometryElement::eByPolygon:
			break;
		default:
			break;
		}
	}
}

// テクスチャを取得
void LoadFBX::GetTextureNames(FbxMesh* mesh) 
{
	int textureNum = 0;

	// メッシュからノードを取得
	FbxNode* node = mesh->GetNode();

	// マテリアルの数を取得
	int materialCnt = node->GetMaterialCount();
	// マテリアルの数だけ繰り返す
	for (int i = 0; i < materialCnt; i++)
	{
		// マテリアルを取得
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		FbxProperty			prop	 = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

		// FbxLayeredTexture の数を取得
		int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
		// アタッチされたテクスチャが FbxLayeredTexture の場合
		if (0 < layeredTextureCount)
		{
			// アタッチされたテクスチャの数だけ繰り返す
			for (int j = 0; j < layeredTextureCount; ++j)
			{
				// レイヤー数を取得
				int textureCount = 1;

				// レイヤー数だけ繰り返す
				for (int k = 0; k < textureCount; ++k)
				{
					// テクスチャを取得
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);
					if (texture) {
						UVSet temp;
						// テクスチャ名を取得
						temp.texName = std::string(texture->GetRelativeFileName());
						// UVSet名を取得
						temp.uvSetName = std::string(texture->UVSet.GetName());

						tempUVSet_.emplace_back(temp);

						//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
						//???
						//size_t size = strlen(textureName);
						//aMeshBuffer[model].uvSet[materialCount].texture = new char[size + 1];
						//memcpy(aMeshBuffer[model].uvSet[materialCount].uvSetName, textureName, size);
						//aMeshBuffer[model].uvSet[materialCount].texture[size] = '\0';
					}
				}
			}
		}
		else 
		{
			// テクスチャ数を取得
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();
			if(fileTextureCount > 0)
			{
			// テクスチャの数だけ繰り返す
				for (int j = 0; j < fileTextureCount; j++)
				{
					// テクスチャを取得
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
					if (texture)
					{
						UVSet temp;
						// テクスチャ名を取得
						temp.texName = fileName_;
						// テクスチャのディレクトリはモデルと同じ
						for (unsigned int k = temp.texName.size() - 1; temp.texName[k] != '/' && k > 0; k--)
						{
							temp.texName.pop_back();
						}

						std::string tempName = texture->GetFileName();
						std::string texName;
						for (unsigned int k = tempName.size() - 1; tempName[k] != 92 && tempName[k] != '/' && k > 0; k--)
						{
							texName.insert(texName.begin(), tempName[k]);
						}
						temp.texName += texName;

						// UVSet名を取得
						temp.uvSetName = std::string(texture->UVSet.Get().Buffer());

						tempUVSet_.emplace_back(temp);
						textureNum++;
					}
				}
			}
			else
			{
				FbxSurfaceMaterial*			mat		= node->GetSrcObject<FbxSurfaceMaterial>(i);
				const FbxImplementation*	implementation  = GetImplementation(mat, FBXSDK_IMPLEMENTATION_HLSL);
				FbxString implemenationType = "HLSL";
				if (!implementation)
				{
					implementation = GetImplementation(mat, FBXSDK_IMPLEMENTATION_CGFX);
					implemenationType = "CGFX";
				}

				if (implementation)
				{
					const FbxBindingTable*	rootTable     = implementation->GetRootTable();
					FbxString				fileName      = rootTable->DescAbsoluteURL.Get();
					FbxString				techniqueName = rootTable->DescTAG.Get();
					const FbxBindingTable*	table		  = implementation->GetRootTable();
					size_t					entryCount    = table->GetEntryCount();
					for (int j = 0; j < (int)entryCount; ++j)
					{
						const FbxBindingTableEntry& entry		 = table->GetEntry(j);
						const char*					entrySrcType = entry.GetEntryType(true);
						FbxProperty					fbxProp;
						FbxString					source		 = entry.GetSource();

						if (strcmp(FbxPropertyEntryView::sEntryType, entrySrcType) == 0)
						{
							fbxProp = mat->FindPropertyHierarchical(entry.GetSource());
							if (!fbxProp.IsValid())
							{
								fbxProp = mat->RootProperty.FindHierarchical(entry.GetSource());
							}
						}
						else
							if (strcmp(FbxConstantEntryView::sEntryType, entrySrcType) == 0)
							{
								fbxProp = implementation->GetConstants().FindHierarchical(entry.GetSource());
							}

						if (fbxProp.IsValid())
						{
							if (fbxProp.GetSrcObjectCount<FbxTexture>() > 0)
							{
								for (int k = 0; k < fbxProp.GetSrcObjectCount<FbxFileTexture>(); ++k)
								{
									FbxFileTexture* pTex = fbxProp.GetSrcObject<FbxFileTexture>(k);
									if (strstr(source.Buffer(), "DiffuseTexture") != nullptr)
									{
										UVSet temp;
										// テクスチャ名を取得
										temp.texName = fileName_;
										// テクスチャのディレクトリはモデルと同じ
										for (unsigned int l = temp.texName.size() - 1; temp.texName[l] != '/' && l > 0; l--)
										{
											temp.texName.pop_back();
										}

										std::string tempName = pTex->GetFileName();
										std::string texName;
										for (unsigned int l = tempName.size() - 1; tempName[l] != 92 && tempName[l] != '/' && l > 0; l--)
										{
											texName.insert(texName.begin(), tempName[k]);
										}

										temp.texName += texName;
										temp.uvSetName = "";
										tempUVSet_.emplace_back(temp);
										textureNum++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	// 設定がなかった時
	if (textureNum == 0)
	{
		UVSet temp;
		temp.texName	= "";
		temp.uvSetName	= "";
		tempUVSet_.emplace_back(temp);
	}
}

// 色の取得
void LoadFBX::GetVertexColor(FbxMesh* mesh)
{
	// 頂点カラーセット数を取得
	int vColorLayerCount = mesh->GetElementVertexColorCount();

	// 設定がなかった時
	if (vColorLayerCount == 0) 
	{
		for (unsigned int i = 0; i < indexCount_; ++i)
		{
			XMFLOAT4 temp = XMFLOAT4(1, 1, 1, 1);
			tempColor_.emplace_back(temp);
		}
	}

	// レイヤー数だけ回る
	for (int i = 0; i < vColorLayerCount; ++i) 
	{
		// 頂点カラーセットを取得
		FbxGeometryElementVertexColor* color = mesh->GetElementVertexColor(i);
		// マッピングモードの取得
		FbxGeometryElement::EMappingMode mappingMode = color->GetMappingMode();
		// リファレンスモードの取得
		FbxGeometryElement::EReferenceMode referenceMode = color->GetReferenceMode();

		// マッピングモードの判別
		switch (mappingMode) {
		case FbxGeometryElement::eByControlPoint:
		{
			FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(i);
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				for (unsigned int j = 0; j < indexCount_; ++j)
				{
					FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)material;
					XMFLOAT4 diffuse = XMFLOAT4(1, 1, 1, 1);
					diffuse.x = (float)lambert->Diffuse.Get()[0];
					diffuse.y = (float)lambert->Diffuse.Get()[1];
					diffuse.z = (float)lambert->Diffuse.Get()[2];

					tempColor_.emplace_back(diffuse);
				}
			}
			break;
		}
		case FbxGeometryElement::eByPolygon:
			break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			// リファレンスモードの判別
			switch (referenceMode) {
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<int>* index = &color->GetIndexArray();
				int indexCount = index->GetCount();
				FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(i);

				//nullチェック
				if (material != nullptr)
				{
					if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
					{
						for (int j = 0; j < indexCount; ++j)
						{
							FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)material;
							XMFLOAT4 diffuse = XMFLOAT4(1, 1, 1, 1);
							diffuse.x = (float)lambert->Diffuse.Get()[0];
							diffuse.y = (float)lambert->Diffuse.Get()[1];
							diffuse.z = (float)lambert->Diffuse.Get()[2];

							tempColor_.emplace_back(diffuse);
						}
					}
				}
			}
			break;
			case FbxGeometryElement::eDirect:
				break;
			case FbxGeometryElement::eIndex:
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	if (tempColor_.size() != indexCount_)
	{
		for (unsigned int j = tempColor_.size(); j < indexCount_; ++j)
		{
			tempColor_.emplace_back(XMFLOAT4(1, 1, 1, 1));
		}
	}
}

bool LoadFBX::CreateModelData(MODEL& model)
{
	HRESULT hr;
	if (meshMaterialList_.size() / 3 == 1)
	{
		std::vector<int> index;
		for (unsigned int i = 0; i < indexCount_; ++i)
		{
			index.emplace_back(i);
		}
		hr = this->CreateVeretx(model, (int)index.size(), &index[0], tempUVSet_[0]);
		if (FAILED(hr))
		{
			return false;
		}
	}
	else
	{
		polygonCount_ = 0;
		// マテリアルの最大値を出す
		int materialMax = 0;
		for (int i = 0; i < (int)meshMaterialList_.size(); ++i)
		{
			if (materialMax < meshMaterialList_[i]) { materialMax = meshMaterialList_[i]; }
		}
		materialMax++;

		// マテリアルの数だけ繰り返す
		for (int i = 0; i < materialMax; ++i)
		{
			// マテリアルを構成するインデックスの数
			std::vector<int> index;
			for (int j = 0; j < (int)meshMaterialList_.size(); ++j)
			{
				if (meshMaterialList_[j] == i) { index.emplace_back(j); }
			}

			polygonCount_ = index.size() / 3;
			hr = this->CreateVeretx(model, (int)index.size(), &index[0], tempUVSet_[materialCount_[1]]);
			if (FAILED(hr))
			{
				return false;
			}

			materialCount_[1]++;
		}
	}

	ReleaseTempObj();

	return true;
}

bool LoadFBX::CreateVeretx(MODEL& model, int indexNum, const int* index, UVSet uv)
{
	UINT size = tempVertex_.size();

	if (tempVertex_.size() == size &&
		tempNormal_.size() == size &&
		tempTangent_.size() == size &&
		tempTexcoord_[0].texcoord.size() == size &&
		tempColor_.size() == size)
	{
		MESH tempMesh;		
		EditMath::Identity(tempMesh.frameTransMtx);
		tempMesh.indexBuffer = 0;
		tempMesh.vertexBuffer = 0;
		auto& m = tempMesh.material;
		m.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.3f);
		m.diffuse = XMFLOAT4(1, 1, 1, 1);
		m.emission = XMFLOAT4(0, 0, 0, 0);
		m.specular = XMFLOAT4(0, 0, 0, 0);
		m.power = 0;
		m.texture = 0;
		m.textureName = "";

		for (int i = 0; i < indexNum; ++i)
		{
			VERTEX temp;
			temp.position = tempVertex_[index[i]].position;
			temp.normal   = tempNormal_[index[i]];
			temp.tangent = tempTangent_[index[i]];
			temp.texcoord = tempTexcoord_[0].texcoord[index[i]];
			temp.color    = tempColor_[index[i]];

			temp.boneIndex.x = tempVertex_[index[i]].boneData[0].boneIndex;
			temp.boneIndex.y = tempVertex_[index[i]].boneData[1].boneIndex;
			temp.boneIndex.z = tempVertex_[index[i]].boneData[2].boneIndex;
			temp.boneIndex.w = tempVertex_[index[i]].boneData[3].boneIndex;

			temp.weight.x = tempVertex_[index[i]].boneData[0].weight;
			temp.weight.y = tempVertex_[index[i]].boneData[1].weight;
			temp.weight.z = tempVertex_[index[i]].boneData[2].weight;
			temp.weight.w = tempVertex_[index[i]].boneData[3].weight;

			tempMesh.vertex.emplace_back(temp);

			//インデックス番号反転
			tempMesh.index.emplace_back((indexNum - 1) - i);
		}
		tempMesh.material.textureName = uv.texName;

		auto& name = tempMesh.material.textureName;
		std::string extension;
		for (int i = uv.texName.size() - 1; i > 0 && uv.texName[i] != '.'; --i)
		{
			extension.insert(extension.begin(), uv.texName[i]);
		}
		if(extension == "tga")
		{ 
			for (int i = name.size() - 1; i > 0 && name[i] != '.'; --i)
			{
				name.pop_back();
			}
			name += "png";
		}

		model.mesh.emplace_back(tempMesh);
	}
	else
	{
		MessageBox(NULL, "頂点情報の数が合いません", "エラー", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	return true;
}

// FbxMatrixから変換
void LoadFBX::CreateMtx(XMFLOAT4X4& mtx, const FbxMatrix fbxMtx)
{
	for (int mtxXCnt = 0; mtxXCnt < 4; mtxXCnt++)
	{
		for (int mtxYCnt = 0; mtxYCnt < 4; mtxYCnt++)
		{			
			mtx.m[mtxYCnt][mtxXCnt] = (float)fbxMtx.Get(mtxYCnt, mtxXCnt);
		}
	}
}

// 一時情報の解放
void LoadFBX::ReleaseTempObj(void)
{
	std::vector<VERTEXPOINTBONE>().swap(tempPoint_);
	std::vector<VERTEXPOINTBONE>().swap(tempVertex_);
	std::vector<XMFLOAT3>().swap(tempNormal_);
	std::vector<XMFLOAT3>().swap(tempTangent_);
	std::vector<XMFLOAT4>().swap(tempColor_);
	std::vector<FBXUV>().swap(tempTexcoord_);
	std::vector<UVSet>().swap(tempUVSet_);
	std::vector<FBXANIMATION>().swap(tempAnim_);
}

// Common.cxx参照
void LoadFBX::InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

bool LoadFBX::LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename, bool animation)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		if (animation)
		{
			lAnimStackCount = lImporter->GetAnimStackCount();

			FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
			FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
			FBXSDK_printf("\n");

			for (i = 0; i < lAnimStackCount; i++)
			{
				FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

				// タイムモード取得
				FbxGlobalSettings& Set = lScene_->GetGlobalSettings();
				FbxTime::EMode timeMode = Set.GetTimeMode();

				FBXANIMATION temp;
				temp.name = lTakeInfo->mName;
				// アニメーションが始まる時間と終わる時間
				FbxTime start = lTakeInfo->mLocalTimeSpan.GetStart();
				FbxTime stop  = lTakeInfo->mLocalTimeSpan.GetStop();
				//fbxの1フレームあたりの時間数
				temp.framePeriod.SetTime(0, 0, 0, 1, 0, timeMode);
				// 1フレームあたりで割れば、フレーム数になる
				temp.startFrame = (int)(start.Get() / temp.framePeriod.Get());
				temp.endFrame = (int)(stop.Get() / temp.framePeriod.Get());
				temp.frameNum = temp.endFrame - temp.startFrame + 1;

				animationStack_.emplace_back(temp);


				FBXSDK_printf("    Animation Stack %d\n", i);
				FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
				FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

				// Change the value of the import name if the animation stack should be imported 
				// under a different name.
				FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

				// Set the value of the import state to false if the animation stack should be not
				// be imported. 
				FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
				FBXSDK_printf("\n");
			}
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);

	if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

void LoadFBX::GetAnimation(FBXANIMATION& animInfo)
{
	FbxAnimStack* pCurrentAnimationStack = lScene_->FindMember<FbxAnimStack>(animInfo.name.c_str());
	if (pCurrentAnimationStack != nullptr)
	{
		lScene_->SetCurrentAnimationStack(pCurrentAnimationStack);
	}
	
	int size = (int)tempNode_.size();
	for (int i = 0; i < size; ++i)
	{	
		const auto& mesh = tempNode_[i]->GetMesh();

		if (mesh && mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
		{
			// スキンの数を取得
			int skinMax = mesh->GetDeformerCount(FbxDeformer::eSkin);
			for (int j = 0; j < skinMax; ++j)
			{
				// i番目のスキンを取得
				FbxSkin* skin = (FbxSkin*)(mesh->GetDeformer(j, FbxDeformer::eSkin));

				// クラスターの数を取得
				int clusterNum = skin->GetClusterCount();

				for (int k = 0; k < clusterNum; ++k)
				{
					BONE t;
					// j番目のクラスタを取得 クラスタ ＝ ボーン
					FbxCluster* cluster = skin->GetCluster(k);

					t.name = cluster->GetLink()->GetName();
					std::vector<XMFLOAT4X4> tempAnim;
					for (int frameCnt = 0; frameCnt < animInfo.frameNum; frameCnt++)
					{
						FbxTime    time = (animInfo.startFrame + frameCnt) * animInfo.framePeriod.Get();
						FbxAMatrix mtx = cluster->GetLink()->EvaluateGlobalTransform(time);

						XMFLOAT4X4 m;
						this->CreateMtx(m, mtx);
						tempAnim.emplace_back(m);
					}
					t.animMtx.emplace_back(tempAnim);
					animBoneList_.emplace_back(t);
				}
			}
		}
		else
		{
			BONE t;
			t.name = tempNode_[i]->GetName();
			std::vector<XMFLOAT4X4> tempAnim;
			for (int j = 0; j < animInfo.frameNum; ++j)
			{
				FbxTime    time = (animInfo.startFrame + j) * animInfo.framePeriod.Get();
				FbxAMatrix mtx = tempNode_[i]->EvaluateGlobalTransform(time);
				XMFLOAT4X4 m;
				this->CreateMtx(m, mtx);
				tempAnim.emplace_back(m);
			}
			t.animMtx.emplace_back(tempAnim);
			animBoneList_.emplace_back(t);
		}
	}
}

void LoadFBX::ReleaseTempAnimObj(void)
{
	animationStack_.clear();
	tempNode_.clear();
	animBoneList_.clear();
}
