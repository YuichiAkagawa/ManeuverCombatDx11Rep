//**-------------------------------------------------------**
//**
//**               actor_plane_elevator_right.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLANE_ELEVATOR_RIGHT_H_
#define _ACTOR_PLANE_ELEVATOR_RIGHT_H_

#include "actor_hierarchy.h"

class ActorCameraSelecter;

class ActorPlaneElevatorRight : public ActorHierarchy
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorPlaneElevatorRight(ActorManager* pActorManager);
	virtual ~ActorPlaneElevatorRight()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLANE_ELEVATOR_RIGHT_H_
