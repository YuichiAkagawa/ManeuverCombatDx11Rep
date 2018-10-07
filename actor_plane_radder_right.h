//**-------------------------------------------------------**
//**
//**               actor_plane_radder_right.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_RADDER_RIGHT_H_
#define _ACTOR_PLANE_RADDER_RIGHT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneRadderRight : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneRadderRight(ActorManager* pActorManager);
	virtual ~ActorPlaneRadderRight()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_RADDER_RIGHT_H_
