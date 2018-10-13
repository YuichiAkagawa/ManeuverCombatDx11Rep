//**-------------------------------------------------------**
//**
//**                  actor_player.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLAYER_H_
#define _ACTOR_PLAYER_H_

#include "actor_character.h"

class ActorPlayerController;

class ActorPlayer : public ActorCharcter
{
private:
	ActorPlayerController* pActorPlayerController_;
	float thrust_;

	void Yaw();
	void Pitch();
	void Roll();
	void Thrust();
public:
	ActorPlayer(ActorManager* pActorManager);
	virtual ~ActorPlayer()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_PLAYER_H_
