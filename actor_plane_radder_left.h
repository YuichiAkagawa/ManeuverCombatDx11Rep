//**-------------------------------------------------------**
//**
//**               actor_plane_radder_left.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_RADDER_LEFT_H_
#define _ACTOR_PLANE_RADDER_LEFT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneRadderLeft : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneRadderLeft(ActorManager* pActorManager);
	virtual ~ActorPlaneRadderLeft()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_RADDER_LEFT_H_
