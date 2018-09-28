//**-------------------------------------------------------**
//**
//**                    scene_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

class Scene;

class SceneManager
{
public:
	static bool Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static bool SetScene(Scene* pScene);
	static Scene* GetScene();
	static double GetProcessMS();

private:
	static Scene* pScene_;
	static ProcessCalculator	gameSystemProcessCal_;
	static double				gameSystemProcessTime_;
};

#endif // !_SCENE_MANAGER_H_