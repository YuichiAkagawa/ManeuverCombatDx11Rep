//**-------------------------------------------------------**
//**
//**                     actor3d.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <DirectXMath.h>
#include "actor.h"
#include "edit_math.h"
#include "imgui/imgui.h"
#include "imgui_manager.h"
#include "main.h"
#include "actor_3d.h"

using namespace DirectX;

Actor3d::Actor3d(ActorManager* pActorManager) : Actor(pActorManager)
{
	EditMath::Identity(mtxWorld_);
}

Actor3d::~Actor3d()
{

}

bool Actor3d::Init()
{
	nameUnique_ = "Actor3d" + std::to_string(number_);
	return true;
}

void Actor3d::Uninit()
{

}

void Actor3d::Update()
{

}

void Actor3d::Stats()
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

		float pos[3] = { pos_.x, pos_.y, pos_.z };
		ImGui::DragFloat3("Position", pos, 1.0f);
		mtxWorld_._41 = pos_.x = pos[0];
		mtxWorld_._42 = pos_.y = pos[1];
		mtxWorld_._43 = pos_.z = pos[2];

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

void Actor3d::Draw()
{

}

const XMFLOAT3& Actor3d::GetPos()const
{
	return pos_;
}

const XMFLOAT3& Actor3d::GetVecFront()const
{
	return vecFront_;
}

const XMFLOAT3& Actor3d::GetVecRight()const
{
	return vecRight_;
}

const XMFLOAT3& Actor3d::GetVecUp()const
{
	return vecUp_;
}

const XMFLOAT4X4& Actor3d::GetMtxWorld()const
{
	return mtxWorld_;
}

void Actor3d::SetPos(const XMFLOAT3& pos)
{
	pos_ = pos;
	mtxWorld_._41 = pos_.x;
	mtxWorld_._42 = pos_.y;
	mtxWorld_._43 = pos_.z;
}

void Actor3d::SetVecFront(const XMFLOAT3& vecFront)
{
	vecFront_ = vecFront;
}

void Actor3d::SetVecRight(const XMFLOAT3& vecRight)
{
	vecRight_ = vecRight;
}

void Actor3d::SetVecUp(const XMFLOAT3& vecUp)
{
	vecUp_ = vecUp;
}

void Actor3d::SetMtxWorld(const XMFLOAT4X4& mtxWorld)
{
	mtxWorld_ = mtxWorld;
	pos_.x = mtxWorld_._41;
	pos_.y = mtxWorld_._42;
	pos_.z = mtxWorld_._43;
}