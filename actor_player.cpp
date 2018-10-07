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

	//アニメーションマネージャ
	ActorHierarchyAnimationManager* pAnimationManager = new ActorHierarchyAnimationManager(pActorManager_);
	pActorManager_->CreateActor(pAnimationManager);

	//先頭から再生
	pAnimationManager->PlayOutset(true, false);

	//機体ボディ生成
	ActorPlaneBody* pPlaneBody = new ActorPlaneBody(pActorManager_);
	pActorManager_->CreateActor(pPlaneBody);
	pPlaneBody->SetParent(this);
	{
		//ボディ用アニメーター生成
		ActorHierarchyAnimator* pPlaneBodyAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneBodyAnimator);
		pPlaneBodyAnimator->SetActorHierarchy(pPlaneBody);
		pPlaneBodyAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_BODY);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneBodyAnimator);
	}

	//前面ガラス生成
	ActorPlaneGlassFront* pPlaneGlassFront = new ActorPlaneGlassFront(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassFront);
	pPlaneGlassFront->SetParent(pPlaneBody);

	//キャノピー生成
	ActorPlaneCanopy* pPlaneCanopy = new ActorPlaneCanopy(pActorManager_);
	pActorManager_->CreateActor(pPlaneCanopy);
	pPlaneCanopy->SetParent(pPlaneBody);
	{
		//キャノピー用アニメーター生成
		ActorHierarchyAnimator* pPlaneCanopyAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneCanopyAnimator);
		pPlaneCanopyAnimator->SetActorHierarchy(pPlaneCanopy);
		pPlaneCanopyAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_CANOPY);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneCanopyAnimator);
	}
	//背面側ガラス面生成
	ActorPlaneGlassBack* pPlaneGlassBack = new ActorPlaneGlassBack(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassBack);
	pPlaneGlassBack->SetParent(pPlaneCanopy);

	//エレベータ右生成
	ActorPlaneElevatorRight* pPlaneElevatorRight = new ActorPlaneElevatorRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorRight);
	pPlaneElevatorRight->SetParent(pPlaneBody);
	{
		//エレベータ右用アニメーター生成
		ActorHierarchyAnimator* pPlaneElevatorRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneElevatorRightAnimator);
		pPlaneElevatorRightAnimator->SetActorHierarchy(pPlaneElevatorRight);
		pPlaneElevatorRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_ELEVATOR);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneElevatorRightAnimator);
	}
	//エレベータ左生成
	ActorPlaneElevatorLeft* pPlaneElevatorLeft = new ActorPlaneElevatorLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorLeft);
	pPlaneElevatorLeft->SetParent(pPlaneBody);
	{
		//エレベータ左用アニメーター生成
		ActorHierarchyAnimator* pPlaneElevatorLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneElevatorLeftAnimator);
		pPlaneElevatorLeftAnimator->SetActorHierarchy(pPlaneElevatorLeft);
		pPlaneElevatorLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_ELEVATOR);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneElevatorLeftAnimator);
	}
	//ラダー右生成
	ActorPlaneRadderRight* pPlaneRadderRight = new ActorPlaneRadderRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderRight);
	pPlaneRadderRight->SetParent(pPlaneBody);
	{
		//ラダー右用アニメーター生成
		ActorHierarchyAnimator* pPlaneRadderRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneRadderRightAnimator);
		pPlaneRadderRightAnimator->SetActorHierarchy(pPlaneRadderRight);
		pPlaneRadderRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_RADDER);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneRadderRightAnimator);
	}

	//ラダー左生成
	ActorPlaneRadderLeft* pPlaneRadderLeft = new ActorPlaneRadderLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderLeft);
	pPlaneRadderLeft->SetParent(pPlaneBody);
	{
		//ラダー左用アニメーター生成
		ActorHierarchyAnimator* pPlaneRadderLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneRadderLeftAnimator);
		pPlaneRadderLeftAnimator->SetActorHierarchy(pPlaneRadderLeft);
		pPlaneRadderLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMATION_RADDER);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneRadderLeftAnimator);
	}

	//エルロン右生成
	ActorPlaneAileronRight* pPlaneAileronRight = new ActorPlaneAileronRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronRight);
	pPlaneAileronRight->SetParent(pPlaneBody);
	{
		//エルロン右用アニメーター生成
		ActorHierarchyAnimator* pPlaneAileronRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneAileronRightAnimator);
		pPlaneAileronRightAnimator->SetActorHierarchy(pPlaneAileronRight);
		pPlaneAileronRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneAileronRightAnimator);
	}

	//エルロン左生成
	ActorPlaneAileronLeft* pPlaneAileronLeft = new ActorPlaneAileronLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronLeft);
	pPlaneAileronLeft->SetParent(pPlaneBody);
	{
		//エルロン左用アニメーター生成
		ActorHierarchyAnimator* pPlaneAileronLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneAileronLeftAnimator);
		pPlaneAileronLeftAnimator->SetActorHierarchy(pPlaneAileronLeft);
		pPlaneAileronLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneAileronLeftAnimator);
	}

	//フラップ右生成
	ActorPlaneFlapRight* pPlaneFlapRight = new ActorPlaneFlapRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneFlapRight);
	pPlaneFlapRight->SetParent(pPlaneBody);
	{
		//フラップ右用アニメーター生成
		ActorHierarchyAnimator* pPlaneFlapRightAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneFlapRightAnimator);
		pPlaneFlapRightAnimator->SetActorHierarchy(pPlaneFlapRight);
		pPlaneFlapRightAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneFlapRightAnimator);
	}

	//フラップ左生成
	ActorPlaneFlapLeft* pPlaneFlapLeft = new ActorPlaneFlapLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneFlapLeft);
	pPlaneFlapLeft->SetParent(pPlaneBody);
	{
		//フラップ用アニメーター生成
		ActorHierarchyAnimator* pPlaneFlapLeftAnimator = new ActorHierarchyAnimator(pActorManager_);
		pActorManager_->CreateActor(pPlaneFlapLeftAnimator);
		pPlaneFlapLeftAnimator->SetActorHierarchy(pPlaneFlapLeft);
		pPlaneFlapLeftAnimator->SetAnimation(ActorHierarchyAnimationManager::ANIMETION_AILERON_FLAP);

		//アニメーションマネージャにアニメーターセット
		pAnimationManager->SetAnimator(pPlaneFlapLeftAnimator);
	}
}

ActorPlayer::~ActorPlayer()
{

}

bool ActorPlayer::Init()
{
	nameUnique_ = "ActorPlayer" + std::to_string(number_);

	//スケール調整
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
