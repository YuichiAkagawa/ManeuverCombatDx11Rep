//**-------------------------------------------------------**
//**
//**                actor_player.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <DirectXMath.h>
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
#include "actor_player_controller.h"
#include "edit_math.h"
#include "main.h"
#include "actor_player.h"

using namespace DirectX;

constexpr float YAW_SPEED = 0.005f;
constexpr float PITCH_SPEED = 0.02f;
constexpr float ROLL_SPEED = 0.02f;

constexpr float THRUST_BASE = 0.87f;		//基本推力
constexpr float THRUST_MAX = 1.32f;			//最大推力
constexpr float THRUST_MIN = 0.01f;			//最小推力
constexpr float THRUST_INCREASE = 0.003f;	//推力増加量
constexpr float THRUST_RETURN = 0.0005f;	//デフォルト値に戻す

ActorPlayer::ActorPlayer(ActorManager* pActorManager) : ActorCharcter(pActorManager)
{
	name_ = ActorManager::NAME_PLAYER;
	tag_ = ActorManager::TAG_PLAYER;
	layer_ = ActorManager::LAYER_PLAYER;

	//推力初期化
	thrust_ = THRUST_BASE;

	//プレイヤーコントローラー生成
	pActorPlayerController_ = new ActorPlayerController(pActorManager_);
	pActorManager_->CreateActor(pActorPlayerController_);

	//機体ボディ生成
	ActorPlaneBody* pPlaneBody = new ActorPlaneBody(pActorManager_);
	pActorManager_->CreateActor(pPlaneBody);
	pPlaneBody->SetParent(this);

	//前面ガラス生成
	ActorPlaneGlassFront* pPlaneGlassFront = new ActorPlaneGlassFront(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassFront);
	pPlaneGlassFront->SetParent(pPlaneBody);

	//キャノピー生成
	ActorPlaneCanopy* pPlaneCanopy = new ActorPlaneCanopy(pActorManager_);
	pActorManager_->CreateActor(pPlaneCanopy);
	pPlaneCanopy->SetParent(pPlaneBody);

	//背面側ガラス面生成
	ActorPlaneGlassBack* pPlaneGlassBack = new ActorPlaneGlassBack(pActorManager_);
	pActorManager_->CreateActor(pPlaneGlassBack);
	pPlaneGlassBack->SetParent(pPlaneCanopy);

	//エレベータ右生成
	ActorPlaneElevatorRight* pPlaneElevatorRight = new ActorPlaneElevatorRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorRight);
	pPlaneElevatorRight->SetParent(pPlaneBody);

	//エレベータ左生成
	ActorPlaneElevatorLeft* pPlaneElevatorLeft = new ActorPlaneElevatorLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneElevatorLeft);
	pPlaneElevatorLeft->SetParent(pPlaneBody);

	//ラダー右生成
	ActorPlaneRadderRight* pPlaneRadderRight = new ActorPlaneRadderRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderRight);
	pPlaneRadderRight->SetParent(pPlaneBody);

	//ラダー左生成
	ActorPlaneRadderLeft* pPlaneRadderLeft = new ActorPlaneRadderLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneRadderLeft);
	pPlaneRadderLeft->SetParent(pPlaneBody);

	//エルロン右生成
	ActorPlaneAileronRight* pPlaneAileronRight = new ActorPlaneAileronRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronRight);
	pPlaneAileronRight->SetParent(pPlaneBody);

	//エルロン左生成
	ActorPlaneAileronLeft* pPlaneAileronLeft = new ActorPlaneAileronLeft(pActorManager_);
	pActorManager_->CreateActor(pPlaneAileronLeft);
	pPlaneAileronLeft->SetParent(pPlaneBody);

	//フラップ右生成
	ActorPlaneFlapRight* pPlaneFlapRight = new ActorPlaneFlapRight(pActorManager_);
	pActorManager_->CreateActor(pPlaneFlapRight);
	pPlaneFlapRight->SetParent(pPlaneBody);

	//フラップ左生成
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

	//スケール調整
	EditMath::Scaling(mtxWorld_, 0.0075f);

	return true;
}

