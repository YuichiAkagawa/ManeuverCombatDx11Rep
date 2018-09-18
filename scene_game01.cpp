//**-------------------------------------------------------**
//**
//**                     scene_game01.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_manager.h"
#include "cube.h"
#include "imgui/imgui.h"
#include "input.h"
#include "main.h"
#include "scene.h"
#include "scene_game02.h"
#include "scene_manager.h"
#include "texture.h"

#include "scene_game01.h"

bool SceneGame01::Init()
{
	//テクスチャ読み込み
	pTextureManager_ = new TextureManager;
	if (!pTextureManager_->Init())
	{
		return false;
	}

	//テクスチャマネージャセット
	actorManager_.SetTextureManager(pTextureManager_);

	//キューブ生成
	pCube_ = new Cube;
	if (!pCube_->Init())
	{
		return false;
	}

	//時間初期化
	startTime_ = timeGetTime();

	if (!actorManager_.Init())
	{
		return false;
	}

	return true;
}

void SceneGame01::Uninit()
{
	pCube_->Uninit();
	actorManager_.Uninit();
	pTextureManager_->Uninit();
	SafeDelete(pTextureManager_);
}

void SceneGame01::Update()
{
	//ImGui
	ImGui::Begin("Stats");
	ImGui::Text("Scene : Game01");
	ImGui::End();

	actorManager_.Update();

	//時間更新
	time_ = (float(timeGetTime() - startTime_)) / 1000.0f;

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SceneManager::SetScene(new SceneGame02);
	}
}

void SceneGame01::Draw()
{
	pCube_->Draw(pTextureManager_);
	actorManager_.Draw();
}
