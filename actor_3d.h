//**-------------------------------------------------------**
//**
//**                     actor_3d.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR3D_H_
#define _ACTOR3D_H_

#include <DirectXMath.h>
#include "actor.h"

class Actor3d : public Actor
{
protected:
	DirectX::XMFLOAT3 pos_ = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 vecFront_ = { 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT3 vecRight_ = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 vecUp_ = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT4X4 mtxWorld_;
public:
	Actor3d(ActorManager* pActorManager);
	virtual ~Actor3d()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
	
	const DirectX::XMFLOAT3& GetPos()const;
	const DirectX::XMFLOAT3& GetVecFront()const;
	const DirectX::XMFLOAT3& GetVecRight()const;
	const DirectX::XMFLOAT3& GetVecUp()const;
	const DirectX::XMFLOAT4X4& GetMtxWorld()const;

	void SetPos(const DirectX::XMFLOAT3& pos);
	void SetVecFront(const DirectX::XMFLOAT3& vecFront);
	void SetVecRight(const DirectX::XMFLOAT3& vecRight);
	void SetVecUp(const DirectX::XMFLOAT3& vecUp);
	void SetMtxWorld(const DirectX::XMFLOAT4X4& mtxWorld);
};

#endif // !_ACTOR_H_
