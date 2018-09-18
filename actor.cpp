//**-------------------------------------------------------**
//**
//**                     actor.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <string>
#include <typeinfo>
#include "imgui/imgui.h"
#include "main.h"
#include "process_calculator.h"
#include "actor.h"

Actor::Actor(ActorManager* pActorManager)
{
	pActorManager_ = pActorManager;
}

Actor::~Actor()
{

}

void Actor::Stats()
{
	if (isDrawImgui_)
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

int Actor::GetName()
{
	return name_;
}

const std::string& Actor::GetNameUnique()const
{
	return nameUnique_;
}

const std::type_info* Actor::GetTypeInfo()const
{
	return typeInfo_;
}

int Actor::GetNumber()
{
	return number_;
}

int Actor::GetLayer()
{
	return layer_;
}

bool Actor::GetIsUpdate()
{
	return isUpdate_;
}

bool Actor::GetIsDraw()
{
	return isDraw_;
}

bool Actor::GetIsDrawImgui()
{
	return isDrawImgui_;
}

ProcessCalculator& Actor::GetUpdateProcessCalc()
{
	return updateProcessCalc_;
}

ProcessCalculator& Actor::GetDrawProcessCalc()
{
	return drawProcessCalc_;
}

void Actor::SetName(int name)
{
	name_ = name;
}

void Actor::SetNameUnique(const std::string& nameUnique)
{
	nameUnique_ = nameUnique;
}

void Actor::SetTypeInfo(const std::type_info* typeInfo)
{
	typeInfo_ = typeInfo;
}

void Actor::SetNumber(int number)
{
	number_ = number;
}

void Actor::SetLayer(int layer)
{
	layer_ = layer;
}

void Actor::SetIsUpdate(bool isUpdate)
{
	isUpdate_ = isUpdate;
}

void Actor::SetIsDraw(bool isDraw)
{
	isDraw_ = isDraw;
}

void Actor::SetIsDrawImgui(bool isDrawImgui)
{
	isDrawImgui_ = isDrawImgui;
}
