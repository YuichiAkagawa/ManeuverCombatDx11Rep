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

constexpr float THRUST_BASE = 0.87f;		//��{����
constexpr float THRUST_MAX = 1.32f;			//�ő各��
constexpr float THRUST_MIN = 0.01f;			//�ŏ�����
constexpr float THRUST_INCREASE = 0.003f;	//���͑�����
constexpr float THRUST_RETURN = 0.0005f;	//�f�t�H���g�l�ɖ߂�

ActorPlayer::ActorPlayer(ActorManager* pActorManager) : ActorCharcter(pActorManager)
{
	name_ = ActorManager::NAME_PLAYER;
	tag_ = ActorManager::TAG_PLAYER;
	layer_ = ActorManager::LAYER_PLAYER;

	//���͏�����
	thrust_ = THRUST_BASE;

	//�v���C���[�R���g���[���[����
	pActorPlayerController_ = new ActorPlayerController(pActorManager_);
	pActorManager_->CreateActor(pActorPlayerController_);

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

	//�ړ�
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