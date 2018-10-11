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
	//�e�N�X�`���ǂݍ���
	pTextureManager_ = new TextureManager;
	if (!pTextureManager_->Init())
	{
		return false;
	}
	//�e�N�X�`���}�l�[�W���Z�b�g
	actorManager_.SetTextureManager(pTextureManager_);

	//���f���ǂݍ���
	pModelManager_ = new ModelManager;
	if (!pModelManager_->Init())
	{
		return false;
	}

	//���f���}�l�[�W���Z�b�g
	actorManager_.SetModelManager(pModelManager_);

	//�Z���N�^�[����
	pCameraSelecter_ = new ActorCameraSelecter(&actorManager_);
	actorManager_.CreateActor(pCameraSelecter_);

	//�L���[�u����
	actorManager_.CreateActor(new ActorCube(&actorManager_));

	//FBX���f������
	//actorManager_.CreateActor(new ActorFbxModel(&actorManager_));

	//�v���C���[����
	ActorPlayer* pPlayer =  new ActorPlayer(&actorManager_);
	actorManager_.CreateActor(pPlayer);

	//�v���C���[�R���g���[���[����
	actorManager_.CreateActor(new ActorPlayerController(&actorManager_));

	//�C����
	actorManager_.CreateActor(new ActorSea(&actorManager_));

	//�V������
	actorManager_.CreateActor(new ActorSkydome(&actorManager_));

	//�O���b�h�\��
	actorManager_.CreateActor(new ActorGrid(&actorManager_));

	//�����G�t�F�N�g������
	pExp_ = new EffekseerEffect(EffekseerEffect::BURNER);
	pExp_->SetRepeat(true);
	pExp_->SetColor(255, 255, 255, 255);
	XMFLOAT4X4 mtxExp;
	EditMath::Translation(mtxExp, XMFLOAT3(0.0f, 20.0f, 0.0f));
	pExp_->SetMatrix(mtxExp);
	pExp_->Play();

	//���ԏ�����
	startTime_ = timeGetTime();

	//�A�N�^�[������
	if (!actorManager_.Init())
	{
		return false;
	}

	// �v���C���[�����ʒu�ݒ�
	pPlayer->SetPos(XMFLOAT3(0.0f, 10.0f, 0.0f));

	return true;
}

void SceneGame01::Uninit()
{
	//�����G�t�F�N�g�I��
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

	//���ԍX�V
	time_ = (float(timeGetTime() - startTime_)) / 1000.0f;

	//���̃J������I��
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
