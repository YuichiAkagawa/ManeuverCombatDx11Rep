//**-------------------------------------------------------**
//**
//**                  actor_sea.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_SEA_H_
#define _ACTOR_SEA_H_

#include <vector>
#include <DirectXMath.h>
#include "actor_field.h"

class ActorCameraSelecter;

class ActorSea : public ActorField
{
private:
	struct VERTEX_SEA
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 vecNormal;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
		DirectX::XMFLOAT3 vecTangent;
	};

	struct OFFSET
	{
		float x, y, z;
	};

	ID3D11Buffer*					pVertexBuffer_;
	ID3D11Buffer*					pIndexBuffer_;
	ID3D11Buffer*					pOffsetBuffer_;
	ActorCameraSelecter*			pCameraSelecter_;

protected:

public:
	ActorSea(ActorManager* pActorManager);
	virtual ~ActorSea()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_SEA_H_
