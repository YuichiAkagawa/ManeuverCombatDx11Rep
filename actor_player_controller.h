//**-------------------------------------------------------**
//**
//**             actor_player_controller.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLAYER_CONTROLLER_H_
#define _ACTOR_PLAYER_CONTROLLER_H_

#include "actor.h"

class ActorPlayerController : public Actor
{
protected:

public:
	ActorPlayerController(ActorManager* pActorManager);
	virtual ~ActorPlayerController()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;

};

#endif // !_ACTOR_PLAYER_CONTROLLER_H_
