//**-------------------------------------------------------**
//**
//**               actor_plane_body.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_BODY_H_
#define _ACTOR_PLANE_BODY_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneBody : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneBody(ActorManager* pActorManager);
	virtual ~ActorPlaneBody()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_HIERARCHY_ANIM_H_
