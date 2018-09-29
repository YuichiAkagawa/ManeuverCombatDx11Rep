//**-------------------------------------------------------**
//**
//**             actor_free_camera.cpp
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
#include "actor_free_camera.h"

using namespace DirectX;

static const XMFLOAT3 DEFAULT_POS = { 0.0f, 20.0f, -50.0f };
static const XMFLOAT3 DEFAULT_POS_AT = { 0.0f, 0.0f, 0.0f };
constexpr float CAM_SPEED = 0.1f;
constexpr float CAM_ROT_SPEED = 0.1f;
constexpr float CAM_LIMIT = 0.2f;

ActorFreeCamera::ActorFreeCamera(ActorManager* pActorManager) : ActorCamera(pActorManager)
{
	name_ = ActorManager::NAME_FREE_CAMERA;
	tag_ = ActorManager::TAG_CAMERA;
	layer_ = ActorManager::LAYER_ETC;
}

ActorFreeCamera::~ActorFreeCamera()
{

}

bool ActorFreeCamera::Init()
{
	nameUnique_ = "ActorFreeCamera" + std::to_string(number_);

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

void ActorFreeCamera::Uninit()
{

}

void ActorFreeCamera::Update()
{
	float processMS = SceneManager::GetProcessMS();

	EditMath::Subtraction(vecFront_, posAt_, pos_);
	EditMath::Normalize(vecFront_, vecFront_);

	EditMath::Cross(vecRight_, vecUp_, vecFront_);
	EditMath::Normalize(vecRight_, vecRight_);

	EditMath::Subtraction(vecCamAt_, posAt_, pos_);
	EditMath::Subtraction(vecAtCam_, pos_, posAt_);

	//前進
	if (GetKeyboardPress(DIK_W))
	{
		XMFLOAT3 vec = vecFront_;
		vec.y = 0.0f;
		EditMath::Normalize(vec, vec);
		EditMath::Multiplication(vec, vec, CAM_SPEED * processMS);
		EditMath::Addition(pos_, pos_, vec);
		EditMath::Addition(posAt_, posAt_, vec);
	}

	//後退
	if (GetKeyboardPress(DIK_S))
	{
		XMFLOAT3 vec = vecFront_;
		vec.y = 0.0f;
		EditMath::Normalize(vec, vec);
		EditMath::Multiplication(vec, vec, CAM_SPEED * processMS);
		EditMath::Subtraction(pos_, pos_, vec);
		EditMath::Subtraction(posAt_, posAt_, vec);
	}

	//右移動
	if (GetKeyboardPress(DIK_D))
	{
		XMFLOAT3 vec = vecRight_;
		vec.y = 0.0f;
		EditMath::Normalize(vec, vec);
		EditMath::Multiplication(vec, vec, CAM_SPEED * processMS);
		EditMath::Addition(pos_, pos_, vec);
		EditMath::Addition(posAt_, posAt_, vec);
	}

	//左移動
	if (GetKeyboardPress(DIK_A))
	{
		XMFLOAT3 vec = vecRight_;
		vec.y = 0.0f;
		EditMath::Normalize(vec, vec);
		EditMath::Multiplication(vec, vec, CAM_SPEED * processMS);
		EditMath::Subtraction(pos_, pos_, vec);
		EditMath::Subtraction(posAt_, posAt_, vec);
	}

	//上昇
	if (GetKeyboardPress(DIK_E))
	{
		XMFLOAT3 vec;
		EditMath::Multiplication(vec, vecUp_, CAM_SPEED * processMS);
		EditMath::Addition(pos_, pos_, vec);
		EditMath::Addition(posAt_, posAt_, vec);
	}

	//下降
	if (GetKeyboardPress(DIK_Q))
	{
		XMFLOAT3 vec;
		EditMath::Multiplication(vec, vecUp_, CAM_SPEED * processMS);
		EditMath::Subtraction(pos_, pos_, vec);
		EditMath::Subtraction(posAt_, posAt_, vec);
	}

	//視点右移動
	if (GetKeyboardPress(DIK_RIGHT) && !GetKeyboardPress(DIK_LSHIFT))
	{
		XMFLOAT4X4 mtxRotY;
		EditMath::RotationY(mtxRotY, XMConvertToRadians(CAM_ROT_SPEED * processMS));

		EditMath::Transform(vecCamAt_, vecCamAt_, mtxRotY);
		EditMath::Transform(vecFront_, vecFront_, mtxRotY);
		EditMath::Transform(vecRight_, vecRight_, mtxRotY);

		EditMath::Addition(posAt_, pos_, vecCamAt_);
	}

	//視点左移動
	if (GetKeyboardPress(DIK_LEFT) && !GetKeyboardPress(DIK_LSHIFT))
	{
		XMFLOAT4X4 mtxRotY;
		EditMath::RotationY(mtxRotY, -XMConvertToRadians(CAM_ROT_SPEED * processMS));

		EditMath::Transform(vecCamAt_, vecCamAt_, mtxRotY);
		EditMath::Transform(vecFront_, vecFront_, mtxRotY);
		EditMath::Transform(vecRight_, vecRight_, mtxRotY);

		EditMath::Addition(posAt_, pos_, vecCamAt_);
	}

	//視点上移動
	if (GetKeyboardPress(DIK_UP))
	{
		XMFLOAT4X4 mtxRot;
		EditMath::RotationAxis(mtxRot, vecRight_, -XMConvertToRadians(CAM_ROT_SPEED * processMS));
		XMFLOAT3 vecDir = vecFront_;
		vecDir.y = 0.0f;
		EditMath::Normalize(vecDir, vecDir);

		XMFLOAT3 vecFrontCheck;
		EditMath::Transform(vecFrontCheck, vecFront_, mtxRot);

		float check = EditMath::Dot(vecFrontCheck, vecDir);
		if (check >= CAM_LIMIT)
		{
			EditMath::Transform(vecFront_, vecFront_, mtxRot);
			EditMath::Transform(vecCamAt_, vecCamAt_, mtxRot);
			EditMath::Transform(vecRight_, vecRight_, mtxRot);

			EditMath::Addition(posAt_, pos_, vecCamAt_);
		}
	}

	//視点下移動
	if (GetKeyboardPress(DIK_DOWN))
	{
		XMFLOAT4X4 mtxRot;
		EditMath::RotationAxis(mtxRot, vecRight_, XMConvertToRadians(CAM_ROT_SPEED * processMS));
		XMFLOAT3 vecDir = vecFront_;
		vecDir.y = 0.0f;
		EditMath::Normalize(vecDir, vecDir);

		XMFLOAT3 vecFrontCheck;
		EditMath::Transform(vecFrontCheck, vecFront_, mtxRot);

		float check = EditMath::Dot(vecFrontCheck, vecDir);
		if (check >= CAM_LIMIT)
		{
			EditMath::Transform(vecFront_, vecFront_, mtxRot);
			EditMath::Transform(vecCamAt_, vecCamAt_, mtxRot);
			EditMath::Transform(vecRight_, vecRight_, mtxRot);

			EditMath::Addition(posAt_, pos_, vecCamAt_);
		}
	}

	//カメラリセット
	if (GetKeyboardTrigger(DIK_R))
	{
		Init();
	}

	EditMath::LookAtLH(mtxView_, pos_, posAt_, vecUp_);
}

void ActorFreeCamera::Stats()
{
	ActorCamera::Stats();
}

void ActorFreeCamera::Draw()
{

}