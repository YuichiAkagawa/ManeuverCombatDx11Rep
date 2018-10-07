//**-------------------------------------------------------**
//**
//**               actor_plane_canopy.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_CANOPY_H_
#define _ACTOR_PLANE_CANOPY_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneCanopy : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneCanopy(ActorManager* pActorManager);
	virtual ~ActorPlaneCanopy()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_CANOPY_H_
