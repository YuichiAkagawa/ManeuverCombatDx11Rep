//**-------------------------------------------------------**
//**
//**              actor_tracking_camera.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_TRACKING_CAMERA_H_
#define _ACTOR_TRACKING_CAMERA_H_

#include <DirectXMath.h>

class ActorTrackingCamera : public ActorCamera
{
private:
	DirectX::XMFLOAT3 vecCamAt_;
	DirectX::XMFLOAT3 vecAtCam_;
	Actor3d* pTrackingActor_;
public:
	ActorTrackingCamera(ActorManager* pActorManager);
	virtual ~ActorTrackingCamera()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
	void SetTrackingActor(Actor3d* pTrackingActor);
};

#endif // !_ACTOR_TRACKING_CAMERA_H_
