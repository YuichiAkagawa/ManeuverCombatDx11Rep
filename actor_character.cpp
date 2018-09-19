//**-------------------------------------------------------**
//**
//**                actor_charcter.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor_3d.h"
#include "actor_character.h"

ActorCharcter::ActorCharcter(ActorManager* pActorManager) : Actor3d(pActorManager)
{
}

ActorCharcter::~ActorCharcter()
{

}

bool ActorCharcter::Init()
{
	nameUnique_ = "ActorCharcter" + std::to_string(number_);

	return true;
}

void ActorCharcter::Uninit()
{

}

void ActorCharcter::Update()
{

}

void ActorCharcter::Stats()
{
	Actor3d::Stats();
}

void ActorCharcter::Draw()
{

}