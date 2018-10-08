//**-------------------------------------------------------**
//**
//**                     scene_game01.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SCENE_GAME01_H_
#define _SCENE_GAME01_H_

#include "scene.h"
#include "texture.h"
#include "model.h"

class Cube;

class SceneGame01 : public Scene
{
public:
	SceneGame01()
	{
		//読み込むテクスチャ
		sceneTexture_.emplace_back(TextureManager::SAMPLE);
		sceneTexture_.emplace_back(TextureManager::NORMAL01);
		sceneTexture_.emplace_back(TextureManager::NORMAL02);
		sceneTexture_.emplace_back(TextureManager::NORMAL03);
		sceneTexture_.emplace_back(TextureManager::NORMAL04);
		sceneTexture_.emplace_back(TextureManager::SEA);
		sceneTexture_.emplace_back(TextureManager::SEA_NORMAL01);
		sceneTexture_.emplace_back(TextureManager::SEA_NORMAL02);

		//読み込むモデル
		sceneModel_.emplace_back(ModelManager::SAMPLE);
		sceneModel_.emplace_back(ModelManager::HORNET_PILOT);
		sceneModel_.emplace_back(ModelManager::HORNET_BODY);
		sceneModel_.emplace_back(ModelManager::HORNET_BODY_PARTS);
		sceneModel_.emplace_back(ModelManager::HORNET_BODY_PARTS2);
		sceneModel_.emplace_back(ModelManager::HORNET_AIM120);
		sceneModel_.emplace_back(ModelManager::HORNET_AIM9_RIGHT);
		sceneModel_.emplace_back(ModelManager::HORNET_AIM9_LEFT);
		sceneModel_.emplace_back(ModelManager::HORNET_FLAP_RIGHT);
		sceneModel_.emplace_back(ModelManager::HORNET_FLAP_LEFT);
		sceneModel_.emplace_back(ModelManager::HORNET_ELEVATOR_RIGHT);
		sceneModel_.emplace_back(ModelManager::HORNET_ELEVATOR_LEFT);
		sceneModel_.emplace_back(ModelManager::HORNET_AILERON_RIGHT);
		sceneModel_.emplace_back(ModelManager::HORNET_AILERON_LEFT);
		sceneModel_.emplace_back(ModelManager::HORNET_RADDER_RIGHT);
		sceneModel_.emplace_back(ModelManager::HORNET_RADDER_LEFT);
		sceneModel_.emplace_back(ModelManager::HORNET_CANOPY);
		sceneModel_.emplace_back(ModelManager::HORNET_GLASS_FRONT);
		sceneModel_.emplace_back(ModelManager::HORNET_GLASS_BACK);
		sceneModel_.emplace_back(ModelManager::SKYDOME);
	}
	~SceneGame01()
	{

	}
	bool Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:
};

#endif // !_SCENE_GAME01_H_