//**-------------------------------------------------------**
//**
//**                actor_fbx_model.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_FBX_MODEL_H_
#define _ACTOR_FBX_MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <fbxsdk.h>
#include "actor_3d.h"

class ActorCameraSelecter;

class ActorFbxModel : public Actor3d
{
private:
	ActorCameraSelecter* pCameraSelecter_;
public:
	ActorFbxModel(ActorManager* pActorManager);
	virtual ~ActorFbxModel()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
};

#endif // !_ACTOR_FBX_MODEL_H_