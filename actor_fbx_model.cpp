//**-------------------------------------------------------**
//**
//**                actor_fbx_model.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <fbxsdk.h>
#include "actor_3d.h"
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "main.h"
#include "renderer.h"
#include "sampler_state.h"
#include "shader.h"
#include "shader_fbx.h"
#include "shader_manager.h"
#include "texture.h"
#include "actor_fbx_model.h"

using namespace DirectX;

ActorFbxModel::ActorFbxModel(ActorManager* pActorManager) : Actor3d(pActorManager)
{
	name_ = ActorManager::NAME_FBX_MODEL;
	tag_ = ActorManager::TAG_FBX_MODEL;
	layer_ = ActorManager::LAYER_FBX_MODEL;
}

ActorFbxModel::~ActorFbxModel()
{

}

bool ActorFbxModel::Init()
{
	nameUnique_ = "ActorFbxModel" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	//FBX読み込み
	pFbxManager_ = FbxManager::Create();
	pFbxScene_ = FbxScene::Create(pFbxManager_, "fbxscene");
	FbxString FileName("humanoid.fbx");
	FbxImporter *fbxImporter = FbxImporter::Create(pFbxManager_, "imp");
	fbxImporter->Initialize(FileName.Buffer(), -1, pFbxManager_->GetIOSettings());
	fbxImporter->Import(pFbxScene_);
	fbxImporter->Destroy();

	//子ノードを検索してメッシュを見つけたら格納
	for (int i = 0; i < pFbxScene_->GetRootNode()->GetChildCount(); i++) {
		if (pFbxScene_->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			pMesh_ = pFbxScene_->GetRootNode()->GetChild(i)->GetMesh();
			break;
		}
	}

	//頂点情報取得
	pVertices_ = new VERTEX[pMesh_->GetControlPointsCount()];
	for (int i = 0; i < pMesh_->GetControlPointsCount(); i++) {
		pVertices_[i].pos.x = (FLOAT)pMesh_->GetControlPointAt(i)[0];
		pVertices_[i].pos.y = (FLOAT)pMesh_->GetControlPointAt(i)[1];
		pVertices_[i].pos.z = (FLOAT)pMesh_->GetControlPointAt(i)[2];
	}

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * pMesh_->GetControlPointsCount();
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = pVertices_;
	Renderer::GetDevice()->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	//インデックス情報取得
	D3D11_BUFFER_DESC bd_index;
	bd_index.ByteWidth = sizeof(int) * pMesh_->GetPolygonVertexCount();
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_index.CPUAccessFlags = 0;
	bd_index.MiscFlags = 0;
	bd_index.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_index;
	data_index.pSysMem = pMesh_->GetPolygonVertices();

	Renderer::GetDevice()->CreateBuffer(&bd_index, &data_index, &pIndexBuffer_);

	return true;
}

void ActorFbxModel::Uninit()
{
	pMesh_->Destroy();
	pMesh_ = nullptr;
	pFbxScene_->Destroy();
	pFbxScene_ = nullptr;
	pFbxManager_->Destroy();
	pFbxManager_ = nullptr;

	SafeRelease(pVertexBuffer_);
	SafeRelease(pIndexBuffer_);
	delete[] pVertices_;
}

void ActorFbxModel::Update()
{
	{
		ShaderFbx::CONSTANT_BUFFER cb;
		cb.mtxWorld = EditMath::Transpose(mtxWorld_);
		cb.mtxView = EditMath::Transpose(pCameraSelecter_->GetSelectCamera()->GetMtxView());
		cb.mtxProj = EditMath::Transpose(pCameraSelecter_->GetSelectCamera()->GetMtxProjection());
		
		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::FBX), 0, NULL, &cb, 0, 0);
	}
}

void ActorFbxModel::Stats()
{
	Actor3d::Stats();
}

void ActorFbxModel::Draw()
{
	//頂点バッファセット
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//インデックスバッファセット
	Renderer::GetDeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//描画方法
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//インプットレイアウト指定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::FBX));

	//シェーダセット
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::FBX), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::FBX), nullptr, 0);

	//コンスタントバッファセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::FBX));

	//描画
	Renderer::GetDeviceContext()->DrawIndexed(pMesh_->GetPolygonVertexCount(), 0, 0);
}