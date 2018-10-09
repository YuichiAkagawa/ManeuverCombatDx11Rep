//**-------------------------------------------------------**
//**
//**                   actor_grid.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_GRID_H_
#define _ACTOR_GRID_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "actor_field.h"

class ActorCameraSelecter;

class ActorGrid : public ActorField
{
private:
	struct VERTEX_GRID
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	ActorCameraSelecter*	pCameraSelecter_;
	ID3D11Buffer*			pVertexBuffer_;
	ID3D11Buffer*			pVertexBufferCenter_;
public:
	ActorGrid(ActorManager* pActorManager);
	virtual ~ActorGrid()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_GRID_H_
