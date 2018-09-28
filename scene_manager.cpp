//**-------------------------------------------------------**
//**
//**                    scene_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "main.h"
#include "process_calculator.h"
#include "scene.h"
#include "scene_manager.h"

Scene* SceneManager::pScene_ = nullptr;
ProcessCalculator SceneManager::gameSystemProcessCal_;
double SceneManager::gameSystemProcessTime_;

bool SceneManager::Init()
{
	if (!pScene_->Init())
	{
		return false;
	}
	//ゲームシステム用処理時間計測初期化
	gameSystemProcessCal_.StartCalculate();
	gameSystemProcessTime_ = 0.0;

	return true;
}

void SceneManager::Uninit()
{
	pScene_->Uninit();
	SafeDelete(pScene_);
}

void SceneManager::Update()
{
	//ゲームシステム用処理時間計測終了
	gameSystemProcessCal_.EndCalculate();
	gameSystemProcessTime_ = gameSystemProcessCal_.GetProcessTimeMS();

	//ゲームシステム用処理時間計測開始
	gameSystemProcessCal_.StartCalculate();

	pScene_->Update();
}

void SceneManager::Draw()
{
	pScene_->Draw();
}

bool SceneManager::SetScene(Scene* pScene)
{
	if (pScene_ != nullptr)
	{
		Uninit();
	}

	pScene_ = pScene;

	if (Init())
	{
		return true;
	}
	else
	{
		pScene_->Uninit();
		return false;
	}
}

Scene* SceneManager::GetScene()
{
	return pScene_;
}

double SceneManager::GetProcessMS()
{
	return gameSystemProcessTime_;
}