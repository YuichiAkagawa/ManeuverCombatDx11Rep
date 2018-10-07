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
	DirectX::XMFLOAT3 tangent;
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
	{ "resource/model/hornet.fbx" },
	{ "resource/model/hornet_pilot.fbx" },
	{ "resource/model/hornet_body.fbx" },
	{ "resource/model/hornet_body_parts.fbx" },
	{ "resource/model/hornet_body_parts2.fbx" },
	{ "resource/model/hornet_aim120.fbx" },
	{ "resource/model/hornet_aim9_right.fbx" },
	{ "resource/model/hornet_aim9_left.fbx" },
	{ "resource/model/hornet_flap_right.fbx" },
	{ "resource/model/hornet_flap_left.fbx" },
	{ "resource/model/hornet_elevator_right.fbx" },
	{ "resource/model/hornet_elevator_left.fbx" },
	{ "resource/model/hornet_aileron_right.fbx" },
	{ "resource/model/hornet_aileron_left.fbx" },
	{ "resource/model/hornet_radder_right.fbx" },
	{ "resource/model/hornet_radder_left.fbx" },
	{ "resource/model/hornet_canopy.fbx" },
	{ "resource/model/hornet_glass_front.fbx" },
	{ "resource/model/hornet_glass_back.fbx" },
	{ "resource/model/skydome.fbx" },
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
		HORNET_PILOT,
		HORNET_BODY,
		HORNET_BODY_PARTS,
		HORNET_BODY_PARTS2,
		HORNET_AIM120,
		HORNET_AIM9_RIGHT,
		HORNET_AIM9_LEFT,
		HORNET_FLAP_RIGHT,
		HORNET_FLAP_LEFT,
		HORNET_ELEVATOR_RIGHT,
		HORNET_ELEVATOR_LEFT,
		HORNET_AILERON_RIGHT,
		HORNET_AILERON_LEFT,
		HORNET_RADDER_RIGHT,
		HORNET_RADDER_LEFT,
		HORNET_CANOPY,
		HORNET_GLASS_FRONT,
		HORNET_GLASS_BACK,
		SKYDOME,
		MAX
	};

private:
	Model* pInst_[MODEL_LIST::MAX];
};
#endif // !_MODEL_H_
