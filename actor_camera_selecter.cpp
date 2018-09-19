//**-------------------------------------------------------**
//**
//**              actor_camera_selecter.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_camera.h"
#include "actor_free_camera.h"
#include "actor_manager.h"
#include "imgui/imgui.h"
#include "main.h"
#include "actor_camera_selecter.h"

constexpr int DEFAULT_TYPE = ActorCameraSelecter::TYPE_FREE_CAMERA_01;

ActorCameraSelecter::ActorCameraSelecter(ActorManager* pActorManager) : Actor(pActorManager)
{
	name_ = ActorManager::NAME_CAMERA_SELECTER;
	tag_ = ActorManager::TAG_CAMERA_SELECTER;
	layer_ = ActorManager::LAYER_ETC;

	//カメラ生成
	ActorFreeCamera* pActorFreeCamera01 = new ActorFreeCamera(pActorManager_);
	pActorManager_->CreateActor(pActorFreeCamera01);
	cameraArray_[TYPE_FREE_CAMERA_01] = pActorFreeCamera01;

	ActorFreeCamera* pActorFreeCamera02 = new ActorFreeCamera(pActorManager_);
	pActorManager_->CreateActor(pActorFreeCamera02);
	cameraArray_[TYPE_FREE_CAMERA_02] = pActorFreeCamera02;
}

ActorCameraSelecter::~ActorCameraSelecter()
{

}

bool ActorCameraSelecter::Init()
{
	nameUnique_ = "ActorCameraSelecter" + std::to_string(number_);

	selectType_ = DEFAULT_TYPE;

	for (int i = 0; i < TYPE_MAX; i++)
	{
		if (i == selectType_)
		{
			cameraArray_[i]->SetIsUpdate(true);
		}
		else
		{
			cameraArray_[i]->SetIsUpdate(false);
		}
	}

	return true;
}

void ActorCameraSelecter::Uninit()
{

}

void ActorCameraSelecter::Update()
{
	//選択中のカメラのみ有効
	for (int i = 0; i < TYPE_MAX; i++)
	{
		if (i == selectType_)
		{
			cameraArray_[i]->SetIsUpdate(true);
		}
		else
		{
			cameraArray_[i]->SetIsUpdate(false);
		}
	}
}

void ActorCameraSelecter::Stats()
{
	if (isDrawImgui_ && GetIsImgui())
	{
		ImGui::SetNextWindowPos(ImVec2(880, 20), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiSetCond_Once);

		ImGui::Begin(nameUnique_.c_str(), &isDrawImgui_);
		
		if (ImGui::Button("ChangeCamera"))
		{
			SelectNextType();
		}

		ImGui::Checkbox("UpdateEnable", &isUpdate_);
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

void ActorCameraSelecter::Draw()
{

}

void ActorCameraSelecter::SelectType(int type)
{
	selectType_ = type;
}

void ActorCameraSelecter::SelectNextType()
{
	selectType_++;
	if (selectType_ >= TYPE_MAX)
	{
		selectType_ = 0;
	}
}

ActorCamera* ActorCameraSelecter::GetSelectCamera()
{
	return cameraArray_[selectType_];
}