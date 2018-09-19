//**-------------------------------------------------------**
//**
//**                  actor_camera.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_CAMERA_H_
#define _ACTOR_CAMERA_H_

#include "actor_3d.h"

class ActorCamera : public Actor3d
{
protected:
	DirectX::XMFLOAT3 posAt_;
	DirectX::XMFLOAT4X4 mtxView_;
	DirectX::XMFLOAT4X4 mtxProjection_;
public:
	ActorCamera(ActorManager* pActorManager);
	virtual ~ActorCamera()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
	
	//Getter
	const DirectX::XMFLOAT4X4& GetMtxView()const;
	const DirectX::XMFLOAT4X4& GetMtxProjection()const;
};

#endif // !_ACTOR_CAMERA_H_
