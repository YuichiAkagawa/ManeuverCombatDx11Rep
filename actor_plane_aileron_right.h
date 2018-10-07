//**-------------------------------------------------------**
//**
//**               actor_plane_aileron_right.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_AILERON_RIGHT_H_
#define _ACTOR_PLANE_AILERON_RIGHT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneAileronRight : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneAileronRight(ActorManager* pActorManager);
	virtual ~ActorPlaneAileronRight()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_AILERON_RIGHT_H_
