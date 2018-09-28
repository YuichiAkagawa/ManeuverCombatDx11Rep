//**-------------------------------------------------------**
//**
//**                actor_field.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor_3d.h"
#include "actor_field.h"

ActorField::ActorField(ActorManager* pActorManager) : Actor3d(pActorManager)
{
}

ActorField::~ActorField()
{

}

bool ActorField::Init()
{
	nameUnique_ = "ActorField" + std::to_string(number_);

	return true;
}

void ActorField::Uninit()
{

}

void ActorField::Update()
{

}

void ActorField::Stats()
{
	Actor3d::Stats();
}

void ActorField::Draw()
{

}