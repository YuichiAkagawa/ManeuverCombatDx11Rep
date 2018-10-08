//**-------------------------------------------------------**
//**
//**                    scene.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "actor_manager.h"
#include "actor_camera_selecter.h"

class TextureManager;
class ModelManager;

class Scene
{
protected:
	ActorManager actorManager_;
	ActorCameraSelecter* pCameraSelecter_;
	std::vector <int> sceneTexture_;
	std::vector <int> sceneModel_;
	TextureManager* pTextureManager_;
	ModelManager* pModelManager_;
	unsigned int startTime_;
	float time_;
public:
	Scene() {}
	virtual ~Scene() {}
	virtual bool Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	const std::vector <int>& GetSceneTexture()const;
	const std::vector <int>& GetSceneModel()const;
	float GetTime();
};

#endif // !_SCENE_H_