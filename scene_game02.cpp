//**-------------------------------------------------------**
//**
//**                     scene_game02.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_manager.h"
#include "imgui/imgui.h"
#include "input.h"
#include "main.h"
#include "scene.h"
#include "scene_game01.h"
#include "scene_manager.h"

#include "scene_game02.h"

bool SceneGame02::Init()
{
	//ŠÔ‰Šú‰»
	startTime_ = timeGetTime();

	if (!actorManager_.Init())
	{
		return false;
	}

	return true;
}

void SceneGame02::Uninit()
{
	actorManager_.Uninit();

}

void SceneGame02::Update()
{
	//ImGui
	ImGui::Begin("Stats");
	ImGui::Text("Scene : Game02");
	ImGui::End();

	actorManager_.Update();

	//ŠÔXV
	time_ = (float(timeGetTime() - startTime_)) / 1000.0f;

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SceneManager::SetScene(new SceneGame01);
	}
}

void SceneGame02::Draw()
{
	actorManager_.Draw();
}
