//**-------------------------------------------------------**
//**
//**                  actor_skydome.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_SKYDOME_H_
#define _ACTOR_SKYDOME_H_

#include "actor_field.h"

class ActorSkydome : public ActorField
{
private:
	ActorCameraSelecter* pCameraSelecter_;
protected:

public:
	ActorSkydome(ActorManager* pActorManager);
	virtual ~ActorSkydome()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_SKYDOME_H_
