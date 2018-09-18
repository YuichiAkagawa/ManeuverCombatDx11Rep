//**-------------------------------------------------------**
//**
//**                     scene_game01.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_manager.h"
#include "imgui/imgui.h"
#include "input.h"
#include "main.h"
#include "scene.h"
#include "scene_game02.h"
#include "scene_manager.h"

#include "scene_game01.h"

bool SceneGame01::Init()
{
	//ŠÔ‰Šú‰»
	startTime_ = timeGetTime();

	if (!actorManager_.Init())
	{
		return false;
	}

	return true;
}

void SceneGame01::Uninit()
{
	actorManager_.Uninit();

}

void SceneGame01::Update()
{
	//ImGui
	ImGui::Begin("Stats");
	ImGui::Text("Scene : Game01");
	ImGui::End();

	actorManager_.Update();

	//ŠÔXV
	time_ = (float(timeGetTime() - startTime_)) / 1000.0f;

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SceneManager::SetScene(new SceneGame02);
	}
}

void SceneGame01::Draw()
{
	actorManager_.Draw();
}
