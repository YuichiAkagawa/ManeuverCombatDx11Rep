//**-------------------------------------------------------**
//**
//**                actor_camera.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <DirectXMath.h>
#include <typeinfo>
#include "actor.h"
#include "actor_3d.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "imgui/imgui.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "actor_camera.h"

using namespace DirectX;

static const XMFLOAT3 DEFAULT_POS = { 0.0f, 10.0f, -50.0f };
static const XMFLOAT3 DEFAULT_POS_AT = { 0.0f, 0.0f, 0.0f };

ActorCamera::ActorCamera(ActorManager* pActorManager) : Actor3d(pActorManager)
{
	name_ = ActorManager::NAME_CAMERA;
	tag_ = ActorManager::TAG_CAMERA;
	layer_ = ActorManager::LAYER_ETC;
}

ActorCamera::~ActorCamera()
{

}

bool ActorCamera::Init()
{
	nameUnique_ = "ActorCamera" + std::to_string(number_);

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

void ActorCamera::Uninit()
{

}

void ActorCamera::Update()
{
	mtxView_ = EditMath::LookAtLH(pos_, posAt_, vecUp_);

	mtxProjection_ = EditMath::PerspectiveFovLH(
		XMConvertToRadians(CAMERA_FOV / 2.0f),
		(float)SCREEN_WIDTH / SCREEN_HEIGHT,
		CAMERA_NEAR,
		CAMERA_FAR);

}

void ActorCamera::Stats()
{
	if (isDrawImgui_ && GetIsImgui())
	{
		ImGui::SetNextWindowPos(ImVec2(880, 20), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiSetCond_Once);

		ImGui::Begin(nameUnique_.c_str(), &isDrawImgui_);

		ImGui::Text("UpdateProcessTime : %.3f ms", updateProcessCalc_.GetProcessTimeMS());
		ImGui::Text("DrawProcessTime : %.3f ms", drawProcessCalc_.GetProcessTimeMS());

		bool processTimeClear = false;
		ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
		if (ImGui::TreeNode("ProcessTimeMax"))
		{
			ImGui::Text("UpdateProcessTimeMax : %.3f ms", updateProcessCalc_.GetProcessTimeMaxMS());
			ImGui::Text("DrawProcessTimeMax : %.3f ms", drawProcessCalc_.GetProcessTimeMaxMS());
			processTimeClear = ImGui::Button("Reset");
			ImGui::TreePop();
		}

		ImGui::Text("Position X:%.4f Y:%.4f Z:%.4f", pos_.x, pos_.y, pos_.z);
		ImGui::Text("FrontVector X:%.4f Y:%.4f Z:%.4f", vecFront_.x, vecFront_.y, vecFront_.z);
		ImGui::Text("RightVector X:%.4f Y:%.4f Z:%.4f", vecRight_.x, vecRight_.y, vecRight_.z);
		ImGui::Text("UpVector X:%.4f Y:%.4f Z:%.4f", vecUp_.x, vecUp_.y, vecUp_.z);

		//リセットボタンが押されたかどうか
		if (processTimeClear)
		{
			//処理時間計測値初期化
			updateProcessCalc_.Reset();
			drawProcessCalc_.Reset();
		}

		ImGui::End();
	}
}

void ActorCamera::Draw()
{

}

const XMFLOAT4X4& ActorCamera::GetMtxView()const
{
	return mtxView_;
}

const XMFLOAT4X4& ActorCamera::GetMtxProjection()const
{
	return mtxProjection_;
}