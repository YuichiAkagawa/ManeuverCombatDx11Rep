//**-------------------------------------------------------**
//**
//**               actor_plane_glass_back.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_GLASS_BACK_H_
#define _ACTOR_PLANE_GLASS_BACK_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneGlassBack : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneGlassBack(ActorManager* pActorManager);
	virtual ~ActorPlaneGlassBack()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_GLASS_BACK_H_
