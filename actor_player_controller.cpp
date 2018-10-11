//**-------------------------------------------------------**
//**
//**             actor_player_controller.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor.h"
#include "actor_manager.h"
#include "imgui/imgui.h"
#include "imgui_manager.h"
#include "actor_player_controller.h"

ActorPlayerController::ActorPlayerController(ActorManager* pActorManager) : Actor(pActorManager)
{
	name_ = ActorManager::NAME_PLAYER_CONTROLLER;
	tag_ = ActorManager::TAG_ETC;
	layer_ = ActorManager::LAYER_ETC;
}

ActorPlayerController::~ActorPlayerController()
{

}

bool ActorPlayerController::Init()
{
	nameUnique_ = "ActorPlayerController" + std::to_string(number_);
	return true;
}

void ActorPlayerController::Uninit()
{

}

void ActorPlayerController::Update()
{

}

void ActorPlayerController::Stats()
{
	Actor::Stats();
}

void ActorPlayerController::Draw()
{

}