//**-------------------------------------------------------**
//**
//**             actor_hierarchy_animator.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_HIERARCHY_ANIMATOR_H_
#define _ACTOR_HIERARCHY_ANIMATOR_H_

#include <vector>
#include "actor.h"
#include "actor_hierarchy_animation_manager.h"

class ActorHierarchy;

class ActorHierarchyAnimator : public Actor
{
private:
	ActorHierarchy* pActorHierarchy_ = nullptr;							//�ΏۂƂ���K�w�A�N�^
	std::vector <std::vector<KEYFRAME>> animationList_;				//�A�j���[�V�������X�g
	int numFrame_ = 0;
	int numKey_ = 0;
	int animation_ = 0;			//�Đ�����A�j���[�V����
public:
	ActorHierarchyAnimator(ActorManager* pActorManager);
	virtual ~ActorHierarchyAnimator()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;

	void SetActorHierarchy(ActorHierarchy* pActorHierarchy);
	void SetNumFrame(int numFrame);
	void SetAnimation(int animation);
	void SetAnimationList(std::vector <std::vector<KEYFRAME>>& animationList);
};

#endif // !_ACTOR_HIERARCHY_ANIMATOR_H_