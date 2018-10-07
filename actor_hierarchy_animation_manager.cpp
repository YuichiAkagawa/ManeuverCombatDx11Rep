//**-------------------------------------------------------**
//**
//**        actor_hierarchy_animation_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <fstream>
#include "actor_manager.h"
#include "actor_hierarchy_animator.h"
#include "imgui/imgui.h"
#include "imgui_manager.h"
#include "main.h"
#include "actor_hierarchy_animation_manager.h"

ActorHierarchyAnimationManager::ActorHierarchyAnimationManager(ActorManager* pActorManager) : Actor(pActorManager)
{
	name_ = ActorManager::NAME_HIERARCHY_ANIMATION_MANAGER;
	tag_ = ActorManager::TAG_HIERARCHY_ANIMATION_MANAGER;
	layer_ = ActorManager::LAYER_ETC;
}

ActorHierarchyAnimationManager::~ActorHierarchyAnimationManager()
{

}

bool ActorHierarchyAnimationManager::Init()
{
	nameUnique_ = "ActorHierarchyAnimationManager" + std::to_string(number_);

	//std::vector<KEYFRAME> keyFrame0;
	//{
	//	KEYFRAME kf = { 0, 0.0f };
	//	keyFrame0.emplace_back(kf);
	//}
	//{
	//	KEYFRAME kf = { 120, 0.2f };
	//	keyFrame0.emplace_back(kf);
	//}
	//{
	//	KEYFRAME kf = { 180, 0.2f };
	//	keyFrame0.emplace_back(kf);
	//}
	//{
	//	KEYFRAME kf = { 420, -0.2f };
	//	keyFrame0.emplace_back(kf);
	//}
	//{
	//	KEYFRAME kf = { 480, -0.2f };
	//	keyFrame0.emplace_back(kf);
	//}
	//{
	//	KEYFRAME kf = { 600, 0.0f };
	//	keyFrame0.emplace_back(kf);
	//}
	//{
	//	KEYFRAME kf = { 660, 0.0f };
	//	keyFrame0.emplace_back(kf);
	//}

	//{
	//	std::ofstream fout;
	//	fout.open("data/anim/body.anim", std::ios::out | std::ios::binary | std::ios::trunc);
	//	if (!fout)
	//	{
	//		return false;
	//	}

	//	for (auto itr = keyFrame0.begin(); itr != keyFrame0.end(); ++itr)
	//	{
	//		fout.write((const char *)&(*itr).frame, sizeof(int));
	//		fout.write((const char *)&(*itr).rotAxis, sizeof(float));
	//	}

	//	fout.close();
	//}

	//アニメーションファイルロード
	if (!Load())
	{
		return false;
	}

	//アニメーションリストをアニメーターに渡す
	for (auto itr = animatorList_.begin(); itr != animatorList_.end(); ++itr)
	{
		(*itr)->SetAnimationList(animationList_);
	}

	//最大フレーム数算出
	for (auto itr = animationList_.begin(); itr != animationList_.end(); ++itr)
	{
		for (auto itr2 = itr->begin(); itr2 != itr->end(); ++itr2)
		{
			maxFrame_ = max(maxFrame_, (*itr2).frame);
		}
	}

	return true;
}

void ActorHierarchyAnimationManager::Uninit()
{
	
}

void ActorHierarchyAnimationManager::Update()
{
	if (isPlay)
	{
		if (!isReverse_)
		{
			//通常再生
			numFrame_++;
			if (maxFrame_< numFrame_)
			{
				numFrame_ = 0;
				if (!isRepeat_)
				{
					isPlay = false;
					numFrame_ = maxFrame_;
				}
			}
		}
		else
		{
			//逆再生
			numFrame_--;
			if (0 > numFrame_)
			{
				numFrame_ = maxFrame_;
				if (!isRepeat_)
				{
					isPlay = false;
					numFrame_ = 0;
				}
			}
		}
	}

	//現在のフレームセット
	for (auto itr = animatorList_.begin(); itr != animatorList_.end(); ++itr)
	{
		(*itr)->SetNumFrame(numFrame_);
	}
}

void ActorHierarchyAnimationManager::Stats()
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

		//リセットボタンが押されたかどうか
		if (processTimeClear)
		{
			//処理時間計測値初期化
			updateProcessCalc_.Reset();
			drawProcessCalc_.Reset();
		}

		ImGui::Text("CurrentFrame : %d", numFrame_);
		ImGui::Text("EndFrame : %d", maxFrame_);

		if (ImGui::Button("PlayOutset"))
		{
			PlayOutset();
		}
		if (ImGui::Button("PlayOnTheWay"))
		{
			PlayOnTheWay();
		}
		if (ImGui::Button("Pause"))
		{
			Pause();
		}
		if (ImGui::Button("Stop"))
		{
			Stop();
		}

		ImGui::Checkbox("Reverse", &isReverse_);
		ImGui::Checkbox("Repeat", &isRepeat_);

		ImGui::End();
	}
}

void ActorHierarchyAnimationManager::Draw()
{

}

bool ActorHierarchyAnimationManager::Load()
{
	for (int i = 0; i < ANIMATION_MAX; i++)
	{
		std::vector<KEYFRAME> kfList;
		{
			std::ifstream fin(g_pAnimFile[i], std::ios::in | std::ios::binary);
			if (!fin)
			{
				return false;
			}

			while (!fin.eof())
			{
				KEYFRAME kf;
				fin.read((char *)&kf.frame, sizeof(int));
				fin.read((char *)&kf.rotAxis, sizeof(float));
				kfList.emplace_back(kf);
			}

			fin.close();
		}

		animationList_.emplace_back(kfList);
	}

	return true;
}

bool ActorHierarchyAnimationManager::Save()
{
	return true;
}

void ActorHierarchyAnimationManager::PlayOutset()
{
	isPlay = true;
	isRepeat_ = false;
	isReverse_ = false;
	numFrame_ = 0;
}

void ActorHierarchyAnimationManager::PlayOutset(bool isRepeat, bool isReverse)
{
	isPlay = true;
	isRepeat_ = isRepeat;
	isReverse_ = isReverse;

	if (!isReverse_)
	{
		//通常再生
		numFrame_ = 0;
	}
	else
	{
		//逆再生
		numFrame_ = maxFrame_;
	}
}

void ActorHierarchyAnimationManager::PlayOnTheWay()
{
	isPlay = true;
}

void ActorHierarchyAnimationManager::PlayOnTheWay(bool isRepeat, bool isReverse)
{
	isPlay = true;
	isRepeat_ = isRepeat;
	isReverse_ = isReverse;
}

void ActorHierarchyAnimationManager::PlayOnTheWay(bool isRepeat, bool isReverse, int numFrame)
{
	isPlay = true;
	isRepeat_ = isRepeat;
	isReverse_ = isReverse;
	numFrame_ = numFrame;
}

void ActorHierarchyAnimationManager::Pause()
{
	isPlay = false;
}

void ActorHierarchyAnimationManager::Stop()
{
	isPlay = false;
	isRepeat_ = false;
	isReverse_ = false;
	numFrame_ = 0;
}

void ActorHierarchyAnimationManager::SetAnimator(ActorHierarchyAnimator* pAnimator)
{
	animatorList_.emplace_back(pAnimator);
}

