//**-------------------------------------------------------**
//**
//**             actor_hierarchy_animator.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_hierarchy.h"
#include "actor_hierarchy_animation_manager.h"
#include "actor_manager.h"
#include "actor_hierarchy_animator.h"

ActorHierarchyAnimator::ActorHierarchyAnimator(ActorManager* pActorManager) : Actor(pActorManager)
{
	name_ = ActorManager::NAME_HIERARCHY_ANIMATOR;
	tag_ = ActorManager::TAG_HIERARCHY_ANIMATOR;
	layer_ = ActorManager::LAYER_ETC;
}

ActorHierarchyAnimator::~ActorHierarchyAnimator()
{

}

bool ActorHierarchyAnimator::Init()
{
	nameUnique_ = "ActorHierarchyAnimator" + std::to_string(number_);

	return true;
}

void ActorHierarchyAnimator::Uninit()
{

}

void ActorHierarchyAnimator::Update()
{
	//現在のフレームからキーを取得
	for (unsigned int i = 0; i < animationList_[animation_].size() - 1; i++)
	{
		if (animationList_[animation_][i].frame <= numFrame_
			&& animationList_[animation_][i + 1].frame >= numFrame_)
		{
			numKey_ = i;
			break;
		}
	}

	//現在のフレームがキーフレームの範囲内の場合のみ動かす
	if (animationList_[animation_][animationList_[animation_].size() - 1].frame >= numFrame_
		&& animationList_[animation_][0].frame <= numFrame_)
	{

		//差分回転角
		float rotAxis = animationList_[animation_][numKey_ + 1].rotAxis - animationList_[animation_][numKey_].rotAxis;

		//差分フレーム数
		int frame = animationList_[animation_][numKey_ + 1].frame - animationList_[animation_][numKey_].frame;

		//1フレーム当たりの回転角
		float addRotAxis = rotAxis / (float)frame * (numFrame_ - animationList_[animation_][numKey_].frame);

		//回転角加算
		pActorHierarchy_->SetRotAxis(animationList_[animation_][numKey_].rotAxis + addRotAxis);
	}
}

void ActorHierarchyAnimator::Stats()
{
	Actor::Stats();
}

void ActorHierarchyAnimator::Draw()
{

}

void ActorHierarchyAnimator::SetActorHierarchy(ActorHierarchy* pActorHierarchy)
{
	pActorHierarchy_ = pActorHierarchy;
}

void ActorHierarchyAnimator::SetNumFrame(int numFrame)
{
	numFrame_ = numFrame;
}

void ActorHierarchyAnimator::SetAnimation(int animation)
{
	animation_ = animation;
}

void ActorHierarchyAnimator::SetAnimationList(std::vector <std::vector<KEYFRAME>>& animationList)
{
	animationList_ = animationList;
}