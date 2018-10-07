//**-------------------------------------------------------**
//**
//**                  actor_player.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_PLAYER_H_
#define _ACTOR_PLAYER_H_

#include "actor_character.h"

class ActorPlayer : public ActorCharcter
{
protected:

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
