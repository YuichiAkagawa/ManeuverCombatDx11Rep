//**-------------------------------------------------------**
//**
//**                actor_player.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_3d.h"
#include "actor_manager.h"
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

	//�@�̃{�f�B����
	ActorPlaneBody* pPlaneBody = new ActorPlaneBody(pActorManager_);
	pActorManager_->CreateActor(pPlaneBody);
	pPlaneBody->SetParent(this);

	//�O�ʃK���X����
	ActorPlaneGlassFront* pPlaneGlassFront = new ActorPlaneGlassFront(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassFront);
	pPlaneGlassFront->SetParent(pPlaneBody);

	//�L���m�s�[����
	ActorPlaneCanopy* pPlaneCanopy = new ActorPlaneCanopy(pActorManager_);
	pActorManager_->CreateActor(pPlaneCanopy);
	pPlaneCanopy->SetParent(pPlaneBody);

	//�w�ʑ��K���X�ʐ���
	ActorPlaneGlassBack* pPlaneGlassBack = new ActorPlaneGlassBack(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassBack);
	pPlaneGlassBack->SetParent(pPlaneCanopy);

	//�G���x�[�^�E����
	ActorPlaneElevatorRight* pPlaneElevatorRight = new ActorPlaneElevatorRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorRight);
	pPlaneElevatorRight->SetParent(pPlaneBody);

	//�G���x�[�^������
	ActorPlaneElevatorLeft* pPlaneElevatorLeft = new ActorPlaneElevatorLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorLeft);
	pPlaneElevatorLeft->SetParent(pPlaneBody);

	//���_�[�E����
	ActorPlaneRadderRight* pPlaneRadderRight = new ActorPlaneRadderRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderRight);
	pPlaneRadderRight->SetParent(pPlaneBody);

	//���_�[������
	ActorPlaneRadderLeft* pPlaneRadderLeft = new ActorPlaneRadderLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderLeft);
	pPlaneRadderLeft->SetParent(pPlaneBody);

	//�G�������E����
	ActorPlaneAileronRight* pPlaneAileronRight = new ActorPlaneAileronRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronRight);
	pPlaneAileronRight->SetParent(pPlaneBody);

	//�G������������
	ActorPlaneAileronLeft* pPlaneAileronLeft = new ActorPlaneAileronLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronLeft);
	pPlaneAileronLeft->SetParent(pPlaneBody);

	//�t���b�v�E����
	ActorPlaneFlapRight* pPlaneFlapRight = new ActorPlaneFlapRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneFlapRight);
	pPlaneFlapRight->SetParent(pPlaneBody);

	//�t���b�v������
	ActorPlaneFlapLeft* pPlaneFlapLeft = new ActorPlaneFlapLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneFlapLeft);
	pPlaneFlapLeft->SetParent(pPlaneBody);
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
