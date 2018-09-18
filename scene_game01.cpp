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
	//�e�N�X�`���ǂݍ���
	pTextureManager_ = new TextureManager;
	if (!pTextureManager_->Init())
	{
		return false;
	}

	//�e�N�X�`���}�l�[�W���Z�b�g
	actorManager_.SetTextureManager(pTextureManager_);

	//�L���[�u����
	pCube_ = new Cube;
	if (!pCube_->Init())
	{
		return false;
	}

	//���ԏ�����
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

	//���ԍX�V
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
