//**-------------------------------------------------------**
//**
//**                  actor_cube.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_CUBE_H_
#define _ACTOR_CUBE_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "actor_3d.h"

class ActorCameraSelecter;

class ActorCube : public Actor3d
{
private:
	struct VERTEX3D
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	ActorCameraSelecter* pCameraSelecter_;
	ID3D11Buffer*			pVertexBuffer_;
	ID3D11Buffer*			pIndexBuffer_;
public:
	ActorCube(ActorManager* pActorManager);
	virtual ~ActorCube()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_CUBE_H_
