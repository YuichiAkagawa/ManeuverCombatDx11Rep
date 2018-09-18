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

class Scene
{
protected:
	ActorManager actorManager_;
	std::vector <int> sceneModel_;
	std::vector <int> sceneTexture_;
	unsigned int startTime_;
	float time_;
public:
	Scene() {}
	virtual ~Scene() {}
	virtual bool Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	const std::vector <int>& GetSceneModel()const;
	const std::vector <int>& GetSceneTexture()const;
};

#endif // !_SCENE_H_