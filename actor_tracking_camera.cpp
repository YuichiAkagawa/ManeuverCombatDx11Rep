//**-------------------------------------------------------**
//**
//**             actor_tracking_camera.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <typeinfo>
#include "edit_math.h"
#include "actor.h"
#include "actor_3d.h"
#include "actor_camera.h"
#include "actor_manager.h"
#include "input.h"
#include "main.h"
#include "renderer.h"
#include "scene_manager.h"
#include "texture.h"
#include "actor_tracking_camera.h"

using namespace DirectX;

static const XMFLOAT3 DEFAULT_POS = { 0.0f, 20.0f, -50.0f };
static const XMFLOAT3 DEFAULT_POS_AT = { 0.0f, 0.0f, 0.0f };
constexpr float CAM_SPEED = 10.0f * FRAME_VALUE;

ActorTrackingCamera::ActorTrackingCamera(ActorManager* pActorManager) : ActorCamera(pActorManager)
{
	name_ = ActorManager::NAME_TRACKING_CAMERA;
	tag_ = ActorManager::TAG_CAMERA;
	layer_ = ActorManager::LAYER_ETC;
}

ActorTrackingCamera::~ActorTrackingCamera()
{

}

bool ActorTrackingCamera::Init()
{
	nameUnique_ = "ActorTrackingCamera" + std::to_string(number_);

	pTrackingActor_ = dynamic_cast <Actor3d*> (pActorManager_->GetActor(ActorManager::NAME_PLAYER, 0));

	pos_ = DEFAULT_POS;
	posAt_ = DEFAULT_POS_AT;

	EditMath::Subtraction(vecFront_, posAt_, pos_);
	EditMath::Normalize(vecFront_, vecFront_);

	vecUp_ = XMFLOAT3(0.0f, 1.0f, 0.0f);
	EditMath::Normalize(vecUp_, vecUp_);

	EditMath::Cross(vecRight_, vecUp_, vecFront_);
	EditMath::Normalize(vecRight_, vecRight_);

	EditMath::LookAtLH(mtxView_, pos_, posAt_, vecUp_);

	EditMath::PerspectiveFovLH(
		mtxProjection_,
		XMConvertToRadians(CAMERA_FOV / 2.0f),
		(float)SCREEN_WIDTH / SCREEN_HEIGHT,
		CAMERA_NEAR,
		CAMERA_FAR);

	return true;
}

void ActorTrackingCamera::Uninit()
{

}

void ActorTrackingCamera::Update()
{
	posAt_ = pTrackingActor_->GetPos();

	EditMath::Subtraction(vecFront_, posAt_, pos_);
	EditMath::Normalize(vecFront_, vecFront_);

	EditMath::Cross(vecRight_, vecUp_, vecFront_);
	EditMath::Normalize(vecRight_, vecRight_);

	EditMath::Subtraction(vecCamAt_, posAt_, pos_);
	EditMath::Subtraction(vecAtCam_, pos_, posAt_);

	if (GetKeyboardPress(DIK_LSHIFT))
	{
		//前進
		if (GetKeyboardPress(DIK_W))
		{
			XMFLOAT3 vec = vecFront_;
			vec.y = 0.0f;
			EditMath::Normalize(vec, vec);
			EditMath::Multiplication(vec, vec, CAM_SPEED);
			EditMath::Addition(pos_, pos_, vec);
			EditMath::Addition(posAt_, posAt_, vec);
		}

		//後退
		if (GetKeyboardPress(DIK_S))
		{
			XMFLOAT3 vec = vecFront_;
			vec.y = 0.0f;
			EditMath::Normalize(vec, vec);
			EditMath::Multiplication(vec, vec, CAM_SPEED);
			EditMath::Subtraction(pos_, pos_, vec);
			EditMath::Subtraction(posAt_, posAt_, vec);
		}

		//右移動
		if (GetKeyboardPress(DIK_D))
		{
			XMFLOAT3 vec = vecRight_;
			vec.y = 0.0f;
			EditMath::Normalize(vec, vec);
			EditMath::Multiplication(vec, vec, CAM_SPEED);
			EditMath::Addition(pos_, pos_, vec);
			EditMath::Addition(posAt_, posAt_, vec);
		}

		//左移動
		if (GetKeyboardPress(DIK_A))
		{
			XMFLOAT3 vec = vecRight_;
			vec.y = 0.0f;
			EditMath::Normalize(vec, vec);
			EditMath::Multiplication(vec, vec, CAM_SPEED);
			EditMath::Subtraction(pos_, pos_, vec);
			EditMath::Subtraction(posAt_, posAt_, vec);
		}

		//上昇
		if (GetKeyboardPress(DIK_E))
		{
			XMFLOAT3 vec;
			EditMath::Multiplication(vec, vecUp_, CAM_SPEED);
			EditMath::Addition(pos_, pos_, vec);
			EditMath::Addition(posAt_, posAt_, vec);
		}

		//下降
		if (GetKeyboardPress(DIK_Q))
		{
			XMFLOAT3 vec;
			EditMath::Multiplication(vec, vecUp_, CAM_SPEED);
			EditMath::Subtraction(pos_, pos_, vec);
			EditMath::Subtraction(posAt_, posAt_, vec);
		}

		//カメラリセット
		if (GetKeyboardTrigger(DIK_R))
		{
			Init();
		}
	}

	EditMath::LookAtLH(mtxView_, pos_, posAt_, vecUp_);
}

void ActorTrackingCamera::Stats()
{
	ActorCamera::Stats();
}

void ActorTrackingCamera::Draw()
{

}

void ActorTrackingCamera::SetTrackingActor(Actor3d* pTrackingActor)
{
	pTrackingActor_ = pTrackingActor;
}