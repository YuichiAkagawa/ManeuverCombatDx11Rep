#ifndef _LOAD_FBX_H_
#define _LOAD_FBX_H_

#include <fbxsdk.h>
#include <unordered_map>
#include <string>
#include <DirectXMath.h>
#include "model.h"

struct FBXANIMATION
{
	std::string name;
	std::vector<BONE> bone;
	int frameNum;
	int startFrame;
	int endFrame;
	FbxTime framePeriod;
};

struct VERTEXFBX
{
	DirectX::XMFLOAT3  position;
	DirectX::XMFLOAT3  normal;
	DirectX::XMFLOAT4  color;
	DirectX::XMFLOAT2  texcoord;
	DirectX::XMFLOAT3  positionDefault;
//	std::vector<VERTEXBONE> bone;
};

struct VERTEXBONE
{
	float boneIndex;
	float weight;
};

struct VERTEXPOINTBONE
{
	DirectX::XMFLOAT3 position;
	VERTEXBONE boneData[4];
};

struct UVSet
{
	std::string uvSetName;
	std::string texName;
};

struct FBXUV
{
	std::string uvSetName;
	std::vector<DirectX::XMFLOAT2> texcoord;
};

struct MESH_
{
	int   pointCount;
	int   polygonCount;
	int   indexCount;
	UVSet uvSet;
	unsigned int  vertexShader;
	std::vector<VERTEXFBX> vertex;
	std::vector<BONE>      bone;
};

class Systems;
class LoadFBX
{
	friend class Systems;
public:
	// コンストラクタ
	LoadFBX(void) {}
	// デストラクタ
	~LoadFBX(void);

	bool Load(std::string fileName, MODEL& model);
	bool LoadAnimation(std::string fileName, MODEL& model);

private:

	// Common.cxx参照
	void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
	bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename, bool animation);

	// メッシュを数える
	void	CountMesh(FbxNode* pNode, MODEL& model);
	// メッシュの情報を取得する
	bool GetMesh(FbxNode* pNode, MODEL& model);

	void	GetPosition(FbxMesh* mesh);					// 頂点座標
	void	GetMeshMaterialList(FbxMesh* mesh);			// メッシュマテリアルリストの取得
	void	GetBone(FbxMesh* mesh);						// ボーン
	void	GetVertexNormal(FbxMesh* mesh);				// 法線
	void	GetVertexTangent(FbxMesh* mesh);			// tangent
	void	GetVertexUV(FbxMesh* mesh);					// テクスチャ座標
	void	GetTextureNames(FbxMesh* mesh);				// テクスチャ
	void	GetVertexColor(FbxMesh* mesh);				// 色

	bool CreateModelData(MODEL& model);
	bool CreateVeretx(MODEL& model, int indexNum, const int* index, UVSet uv);

	void CreateMtx(DirectX::XMFLOAT4X4& mtx, const FbxMatrix fbxMtx);
	void ReleaseTempObj(void);			// 一時情報の解放
	void ReleaseTempAnimObj(void);			// 一時情報の解放

	void GetAnimation(FBXANIMATION& animInfo);

	// マテリアルの数(0 = CountMesh, 1 = GetMesh)
	int materialCount_[2] = { 0, 0 };

	// FBX
	FbxManager* lSdkManager_ = NULL;
	FbxScene*   lScene_ = NULL;

	// 一時変数
	std::string	fileName_;
	int		pointCount_;
	int		polygonCount_;
	unsigned int		indexCount_;
	std::vector<VERTEXPOINTBONE>			tempPoint_;
	std::vector<VERTEXPOINTBONE>			tempVertex_;
	std::vector<DirectX::XMFLOAT3>			tempNormal_;
	std::vector<DirectX::XMFLOAT3>			tempTangent_;
	std::vector<DirectX::XMFLOAT4>			tempColor_;
	std::vector<FBXUV>						tempTexcoord_;
	std::vector<UVSet>						tempUVSet_;
	std::vector<FBXANIMATION>				tempAnim_;
	std::vector<int>						meshMaterialList_;

	std::vector<BONE>						boneList_;

	std::vector<FBXANIMATION>				animationStack_;
	std::vector<FbxNode*>					tempNode_;
	std::vector<BONE>						animBoneList_;

	DirectX::XMFLOAT4X4 mtx_;		//変換全体
	DirectX::XMFLOAT4X4 mtxViewZ_;	//Zバッファ用
	DirectX::XMFLOAT4X4 mtxProjZ_;
};

#endif //_LOAD_FBX_H_
