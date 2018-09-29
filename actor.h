//**-------------------------------------------------------**
//**
//**                     actor.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <string>
#include <typeinfo>
#include "process_calculator.h"

class ActorManager;
class TextureManager;
class ModelManager;

class Actor
{
protected:
	ActorManager* pActorManager_;
	TextureManager* pTextureManager_;
	ModelManager* pModelManager_;
	int name_;
	std::string nameUnique_;
	int number_;
	int tag_;
	int layer_;
	bool isUpdate_ = true;
	bool isDraw_ = true;
	bool isPausingUpdate_ = false;
	bool isDrawImgui_ = false;
	ProcessCalculator updateProcessCalc_;
	ProcessCalculator drawProcessCalc_;

public:
	Actor(ActorManager* pActorManager);
	virtual ~Actor();
	virtual bool Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Stats();
	virtual void Draw() = 0;

	//Getter
	int GetName();
	const std::string& GetNameUnique()const;
	int GetNumber();
	int GetLayer();
	bool GetIsUpdate();
	bool GetIsDraw();
	bool GetIsDrawImgui();
	ProcessCalculator& GetUpdateProcessCalc();
	ProcessCalculator& GetDrawProcessCalc();

	//Setter
	void SetTextureManager(TextureManager* pTextureManager);
	void SetModelManager(ModelManager* pModelManager);
	void SetName(int name);
	void SetNameUnique(const std::string& nameUnique);
	void SetTypeInfo(const std::type_info* typeInfo);
	void SetNumber(int number);
	void SetLayer(int layer);
	void SetIsUpdate(bool isUpdate);
	void SetIsDraw(bool isDraw);
	void SetIsDrawImgui(bool isDrawImgui);

};

#endif // !_ACTOR_H_
