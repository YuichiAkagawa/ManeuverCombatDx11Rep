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
#include "input.h"
#include "xcontroller.h"
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

float ActorPlayerController::Yaw()
{
	if (XController::GetConnectController())
	{
		if (XController::GetXcontrollerButtonPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)
			&& XController::GetXcontrollerButtonPress(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			return 0.0f;
		}

		if (XController::GetXcontrollerButtonPress(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			return 1.0f;
		}
		else if (XController::GetXcontrollerButtonPress(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			return -1.0f;
		}
		else
		{
			return 0.0f;
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_E) && GetKeyboardPress(DIK_Q))
		{
			return 0.0f;
		}

		if (GetKeyboardPress(DIK_E))
		{
			return 1.0f;
		}
		else if (GetKeyboardPress(DIK_Q))
		{
			return -1.0f;
		}
		else
		{
			return 0.0f;
		}
	}
}

float ActorPlayerController::Pitch()
{
	if (XController::GetConnectController())
	{
		return (float)XController::GetXcontrollerLStickY();
	}
	else
	{
		if ((GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_S)))
		{
			return 0.0f;
		}

		if (GetKeyboardPress(DIK_S))
		{
			return -1.0f;
		}
		else if (GetKeyboardPress(DIK_W))
		{
			return 1.0f;
		}
		else
		{
			return 0.0f;
		}
	}
}

float ActorPlayerController::Roll()
{
	if (XController::GetConnectController())
	{
		return (float)-XController::GetXcontrollerLStickX();
	}
	else
	{
		if ((GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_A)))
		{
			return 0.0f;
		}

		if (GetKeyboardPress(DIK_D))
		{
			return -1.0f;
		}
		else if (GetKeyboardPress(DIK_A))
		{
			return 1.0f;
		}
		else
		{
			return 0.0f;
		}
	}
}

float ActorPlayerController::Thrust()
{
	if (XController::GetConnectController())
	{
		return (float)(XController::GetXcontrollerRTrigger() + -(XController::GetXcontrollerLTrigger()));
	}
	else
	{
		if ((GetKeyboardPress(DIK_K) && GetKeyboardPress(DIK_L)))
		{
			return 0.0f;
		}

		if (GetKeyboardPress(DIK_K))
		{
			return 1.0f;
		}
		else if (GetKeyboardPress(DIK_L))
		{
			return -1.0f;
		}
		else
		{
			return 0.0f;
		}
	}
}