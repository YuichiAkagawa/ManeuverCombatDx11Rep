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
	actorManager_.CreateActor(new ActorFbxModel(&actorManager_));

	//���ԏ�����
	startTime_ = timeGetTime();

	//�A�N�^�[������
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
}

void SceneGame01::Draw()
{
	actorManager_.Draw();
}
