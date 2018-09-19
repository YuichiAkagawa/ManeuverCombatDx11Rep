//**-------------------------------------------------------**
//**
//**                 actor_free_camera.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_FREE_CAMERA_H_
#define _ACTOR_FREE_CAMERA_H_

#include <DirectXMath.h>

class ActorFreeCamera : public ActorCamera
{
private:
	DirectX::XMFLOAT3 vecCamAt_;
	DirectX::XMFLOAT3 vecAtCam_;
public:
	ActorFreeCamera(ActorManager* pActorManager);
	virtual ~ActorFreeCamera()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;

};

#endif // !_ACTOR_FREE_CAMERA_H_
