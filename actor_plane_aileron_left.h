//**-------------------------------------------------------**
//**
//**               actor_plane_aileron_left.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_AILERON_LEFT_H_
#define _ACTOR_PLANE_AILERON_LEFT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneAileronLeft : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneAileronLeft(ActorManager* pActorManager);
	virtual ~ActorPlaneAileronLeft()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_AILERON_LEFT_H_
