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
class Cube;

class SceneGame01 : public Scene
{
public:
	SceneGame01()
	{
		//読み込むテクスチャ
		sceneTexture_.emplace_back(TextureManager::SAMPLE);
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