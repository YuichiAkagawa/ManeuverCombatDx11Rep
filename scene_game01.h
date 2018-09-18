//**-------------------------------------------------------**
//**
//**                     scene_game01.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SCENE_GAME01_H_
#define _SCENE_GAME01_H_

#include "scene.h"

class SceneGame01 : public Scene
{
public:
	SceneGame01()
	{

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