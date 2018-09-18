//**-------------------------------------------------------**
//**
//**                    scene_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "main.h"
#include "scene.h"
#include "scene_manager.h"

Scene* SceneManager::pScene_ = nullptr;

bool SceneManager::Init()
{
	return pScene_->Init();
}

void SceneManager::Uninit()
{
	pScene_->Uninit();
	SafeDelete(pScene_);
}

void SceneManager::Update()
{
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