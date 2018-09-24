//**-------------------------------------------------------**
//**
//**                     model.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <string>
#include <vector>
#include <DirectXMath.h>

struct VERTEX
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT3 binormal;
	DirectX::XMFLOAT4 boneIndex;
	DirectX::XMFLOAT4 weight;
};

struct MATERIAL
{
	DirectX::XMFLOAT4				ambient;
	DirectX::XMFLOAT4				diffuse;
	DirectX::XMFLOAT4				specular;
	DirectX::XMFLOAT4				emission;
	float							power;
	ID3D11ShaderResourceView*		texture;
	std::string						textureName;
};

struct BONE
{
	std::string		name;
	DirectX::XMFLOAT4X4	initMtx;
	std::vector<std::vector<DirectX::XMFLOAT4X4>> animMtx;
};

struct MESH
{
	DirectX::XMFLOAT4X4			frameTransMtx;
	std::vector<VERTEX>			vertex;
	std::vector<WORD>			index;
	ID3D11Buffer*				vertexBuffer;
	ID3D11Buffer*				indexBuffer;
	MATERIAL					material;
};

struct MODEL
{
	DirectX::XMFLOAT4X4		frameTransMtx;
	std::vector<MESH>	mesh;

	std::vector<BONE>	bone;
};

static const std::string g_pModelFile[] =
{
	{ "resource/model/sample.fbx" },
};

class LoadFBX;

class Model
{
private:
	MODEL model_;
	LoadFBX* pLoadFbx_;
public:
	bool Init(int modelName);
	void Uninit();
	void Draw();
};

class ModelManager
{
public:
	bool Init();
	void Uninit();
	Model* GetInstance(int modelName);

	enum MODEL_LIST
	{
		UNKNOWN = -1,
		SAMPLE,
		MAX
	};

private:
	Model* pInst_[MODEL_LIST::MAX];
};
#endif // !_MODEL_H_
