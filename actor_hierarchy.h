//**-------------------------------------------------------**
//**
//**               actor_hierarchy.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _ACTOR_HIERARCHY_H_
#define _ACTOR_HIERARCHY_H_

#include "actor_3d.h"

class ActorHierarchy : public Actor3d
{
protected:
	Actor3d* pActorParent_ = nullptr;
	DirectX::XMFLOAT3 transOffset_ = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotOffset_ = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scaleOffset_ = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 vecRotAxisOffset_ = { 1.0f, 0.0f, 0.0f };
	float rotAxis_ = 0.0f;

	virtual void ReflectionMatrix();
public:
	ActorHierarchy(ActorManager* pActorManager);
	virtual ~ActorHierarchy()override;
	virtual bool Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Stats()override;
	virtual void Draw()override;
	
	//Getter
	const DirectX::XMFLOAT3& GetTransOffset()const;
	const DirectX::XMFLOAT3& GetRotOffset()const;
	const DirectX::XMFLOAT3& GetScaleOffset()const;
	const DirectX::XMFLOAT3& GetVecRotAxisOffset()const;
	float GetRotAxis();

	//Setter
	void SetParent(Actor3d* pActorParent);
	void SetTransOffset(const DirectX::XMFLOAT3& transOffset);
	void SetRotOffset(const DirectX::XMFLOAT3& rotOffset);
	void SetScaleOffset(const DirectX::XMFLOAT3& scaleOffset);
	void SetVecRotAxisOffset(const DirectX::XMFLOAT3& vecRotAxisOffset);
	void SetRotAxis(float rotAxis);
};

#endif // !_ACTOR_HIERARCHY_H_