void ActorPlayer::Uninit()
{

}

void ActorPlayer::Update()
{
	Yaw();
	Pitch();
	Roll();
	Thrust();

	//移動
	XMFLOAT3 trans;
	EditMath::Multiplication(trans, vecFront_, thrust_);
	EditMath::Addition(pos_, pos_, trans);
	mtxWorld_._41 = pos_.x;
	mtxWorld_._42 = pos_.y;
	mtxWorld_._43 = pos_.z;
}

void ActorPlayer::Stats()
{
	ActorCharcter::Stats();
}

void ActorPlayer::Draw()
{

}

void ActorPlayer::Yaw()
{
	XMFLOAT4X4 mtxYaw;
	EditMath::RotationQuaternion(mtxYaw, VEC_Y, pActorPlayerController_->Yaw() * YAW_SPEED);
	EditMath::Multiplication(mtxWorld_, mtxYaw, mtxWorld_);

	EditMath::RotationQuaternion(mtxYaw, vecUp_, pActorPlayerController_->Yaw() * YAW_SPEED);
	
	EditMath::Transform(vecFront_, vecFront_, mtxYaw);
	EditMath::Normalize(vecFront_, vecFront_);
	
	EditMath::Transform(vecUp_, vecUp_, mtxYaw);
	EditMath::Normalize(vecUp_, vecUp_);
	
	EditMath::Transform(vecRight_, vecRight_, mtxYaw);
	EditMath::Normalize(vecRight_, vecRight_);
}

void ActorPlayer::Pitch()
{
	XMFLOAT4X4 mtxPitch;
	EditMath::RotationQuaternion(mtxPitch, VEC_X, pActorPlayerController_->Pitch() * PITCH_SPEED);
	EditMath::Multiplication(mtxWorld_, mtxPitch, mtxWorld_);

	EditMath::RotationQuaternion(mtxPitch, vecRight_, pActorPlayerController_->Pitch() * PITCH_SPEED);
	
	EditMath::Transform(vecFront_, vecFront_, mtxPitch);
	EditMath::Normalize(vecFront_, vecFront_);
	
	EditMath::Transform(vecUp_, vecUp_, mtxPitch);
	EditMath::Normalize(vecUp_, vecUp_);
	
	EditMath::Transform(vecRight_, vecRight_, mtxPitch);
	EditMath::Normalize(vecRight_, vecRight_);
}

void ActorPlayer::Roll()
{
	XMFLOAT4X4 mtxRoll;
	EditMath::RotationQuaternion(mtxRoll, VEC_Z, pActorPlayerController_->Roll() * ROLL_SPEED);
	EditMath::Multiplication(mtxWorld_, mtxRoll, mtxWorld_);

	EditMath::RotationQuaternion(mtxRoll, vecFront_, pActorPlayerController_->Roll() * ROLL_SPEED);
	
	EditMath::Transform(vecFront_, vecFront_, mtxRoll);
	EditMath::Normalize(vecFront_, vecFront_);
	
	EditMath::Transform(vecUp_, vecUp_, mtxRoll);
	EditMath::Normalize(vecUp_, vecUp_);
	
	EditMath::Transform(vecRight_, vecRight_, mtxRoll);
	EditMath::Normalize(vecRight_, vecRight_);
}

void ActorPlayer::Thrust()
{
	thrust_ += pActorPlayerController_->Thrust() * THRUST_INCREASE;
	if (thrust_ > THRUST_BASE)
	{
		thrust_ -= THRUST_RETURN;
	}
	else if(thrust_ < THRUST_BASE)
	{
		thrust_ += THRUST_RETURN;
	}
	thrust_ = max(min(thrust_, THRUST_MAX), THRUST_MIN);
}