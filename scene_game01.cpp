//**-------------------------------------------------------**
//**
//**                     scene_game01.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_camera_selecter.h"
#include "actor_cube.h"
#include "actor_fbx_model.h"
#include "actor_manager.h"
#include "imgui/imgui.h"
#include "input.h"
#include "main.h"
#include "model.h"
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

	//モデル読み込み
	pModelManager_ = new ModelManager;
	if (!pModelManager_->Init())
	{
		return false;
	}
	//モデルマネージャセット
	actorManager_.SetModelManager(pModelManager_);

	//セレクター生成
	pCameraSelecter_ = new ActorCameraSelecter(&actorManager_);
	actorManager_.CreateActor(pCameraSelecter_);

	//キューブ生成
	actorManager_.CreateActor(new ActorCube(&actorManager_));

	//FBXモデル生成
	actorManager_.CreateActor(new ActorFbxModel(&actorManager_));

	//時間初期化
	startTime_ = timeGetTime();

	//アクター初期化
	if (!actorManager_.Init())
	{
		return false;
	}

	return true;
}

void SceneGame01::Uninit()
{
	actorManager_.Uninit();
	pTextureManager_->Uninit();
	SafeDelete(pTextureManager_);
	pModelManager_->Uninit();
	SafeDelete(pModelManager_);
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

	//次のカメラを選択
	if (GetKeyboardTrigger(DIK_C))
	{
		pCameraSelecter_->SelectNextType();
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SceneManager::SetScene(new SceneGame02);
	}
}

void SceneGame01::Draw()
{
	actorManager_.Draw();
}
