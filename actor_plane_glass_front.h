//**-------------------------------------------------------**
//**
//**               actor_plane_glass_front.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_GLASS_FRONT_H_
#define _ACTOR_PLANE_GLASS_FRONT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneGlassFront : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneGlassFront(ActorManager* pActorManager);
	virtual ~ActorPlaneGlassFront()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_GLASS_FRONT_H_
