//**-------------------------------------------------------**
//**
//**                     scene_game01.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <DirectXMath.h>
#include "actor.h"
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_cube.h"
#include "actor_fbx_model.h"
#include "actor_grid.h"
#include "actor_manager.h"
#include "actor_player.h"
#include "actor_player_controller.h"
#include "actor_sea.h"
#include "actor_skydome.h"
#include "edit_math.h"
#include "effekseer_effect.h"
#include "imgui/imgui.h"
#include "input.h"
#include "main.h"
#include "model.h"
#include "scene.h"
#include "scene_game02.h"
#include "scene_manager.h"
#include "texture.h"

#include "scene_game01.h"

using namespace DirectX;

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
	//actorManager_.CreateActor(new ActorFbxModel(&actorManager_));

	//プレイヤー生成
	ActorPlayer* pPlayer =  new ActorPlayer(&actorManager_);
	actorManager_.CreateActor(pPlayer);

	//プレイヤーコントローラー生成
	actorManager_.CreateActor(new ActorPlayerController(&actorManager_));

	//海生成
	actorManager_.CreateActor(new ActorSea(&actorManager_));

	//天球生成
	actorManager_.CreateActor(new ActorSkydome(&actorManager_));

	//グリッド表示
	actorManager_.CreateActor(new ActorGrid(&actorManager_));

	//爆発エフェクト初期化
	pExp_ = new EffekseerEffect(EffekseerEffect::BURNER);
	pExp_->SetRepeat(true);
	pExp_->SetColor(255, 255, 255, 255);
	XMFLOAT4X4 mtxExp;
	EditMath::Translation(mtxExp, XMFLOAT3(0.0f, 20.0f, 0.0f));
	pExp_->SetMatrix(mtxExp);
	pExp_->Play();

	//時間初期化
	startTime_ = timeGetTime();

	//アクター初期化
	if (!actorManager_.Init())
	{
		return false;
	}

	// プレイヤー初期位置設定
	pPlayer->SetPos(XMFLOAT3(0.0f, 10.0f, 0.0f));

	return true;
}

void SceneGame01::Uninit()
{
	//爆発エフェクト終了
	SafeDelete(pExp_);

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

	pExp_->SetViewData(
		pCameraSelecter_->GetSelectCamera()->GetPos(),
		pCameraSelecter_->GetSelectCamera()->GetPosAt(),
		pCameraSelecter_->GetSelectCamera()->GetVecUp());
	pExp_->Update();
}

void SceneGame01::Draw()
{
	actorManager_.Draw();
	pExp_->Draw();
}
