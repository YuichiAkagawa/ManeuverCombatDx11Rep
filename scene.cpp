//**-------------------------------------------------------**
//**
//**                      scene.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "scene.h"

const std::vector <int>& Scene::GetSceneModel()const
{
	return sceneModel_;
}

const std::vector <int>& Scene::GetSceneTexture()const
{
	return sceneTexture_;
}