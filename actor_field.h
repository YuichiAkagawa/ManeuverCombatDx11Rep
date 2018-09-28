//**-------------------------------------------------------**
//**
//**                  actor_field.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_FIELD_H_
#define _ACTOR_FIELD_H_

class ActorField : public Actor3d
{
protected:

public:
	ActorField(ActorManager* pActorManager);
	virtual ~ActorField()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_FIELD_H_
