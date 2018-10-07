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
	//���݂̃t���[������L�[���擾
	for (unsigned int i = 0; i < animationList_[animation_].size() - 1; i++)
	{
		if (animationList_[animation_][i].frame <= numFrame_
			&& animationList_[animation_][i + 1].frame >= numFrame_)
		{
			numKey_ = i;
			break;
		}
	}

	//���݂̃t���[�����L�[�t���[���͈͓̔��̏ꍇ�̂ݓ�����
	if (animationList_[animation_][animationList_[animation_].size() - 1].frame >= numFrame_
		&& animationList_[animation_][0].frame <= numFrame_)
	{

		//������]�p
		float rotAxis = animationList_[animation_][numKey_ + 1].rotAxis - animationList_[animation_][numKey_].rotAxis;

		//�����t���[����
		int frame = animationList_[animation_][numKey_ + 1].frame - animationList_[animation_][numKey_].frame;

		//1�t���[��������̉�]�p
		float addRotAxis = rotAxis / (float)frame * (numFrame_ - animationList_[animation_][numKey_].frame);

		//��]�p���Z
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