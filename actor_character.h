//**-------------------------------------------------------**
//**
//**                  actor_charcter.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_CHARCTER_H_
#define _ACTOR_CHARCTER_H_

class ActorCharcter : public Actor3d
{
protected:

public:
	ActorCharcter(ActorManager* pActorManager);
	virtual ~ActorCharcter()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_CHARCTER_H_
