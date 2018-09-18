//**-------------------------------------------------------**
//**
//**                     scene_game02.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SCENE_GAME02_H_
#define _SCENE_GAME02_H_

#include "scene.h"

class SceneGame02 : public Scene
{
public:
	SceneGame02()
	{

	}
	~SceneGame02()
	{

	}
	bool Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:

};

#endif // !_SCENE_GAME02_H_