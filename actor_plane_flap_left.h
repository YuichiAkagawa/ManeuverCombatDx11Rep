//**-------------------------------------------------------**
//**
//**               actor_plane_flap_left.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_FLAP_LEFT_H_
#define _ACTOR_PLANE_FLAP_LEFT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneFlapLeft : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneFlapLeft(ActorManager* pActorManager);
	virtual ~ActorPlaneFlapLeft()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_FLAP_LEFT_H_
