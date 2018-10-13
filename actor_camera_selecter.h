//**-------------------------------------------------------**
//**
//**              actor_camera_selecter.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_CAMERA_SELECTER_H_
#define _ACTOR_CAMERA_SELECTER_H_

#include <array>
#include "actor.h"

class ActorCamera;

class ActorCameraSelecter : public Actor
{
public:
	enum TYPE
	{
		TYPE_UNKNOWN = -1,
		TYPE_FREE_CAMERA_01,
		TYPE_TRACKING_CAMERA_01,
		TYPE_MAX
	};

	ActorCameraSelecter(ActorManager* pActorManager);
	virtual ~ActorCameraSelecter()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
	void SelectType(int type);
	void SelectNextType();
	ActorCamera* GetSelectCamera();

private:
	std::array <ActorCamera*, TYPE_MAX> cameraArray_;
	int selectType_;
};

#endif // !_ACTOR_CAMERA_SELECTER_H_
