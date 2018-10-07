//**-------------------------------------------------------**
//**
//**               actor_hierarchy.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <DirectXMath.h>
#include "actor_3d.h"
#include "edit_math.h"
#include "imgui/imgui.h"
#include "imgui_manager.h"
#include "main.h"

#include "actor_hierarchy.h"

using namespace DirectX;

ActorHierarchy::ActorHierarchy(ActorManager* pActorManager) : Actor3d(pActorManager)
{

}

ActorHierarchy::~ActorHierarchy()
{

}

bool ActorHierarchy::Init()
{
	nameUnique_ = "ActorHierarchy" + std::to_string(number_);

	return true;
}

void ActorHierarchy::Uninit()
{

}

void ActorHierarchy::Update()
{

}

void ActorHierarchy::Stats()
{
	if (isDrawImgui_ && ImguiManager::GetIsDraw())
	{
		ImGui::SetNextWindowPos(ImVec2(880, 20), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiSetCond_Once);

		ImGui::Begin(nameUnique_.c_str(), &isDrawImgui_);
		ImGui::Checkbox("UpdateEnable", &isUpdate_);
		ImGui::Checkbox("DrawEnable", &isDraw_);
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

		ImGui::DragFloat("RotAxis", &rotAxis_, 0.005f);

		float vecRotAxisOffset[3] = { vecRotAxisOffset_.x, vecRotAxisOffset_.y, vecRotAxisOffset_.z };
		ImGui::DragFloat3("RotAxisOffset", vecRotAxisOffset, 0.1f);
		vecRotAxisOffset_.x = vecRotAxisOffset[0];
		vecRotAxisOffset_.y = vecRotAxisOffset[1];
		vecRotAxisOffset_.z = vecRotAxisOffset[2];

		float transOffset[3] = { transOffset_.x, transOffset_.y, transOffset_.z };
		ImGui::DragFloat3("TransOffset", transOffset, 0.01f);
		transOffset_.x = transOffset[0];
		transOffset_.y = transOffset[1];
		transOffset_.z = transOffset[2];

		float rotOffset[3] = { rotOffset_.x, rotOffset_.y, rotOffset_.z };
		ImGui::DragFloat3("RotOffset", rotOffset, 0.005f);
		rotOffset_.x = rotOffset[0];
		rotOffset_.y = rotOffset[1];
		rotOffset_.z = rotOffset[2];

		float scaleOffset[3] = { scaleOffset_.x, scaleOffset_.y, scaleOffset_.z };
		ImGui::DragFloat3("ScaleOffset", scaleOffset, 0.1f);
		scaleOffset_.x = scaleOffset[0];
		scaleOffset_.y = scaleOffset[1];
		scaleOffset_.z = scaleOffset[2];

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

void ActorHierarchy::Draw()
{

}

void ActorHierarchy::ReflectionMatrix()
{
	XMFLOAT4X4 mtxRotOffset;
	if (pActorParent_ != nullptr)
	{
		XMFLOAT4X4 mtxScaleOffset;
		EditMath::Scaling(mtxScaleOffset, scaleOffset_);

		XMFLOAT4X4 mtxRotOffsetX;
		EditMath::RotationX(mtxRotOffsetX, rotOffset_.x);

		XMFLOAT4X4 mtxRotOffsetY;
		EditMath::RotationY(mtxRotOffsetY, rotOffset_.y);

		XMFLOAT4X4 mtxRotOffsetZ;
		EditMath::RotationZ(mtxRotOffsetZ, rotOffset_.z);

		EditMath::Normalize(vecRotAxisOffset_, vecRotAxisOffset_);
		XMFLOAT4X4 mtxRotAxisOffset;
		EditMath::RotationAxis(mtxRotAxisOffset, vecRotAxisOffset_, rotAxis_);

		EditMath::Multiplication(mtxRotOffset, mtxRotOffsetX, mtxRotOffsetY);
		EditMath::Multiplication(mtxRotOffset, mtxRotOffset, mtxRotOffsetZ);
		EditMath::Multiplication(mtxRotOffset, mtxRotOffset, mtxRotAxisOffset);

		XMFLOAT4X4 mtxTransOffset;
		EditMath::Translation(mtxTransOffset, transOffset_);

		EditMath::Multiplication(mtxWorld_, mtxScaleOffset, mtxRotOffset);
		EditMath::Multiplication(mtxWorld_, mtxWorld_, mtxTransOffset);
		EditMath::Multiplication(mtxWorld_, mtxWorld_, pActorParent_->GetMtxWorld());
	}
	else
	{
		XMFLOAT4X4 mtxScaleOffset;
		EditMath::Scaling(mtxScaleOffset, scaleOffset_);

		XMFLOAT4X4 mtxRotOffsetX;
		EditMath::RotationX(mtxRotOffsetX, rotOffset_.x);

		XMFLOAT4X4 mtxRotOffsetY;
		EditMath::RotationY(mtxRotOffsetY, rotOffset_.y);

		XMFLOAT4X4 mtxRotOffsetZ;
		EditMath::RotationZ(mtxRotOffsetZ, rotOffset_.z);

		EditMath::Normalize(vecRotAxisOffset_, vecRotAxisOffset_);
		XMFLOAT4X4 mtxRotAxisOffset;
		EditMath::RotationAxis(mtxRotAxisOffset, vecRotAxisOffset_, rotAxis_);

		EditMath::Multiplication(mtxRotOffset, mtxRotOffsetX, mtxRotOffsetY);
		EditMath::Multiplication(mtxRotOffset, mtxRotOffset, mtxRotOffsetZ);
		EditMath::Multiplication(mtxRotOffset, mtxRotOffset, mtxRotAxisOffset);

		XMFLOAT4X4 mtxTransOffset;
		EditMath::Translation(mtxTransOffset, transOffset_);

		EditMath::Multiplication(mtxWorld_, mtxScaleOffset, mtxRotOffset);
		EditMath::Multiplication(mtxWorld_, mtxWorld_, mtxTransOffset);
	}

	EditMath::Transform(vecFront_, vecFront_, mtxRotOffset);
	EditMath::Transform(vecRight_, vecRight_, mtxRotOffset);
	EditMath::Transform(vecUp_, vecUp_, mtxRotOffset);

	pos_.x = mtxWorld_._41;
	pos_.y = mtxWorld_._42;
	pos_.z = mtxWorld_._43;
}

const XMFLOAT3& ActorHierarchy::GetTransOffset()const
{
	return transOffset_;
}

const XMFLOAT3& ActorHierarchy::GetRotOffset()const
{
	return rotOffset_;
}

const XMFLOAT3& ActorHierarchy::GetScaleOffset()const
{
	return scaleOffset_;
}

const XMFLOAT3& ActorHierarchy::GetVecRotAxisOffset()const
{
	return vecRotAxisOffset_;
}

float ActorHierarchy::GetRotAxis()
{
	return rotAxis_;
}

void ActorHierarchy::SetParent(Actor3d* pActorParent)
{
	pActorParent_ = pActorParent;
}

void ActorHierarchy::SetTransOffset(const XMFLOAT3& transOffset)
{
	transOffset_ = transOffset;
}

void ActorHierarchy::SetRotOffset(const XMFLOAT3& rotOffset)
{
	rotOffset_ = rotOffset;
}

void ActorHierarchy::SetScaleOffset(const XMFLOAT3& scaleOffset)
{
	scaleOffset_ = scaleOffset;
}

void ActorHierarchy::SetVecRotAxisOffset(const XMFLOAT3& vecRotAxisOffset)
{
	vecRotAxisOffset_ = vecRotAxisOffset;
}

void ActorHierarchy::SetRotAxis(float rotAxis)
{
	rotAxis_ = rotAxis;
}