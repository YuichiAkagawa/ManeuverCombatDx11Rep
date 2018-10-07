//**-------------------------------------------------------**
//**
//**               actor_plane_flap_right.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_FLAP_RIGHT_H_
#define _ACTOR_PLANE_FLAP_RIGHT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneFlapRight : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneFlapRight(ActorManager* pActorManager);
	virtual ~ActorPlaneFlapRight()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_FLAP_RIGHT_H_
