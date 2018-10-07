//**-------------------------------------------------------**
//**
//**        actor_hierarchy_animation_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_HIERARCHY_ANIMATION_MANAGER_H_
#define _ACTOR_HIERARCHY_ANIMATION_MANAGER_H_

#include <list>
#include <vector>
#include "actor.h"

static const char *g_pAnimFile[] =
{
	{ "resource/anim/canopy.anim" },
	{ "resource/anim/elevator.anim" },
	{ "resource/anim/radder.anim" },
	{ "resource/anim/aileron_flap.anim" },
	{ "resource/anim/body.anim" },
};

struct KEYFRAME
{
	int frame;
	float rotAxis;
};

class ActorHierarchyAnimator;

class ActorHierarchyAnimationManager : public Actor
{
private:
	std::list <ActorHierarchyAnimator*> animatorList_;		//管理する対象のアニメーター
	std::vector <std::vector<KEYFRAME>> animationList_;		//アニメーションリスト
	int numFrame_ = 0;
	bool isPlay = false;
	bool isReverse_ = false;
	bool isRepeat_ = false;
	int maxFrame_ = 0;				//格納されているアニメーションの最大フレーム数

	bool Load();
	bool Save();
public:
	enum
	{
		ANIMATION_UNKNOWN = -1,
		ANIMATION_CANOPY,
		ANIMATION_ELEVATOR,
		ANIMATION_RADDER,
		ANIMETION_AILERON_FLAP,
		ANIMATION_BODY,
		ANIMATION_MAX
	};

	ActorHierarchyAnimationManager(ActorManager* pActorManager);
	virtual ~ActorHierarchyAnimationManager()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;

	void PlayOutset();
	void PlayOutset(bool isRepeat, bool isReverse);
	void PlayOnTheWay();
	void PlayOnTheWay(bool isRepeat, bool isReverse);
	void PlayOnTheWay(bool isRepeat, bool isReverse, int numFrame);
	void Pause();
	void Stop();

	//Setter
	void SetAnimator(ActorHierarchyAnimator* pAinmator);

};

#endif // !_ACTOR_HIERARCHY_ANIMATION_MANAGER_H_