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
#include "texture.h"
#include "actor_free_camera.h"

using namespace DirectX;

static const XMFLOAT3 DEFAULT_POS = { 0.0f, 10.0f, -50.0f };
static const XMFLOAT3 DEFAULT_POS_AT = { 0.0f, 0.0f, 0.0f };
constexpr float CAM_SPEED = 0.5f;
constexpr float CAM_ROT_SPEED = 1.0f;
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

	vecFront_ = EditMath::Sub(posAt_, pos_);
	vecFront_ = EditMath::Normalize(vecFront_);

	vecUp_ = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vecUp_ = EditMath::Normalize(vecUp_);

	vecRight_ = EditMath::Cross(vecUp_, vecFront_);
	vecRight_ = EditMath::Normalize(vecRight_);

	mtxView_ = EditMath::LookAtLH(pos_, posAt_, vecUp_);

	mtxProjection_ = EditMath::PerspectiveFovLH(
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
	vecFront_ = EditMath::Sub(posAt_, pos_);
	vecFront_ = EditMath::Normalize(vecFront_);

	vecRight_ = EditMath::Cross(vecUp_, vecFront_);
	vecRight_ = EditMath::Normalize(vecRight_);

	vecCamAt_ = EditMath::Sub(posAt_, pos_);
	vecAtCam_ = EditMath::Sub(pos_, posAt_);

	//�O�i
	if (GetKeyboardPress(DIK_W))
	{
		XMFLOAT3 vec = vecFront_;
		vec.y = 0.0f;
		vec = EditMath::Normalize(vec);
		vec = EditMath::Mul(vec, CAM_SPEED);
		pos_ = EditMath::Add(pos_, vec);
		posAt_ = EditMath::Add(posAt_, vec);
	}

	//���
	if (GetKeyboardPress(DIK_S))
	{
		XMFLOAT3 vec = vecFront_;
		vec.y = 0.0f;
		vec = EditMath::Normalize(vec);
		vec = EditMath::Mul(vec, CAM_SPEED);
		pos_ = EditMath::Sub(pos_, vec);
		posAt_ = EditMath::Sub(posAt_, vec);
	}

	//�E�ړ�
	if (GetKeyboardPress(DIK_D))
	{
		XMFLOAT3 vec = vecRight_;
		vec.y = 0.0f;
		vec = EditMath::Normalize(vec);
		vec = EditMath::Mul(vec, CAM_SPEED);
		pos_ = EditMath::Add(pos_, vec);
		posAt_ = EditMath::Add(posAt_, vec);
	}

	//���ړ�
	if (GetKeyboardPress(DIK_A))
	{
		XMFLOAT3 vec = vecRight_;
		vec.y = 0.0f;
		vec = EditMath::Normalize(vec);
		vec = EditMath::Mul(vec, CAM_SPEED);
		pos_ = EditMath::Sub(pos_, vec);
		posAt_ = EditMath::Sub(posAt_, vec);
	}

	//�㏸
	if (GetKeyboardPress(DIK_E))
	{
		XMFLOAT3 vec = EditMath::Mul(vecUp_, CAM_SPEED);
		pos_ = EditMath::Add(pos_, vec);
		posAt_ = EditMath::Add(posAt_, vec);
	}

	//���~
	if (GetKeyboardPress(DIK_Q))
	{
		XMFLOAT3 vec = EditMath::Mul(vecUp_, CAM_SPEED);
		pos_ = EditMath::Sub(pos_, vec);
		posAt_ = EditMath::Sub(posAt_, vec);
	}

	//���_�E�ړ�
	if (GetKeyboardPress(DIK_RIGHT) && !GetKeyboardPress(DIK_LSHIFT))
	{
		XMFLOAT4X4 mtxRotY = EditMath::RotationY(XMConvertToRadians(CAM_ROT_SPEED));

		vecCamAt_ = EditMath::Transform(vecCamAt_, mtxRotY);
		vecFront_ = EditMath::Transform(vecFront_, mtxRotY);
		vecRight_ = EditMath::Transform(vecRight_, mtxRotY);

		posAt_ = EditMath::Add(pos_, vecCamAt_);
	}

	//���_���ړ�
	if (GetKeyboardPress(DIK_LEFT) && !GetKeyboardPress(DIK_LSHIFT))
	{
		XMFLOAT4X4 mtxRotY = EditMath::RotationY(-XMConvertToRadians(CAM_ROT_SPEED));

		vecCamAt_ = EditMath::Transform(vecCamAt_, mtxRotY);
		vecFront_ = EditMath::Transform(vecFront_, mtxRotY);
		vecRight_ = EditMath::Transform(vecRight_, mtxRotY);

		posAt_ = EditMath::Add(pos_, vecCamAt_);
	}

	//���_��ړ�
	if (GetKeyboardPress(DIK_UP))
	{
		XMFLOAT4X4 mtxRot = EditMath::RotationAxis(vecRight_, -XMConvertToRadians(CAM_ROT_SPEED));
		XMFLOAT3 vecDir = vecFront_;
		vecDir.y = 0.0f;
		vecDir = EditMath::Normalize(vecDir);

		XMFLOAT3 vecFrontCheck;
		vecFrontCheck = EditMath::Transform(vecFront_, mtxRot);

		float check = EditMath::Dot(vecFrontCheck, vecDir);
		if (check >= CAM_LIMIT)
		{
			vecFront_ = EditMath::Transform(vecFront_, mtxRot);
			vecCamAt_ = EditMath::Transform(vecCamAt_, mtxRot);
			vecRight_ = EditMath::Transform(vecRight_, mtxRot);

			posAt_ = EditMath::Add(pos_, vecCamAt_);
		}
	}

	//���_���ړ�
	if (GetKeyboardPress(DIK_DOWN))
	{
		XMFLOAT4X4 mtxRot = EditMath::RotationAxis(vecRight_, XMConvertToRadians(CAM_ROT_SPEED));
		XMFLOAT3 vecDir = vecFront_;
		vecDir.y = 0.0f;
		vecDir = EditMath::Normalize(vecDir);

		XMFLOAT3 vecFrontCheck;
		vecFrontCheck = EditMath::Transform(vecFront_, mtxRot);

		float check = EditMath::Dot(vecFrontCheck, vecDir);
		if (check >= CAM_LIMIT)
		{
			vecFront_ = EditMath::Transform(vecFront_, mtxRot);
			vecCamAt_ = EditMath::Transform(vecCamAt_, mtxRot);
			vecRight_ = EditMath::Transform(vecRight_, mtxRot);

			posAt_ = EditMath::Add(pos_, vecCamAt_);
		}
	}

	//�J�������Z�b�g
	if (GetKeyboardTrigger(DIK_R))
	{
		Init();
	}

	mtxView_ = EditMath::LookAtLH(pos_, posAt_, vecUp_);
}

void ActorFreeCamera::Stats()
{
	ActorCamera::Stats();
}

void ActorFreeCamera::Draw()
{

}