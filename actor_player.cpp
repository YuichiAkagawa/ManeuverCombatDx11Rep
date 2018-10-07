//**-------------------------------------------------------**
//**
//**                actor_player.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_3d.h"
#include "actor_manager.h"
#include "actor_hierarchy_animator.h"
#include "actor_hierarchy_animation_manager.h"
#include "actor_plane_body.h"
#include "actor_plane_canopy.h"
#include "actor_plane_elevator_right.h"
#include "actor_plane_elevator_left.h"
#include "actor_plane_glass_back.h"
#include "actor_plane_glass_front.h"
#include "actor_plane_radder_right.h"
#include "actor_plane_radder_left.h"
#include "actor_plane_aileron_right.h"
#include "actor_plane_aileron_left.h"
#include "actor_plane_flap_left.h"
#include "actor_plane_flap_right.h"
#include "edit_math.h"
#include "actor_player.h"

ActorPlayer::ActorPlayer(ActorManager* pActorManager) : ActorCharcter(pActorManager)
{
	name_ = ActorManager::NAME_PLAYER;
	tag_ = ActorManager::TAG_PLAYER;
	layer_ = ActorManager::LAYER_PLAYER;

	//�A�j���[�V�����}�l�[�W��
	ActorHierarchyAnimationManager* pAnimationManager = new ActorHierarchyAnimationManager(pActorManager_);
	pActorManager_->CreateActor(pAnimationManager);

	//�擪����Đ�
	pAnimationManager->PlayOutset(true, false);

	//�@�̃{�f�B����
	ActorPlaneBody* pPlaneBody = new ActorPlaneBody(pActorManager_);
	pActorManager_->CreateActor(pPlaneBody);
	pPlaneBody->SetParent(this);
	{
		//�{�f�B�p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneBodyAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneBodyAnimator);
		pPlaneBodyAnimator->SetActorHierarchy(pPlaneBody);
		pPlaneBodyAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_BODY);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneBodyAnimator);
	}

	//�O�ʃK���X����
	ActorPlaneGlassFront* pPlaneGlassFront = new ActorPlaneGlassFront(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassFront);
	pPlaneGlassFront->SetParent(pPlaneBody);

	//�L���m�s�[����
	ActorPlaneCanopy* pPlaneCanopy = new ActorPlaneCanopy(pActorManager_);
	pActorManager_->CreateActor(pPlaneCanopy);
	pPlaneCanopy->SetParent(pPlaneBody);
	{
		//�L���m�s�[�p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneCanopyAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneCanopyAnimator);
		pPlaneCanopyAnimator->SetActorHierarchy(pPlaneCanopy);
		pPlaneCanopyAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_CANOPY);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneCanopyAnimator);
	}
	//�w�ʑ��K���X�ʐ���
	ActorPlaneGlassBack* pPlaneGlassBack = new ActorPlaneGlassBack(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassBack);
	pPlaneGlassBack->SetParent(pPlaneCanopy);

	//�G���x�[�^�E����
	ActorPlaneElevatorRight* pPlaneElevatorRight = new ActorPlaneElevatorRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorRight);
	pPlaneElevatorRight->SetParent(pPlaneBody);
	{
		//�G���x�[�^�E�p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneElevatorRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneElevatorRightAnimator);
		pPlaneElevatorRightAnimator->SetActorHierarchy(pPlaneElevatorRight);
		pPlaneElevatorRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_ELEVATOR);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneElevatorRightAnimator);
	}
	//�G���x�[�^������
	ActorPlaneElevatorLeft* pPlaneElevatorLeft = new ActorPlaneElevatorLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorLeft);
	pPlaneElevatorLeft->SetParent(pPlaneBody);
	{
		//�G���x�[�^���p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneElevatorLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneElevatorLeftAnimator);
		pPlaneElevatorLeftAnimator->SetActorHierarchy(pPlaneElevatorLeft);
		pPlaneElevatorLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_ELEVATOR);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneElevatorLeftAnimator);
	}
	//���_�[�E����
	ActorPlaneRadderRight* pPlaneRadderRight = new ActorPlaneRadderRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderRight);
	pPlaneRadderRight->SetParent(pPlaneBody);
	{
		//���_�[�E�p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneRadderRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneRadderRightAnimator);
		pPlaneRadderRightAnimator->SetActorHierarchy(pPlaneRadderRight);
		pPlaneRadderRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_RADDER);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneRadderRightAnimator);
	}

	//���_�[������
	ActorPlaneRadderLeft* pPlaneRadderLeft = new ActorPlaneRadderLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderLeft);
	pPlaneRadderLeft->SetParent(pPlaneBody);
	{
		//���_�[���p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneRadderLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneRadderLeftAnimator);
		pPlaneRadderLeftAnimator->SetActorHierarchy(pPlaneRadderLeft);
		pPlaneRadderLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_RADDER);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneRadderLeftAnimator);
	}

	//�G�������E����
	ActorPlaneAileronRight* pPlaneAileronRight = new ActorPlaneAileronRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronRight);
	pPlaneAileronRight->SetParent(pPlaneBody);
	{
		//�G�������E�p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneAileronRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneAileronRightAnimator);
		pPlaneAileronRightAnimator->SetActorHierarchy(pPlaneAileronRight);
		pPlaneAileronRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneAileronRightAnimator);
	}

	//�G������������
	ActorPlaneAileronLeft* pPlaneAileronLeft = new ActorPlaneAileronLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronLeft);
	pPlaneAileronLeft->SetParent(pPlaneBody);
	{
		//�G���������p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneAileronLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneAileronLeftAnimator);
		pPlaneAileronLeftAnimator->SetActorHierarchy(pPlaneAileronLeft);
		pPlaneAileronLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneAileronLeftAnimator);
	}

	//�t���b�v�E����
	ActorPlaneFlapRight* pPlaneFlapRight = new ActorPlaneFlapRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneFlapRight);
	pPlaneFlapRight->SetParent(pPlaneBody);
	{
		//�t���b�v�E�p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneFlapRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneFlapRightAnimator);
		pPlaneFlapRightAnimator->SetActorHierarchy(pPlaneFlapRight);
		pPlaneFlapRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneFlapRightAnimator);
	}

	//�t���b�v������
	ActorPlaneFlapLeft* pPlaneFlapLeft = new ActorPlaneFlapLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneFlapLeft);
	pPlaneFlapLeft->SetParent(pPlaneBody);
	{
		//�t���b�v�p�A�j���[�^�[����
		ActorHierarchyAnimator* pPlaneFlapLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneFlapLeftAnimator);
		pPlaneFlapLeftAnimator->SetActorHierarchy(pPlaneFlapLeft);
		pPlaneFlapLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//�A�j���[�V�����}�l�[�W���ɃA�j���[�^�[�Z�b�g
		pAnimationManager->SetAnimator(pPlaneFlapLeftAnimator);
	}
}

ActorPlayer::~ActorPlayer()
{

}

bool ActorPlayer::Init()
{
	nameUnique_ = "ActorPlayer" + std::to_string(number_);

	//�X�P�[������
	EditMath::Scaling(mtxWorld_, 0.1f);

	return true;
}

void ActorPlayer::Uninit()
{

}

void ActorPlayer::Update()
{

}

void ActorPlayer::Stats()
{
	ActorCharcter::Stats();
}

void ActorPlayer::Draw()
{

}
