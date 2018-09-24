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
		//�ǂݍ��ރe�N�X�`��
		sceneTexture_.emplace_back(TextureManager::SAMPLE);
		sceneTexture_.emplace_back(TextureManager::NORMAL01);

		//�ǂݍ��ރ��f��
		sceneModel_.emplace_back(ModelManager::SAMPLE);
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