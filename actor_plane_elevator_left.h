//**-------------------------------------------------------**
//**
//**               actor_plane_elevator_left.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_ELEVATOR_LEFT_H_
#define _ACTOR_PLANE_ELEVATOR_LEFT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneElevatorLeft : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneElevatorLeft(ActorManager* pActorManager);
	virtual ~ActorPlaneElevatorLeft()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_ELEVATOR_LEFT_H_
