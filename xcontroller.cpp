//**-------------------------------------------------------**
//**
//**                  xcontroller.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <Windows.h>
#include <string>
#include "imgui/imgui.h"
#include "imgui_manager.h"
#include "main.h"
#include "xcontroller.h"

constexpr double LEFT_STICK_DEADZONE = 8000.0;
constexpr double RIGHT_STICK_DEADZONE = 8000.0;
constexpr double TRIGGER_DEADZONE = 20.0;

XController::XCONT_BUTTON_STATUS XController::buttonStatus_;
XController::XCONT_BUTTON_STATUS XController::buttonStatusOld_;

XINPUT_STATE XController::controllerState_;
bool XController::connected_;
bool XController::isDrawImgui_;

void XController::InitXcontroller()
{
	buttonStatus_.DPAD_UP = false;
	buttonStatus_.DPAD_DOWN = false;
	buttonStatus_.DPAD_LEFT = false;
	buttonStatus_.DPAD_RIGHT = false;
	buttonStatus_.START = false;
	buttonStatus_.BACK = false;
	buttonStatus_.LEFT_THUMB = false;
	buttonStatus_.RIGHT_THUMB = false;
	buttonStatus_.LEFT_SHOULDER = false;
	buttonStatus_.RIGHT_SHOULDER = false;
	buttonStatus_.A = false;
	buttonStatus_.B = false;
	buttonStatus_.X = false;
	buttonStatus_.Y = false;
	buttonStatus_.L_STICK_UP = false;
	buttonStatus_.L_STICK_DOWN = false;
	buttonStatus_.L_STICK_RIGHT = false;
	buttonStatus_.L_STICK_LEFT = false;

	buttonStatusOld_.DPAD_UP = false;
	buttonStatusOld_.DPAD_DOWN = false;
	buttonStatusOld_.DPAD_LEFT = false;
	buttonStatusOld_.DPAD_RIGHT = false;
	buttonStatusOld_.START = false;
	buttonStatusOld_.BACK = false;
	buttonStatusOld_.LEFT_THUMB = false;
	buttonStatusOld_.RIGHT_THUMB = false;
	buttonStatusOld_.LEFT_SHOULDER = false;
	buttonStatusOld_.RIGHT_SHOULDER = false;
	buttonStatusOld_.A = false;
	buttonStatusOld_.B = false;
	buttonStatusOld_.X = false;
	buttonStatusOld_.Y = false;
	buttonStatusOld_.L_STICK_UP = false;
	buttonStatusOld_.L_STICK_DOWN = false;
	buttonStatusOld_.L_STICK_RIGHT = false;
	buttonStatusOld_.L_STICK_LEFT = false;

	isDrawImgui_ = false;
}

void XController::UninitXcontroller()
{
	XInputEnable(false);
}

void XController::UpdateXcontroller()
{
	DWORD dwResult;

	ZeroMemory(&controllerState_, sizeof(XINPUT_STATE));

	// Simply get the state of the controller from XInput.
	dwResult = XInputGetState(0, &controllerState_);

	connected_ = false;
	if (dwResult == ERROR_SUCCESS)
	{
		// Controller is connected
		connected_ = true;
	}

	//コントローラー状態取得
	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		buttonStatus_.DPAD_UP = true;
	}
	else
	{
		buttonStatus_.DPAD_UP = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		buttonStatus_.DPAD_DOWN = true;
	}
	else
	{
		buttonStatus_.DPAD_DOWN = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		buttonStatus_.DPAD_LEFT = true;
	}
	else
	{
		buttonStatus_.DPAD_LEFT = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		buttonStatus_.DPAD_RIGHT = true;
	}
	else
	{
		buttonStatus_.DPAD_RIGHT = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		buttonStatus_.START = true;
	}
	else
	{
		buttonStatus_.START = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
	{
		buttonStatus_.BACK = true;
	}
	else
	{
		buttonStatus_.BACK = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		buttonStatus_.LEFT_THUMB = true;
	}
	else
	{
		buttonStatus_.LEFT_THUMB = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		buttonStatus_.RIGHT_THUMB = true;
	}
	else
	{
		buttonStatus_.RIGHT_THUMB = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		buttonStatus_.LEFT_SHOULDER = true;
	}
	else
	{
		buttonStatus_.LEFT_SHOULDER = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		buttonStatus_.RIGHT_SHOULDER = true;
	}
	else
	{
		buttonStatus_.RIGHT_SHOULDER = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		buttonStatus_.A = true;
	}
	else
	{
		buttonStatus_.A = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		buttonStatus_.B = true;
	}
	else
	{
		buttonStatus_.B = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		buttonStatus_.X = true;
	}
	else
	{
		buttonStatus_.X = false;
	}

	if (controllerState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		buttonStatus_.Y = true;
	}
	else
	{
		buttonStatus_.Y = false;
	}

	//LStickX
	//DeadZone
	if (controllerState_.Gamepad.sThumbLX < LEFT_STICK_DEADZONE && controllerState_.Gamepad.sThumbLX > -LEFT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbLX = (SHORT)0.0f;
	}
	else if (controllerState_.Gamepad.sThumbLX >= LEFT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbLX -= (SHORT)LEFT_STICK_DEADZONE;
	}
	else if (controllerState_.Gamepad.sThumbLX <= -LEFT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbLX += (SHORT)LEFT_STICK_DEADZONE;
	}

	if (controllerState_.Gamepad.sThumbLX > LEFT_STICK_DEADZONE)
	{
		buttonStatus_.L_STICK_RIGHT = true;
	}
	else
	{
		buttonStatus_.L_STICK_RIGHT = false;
	}

	if (controllerState_.Gamepad.sThumbLX < -LEFT_STICK_DEADZONE)
	{
		buttonStatus_.L_STICK_LEFT = true;
	}
	else
	{
		buttonStatus_.L_STICK_LEFT = false;
	}

	//LStickY
	//DeadZone
	if (controllerState_.Gamepad.sThumbLY < LEFT_STICK_DEADZONE && controllerState_.Gamepad.sThumbLY > -LEFT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbLY = (SHORT)0.0f;
	}
	else if (controllerState_.Gamepad.sThumbLY >= LEFT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbLY -= (SHORT)LEFT_STICK_DEADZONE;
	}
	else if (controllerState_.Gamepad.sThumbLY <= -LEFT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbLY += (SHORT)LEFT_STICK_DEADZONE;
	}

	if (controllerState_.Gamepad.sThumbLY > LEFT_STICK_DEADZONE)
	{
		buttonStatus_.L_STICK_UP = true;
	}
	else
	{
		buttonStatus_.L_STICK_UP = false;
	}

	if (controllerState_.Gamepad.sThumbLY < -LEFT_STICK_DEADZONE)
	{
		buttonStatus_.L_STICK_DOWN = true;
	}
	else
	{
		buttonStatus_.L_STICK_DOWN = false;
	}

	//RStickX
	//DeadZone
	if (controllerState_.Gamepad.sThumbRX < RIGHT_STICK_DEADZONE && controllerState_.Gamepad.sThumbRX > -RIGHT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbRX = (SHORT)0.0f;
	}
	else if (controllerState_.Gamepad.sThumbRX >= RIGHT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbRX -= (SHORT)RIGHT_STICK_DEADZONE;
	}
	else if (controllerState_.Gamepad.sThumbRX <= -RIGHT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbRX += (SHORT)RIGHT_STICK_DEADZONE;
	}

	//RStickY
	//DeadZone
	if (controllerState_.Gamepad.sThumbRY < RIGHT_STICK_DEADZONE && controllerState_.Gamepad.sThumbRY > -RIGHT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbRY = (SHORT)0.0f;
	}
	else if (controllerState_.Gamepad.sThumbRY >= RIGHT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbRY -= (SHORT)RIGHT_STICK_DEADZONE;
	}
	else if (controllerState_.Gamepad.sThumbRY <= -RIGHT_STICK_DEADZONE)
	{
		controllerState_.Gamepad.sThumbRY += (SHORT)RIGHT_STICK_DEADZONE;
	}
}

void XController::UpdateXcontrollerCreateOld()
{

	buttonStatusOld_ = buttonStatus_;
}

void XController::DrawImgui()
{
	//imgui
	ImGui::Begin("Stats");

	if (ImGui::MenuItem("Controller"))
	{
		isDrawImgui_ = true;
	}

	ImGui::End();

	XcontrollerValue(&isDrawImgui_);
}

void XController::XcontrollerValue(bool* isDrawImgui)
{
	if (*isDrawImgui && ImguiManager::GetIsDraw())
	{
		ImGui::SetNextWindowPos(ImVec2(880, 20), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiSetCond_Once);

		ImGui::Begin("Controller", isDrawImgui);

		ImGui::Text("%s", connected_ ? "Connected" : "No Connection");

		if (ImGui::TreeNode("Axis"))
		{
			ImGui::Text("LeftStick X : %6d (%.6f)", controllerState_.Gamepad.sThumbLX, GetXcontrollerLStickX());
			ImGui::Text("LeftStick Y : %6d (%.6f)", controllerState_.Gamepad.sThumbLY, GetXcontrollerLStickY());
			ImGui::Text("RightStick X : %6d (%.6f)", controllerState_.Gamepad.sThumbRX, GetXcontrollerRStickX());
			ImGui::Text("RightStick Y : %6d (%.6f)", controllerState_.Gamepad.sThumbRY, GetXcontrollerRStickY());
			ImGui::Text("LeftTrigger : %6d (%.6f)", controllerState_.Gamepad.bLeftTrigger, GetXcontrollerLTrigger());
			ImGui::Text("RightTrigger : %6d (%.6f)", controllerState_.Gamepad.bRightTrigger, GetXcontrollerRTrigger());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Button"))
		{
			ImGui::Text("DPAD_UP : %s", buttonStatus_.DPAD_UP ? "true" : "false");
			ImGui::Text("DPAD_DOWN : %s", buttonStatus_.DPAD_DOWN ? "true" : "false");
			ImGui::Text("DPAD_LEFT : %s", buttonStatus_.DPAD_LEFT ? "true" : "false");
			ImGui::Text("DPAD_RIGHT : %s", buttonStatus_.DPAD_RIGHT ? "true" : "false");
			ImGui::Text("START : %s", buttonStatus_.START ? "true" : "false");
			ImGui::Text("BACK : %s", buttonStatus_.BACK ? "true" : "false");
			ImGui::Text("LEFT_THUMB : %s", buttonStatus_.LEFT_THUMB ? "true" : "false");
			ImGui::Text("RIGHT_THUMB : %s", buttonStatus_.RIGHT_THUMB ? "true" : "false");
			ImGui::Text("LEFT_SHOULDER : %s", buttonStatus_.LEFT_SHOULDER ? "true" : "false");
			ImGui::Text("RIGHT_SHOULDER : %s", buttonStatus_.RIGHT_SHOULDER ? "true" : "false");
			ImGui::Text("A : %s", buttonStatus_.A ? "true" : "false");
			ImGui::Text("B : %s", buttonStatus_.B ? "true" : "false");
			ImGui::Text("X : %s", buttonStatus_.X ? "true" : "false");
			ImGui::Text("Y : %s", buttonStatus_.Y ? "true" : "false");
			ImGui::Text("LEFT_STICK_UP : %s", buttonStatus_.L_STICK_UP ? "true" : "false");
			ImGui::Text("LEFT_STICK_DOWN : %s", buttonStatus_.L_STICK_DOWN ? "true" : "false");
			ImGui::Text("LEFT_STICK_LEFT : %s", buttonStatus_.L_STICK_LEFT ? "true" : "false");
			ImGui::Text("LEFT_STICK_RIGHT : %s", buttonStatus_.L_STICK_RIGHT ? "true" : "false");

			ImGui::TreePop();
		}
		ImGui::End();
	}
}

double XController::GetXcontrollerLStickX()
{
	//NormalizeReturn
	return (controllerState_.Gamepad.sThumbLX - (-(32768 - LEFT_STICK_DEADZONE))) / (32768.0f - LEFT_STICK_DEADZONE) - 1.0f;
}

double XController::GetXcontrollerLStickY()
{
	//NormalizeReturn
	return (controllerState_.Gamepad.sThumbLY - (-(32768 - LEFT_STICK_DEADZONE))) / (32768.0f - LEFT_STICK_DEADZONE) - 1.0f;
}

bool XController::GetXcontrollerLStickUpPress()
{
	return buttonStatus_.L_STICK_UP;
}

bool XController::GetXcontrollerLStickDownPress()
{
	return buttonStatus_.L_STICK_DOWN;
}

bool XController::GetXcontrollerLStickRightPress()
{
	return buttonStatus_.L_STICK_RIGHT;
}

bool XController::GetXcontrollerLStickLeftPress()
{
	return buttonStatus_.L_STICK_LEFT;
}

bool XController::GetXcontrollerLStickUpTrigger()
{
	if (buttonStatusOld_.L_STICK_UP == false)
	{
		return buttonStatus_.L_STICK_UP;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickDownTrigger()
{
	if (buttonStatusOld_.L_STICK_DOWN == false)
	{
		return buttonStatus_.L_STICK_DOWN;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickRightTrigger()
{
	if (buttonStatusOld_.L_STICK_RIGHT == false)
	{
		return buttonStatus_.L_STICK_RIGHT;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickLeftTrigger()
{
	if (buttonStatusOld_.L_STICK_LEFT == false)
	{
		return buttonStatus_.L_STICK_LEFT;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickUpRelease()
{
	if (buttonStatusOld_.L_STICK_UP == true && buttonStatus_.L_STICK_UP == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickDownRelease()
{
	if (buttonStatusOld_.L_STICK_DOWN == true && buttonStatus_.L_STICK_DOWN == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickRightRelease()
{
	if (buttonStatusOld_.L_STICK_RIGHT == true && buttonStatus_.L_STICK_RIGHT == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickLeftRelease()
{
	if (buttonStatusOld_.L_STICK_LEFT == true && buttonStatus_.L_STICK_LEFT == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double XController::GetXcontrollerRStickX()
{
	//NormalizeReturn
	return (controllerState_.Gamepad.sThumbRX - (-(32768 - RIGHT_STICK_DEADZONE))) / (32768.0f - RIGHT_STICK_DEADZONE) - 1.0f;
}

double XController::GetXcontrollerRStickY()
{
	//NormalizeReturn
	return (controllerState_.Gamepad.sThumbRY - (-(32768 - RIGHT_STICK_DEADZONE))) / (32768.0f - RIGHT_STICK_DEADZONE) - 1.0f;
}

double XController::GetXcontrollerLTrigger()
{
	//DeadZone
	if (controllerState_.Gamepad.bLeftTrigger < TRIGGER_DEADZONE)
	{
		controllerState_.Gamepad.bLeftTrigger = (SHORT)0.0f;
	}
	
	//NormalizeReturn
	return (controllerState_.Gamepad.bLeftTrigger - 0) / 255.0f;
}

double XController::GetXcontrollerRTrigger()
{
	//DeadZone
	if (controllerState_.Gamepad.bRightTrigger < TRIGGER_DEADZONE)
	{
		controllerState_.Gamepad.bRightTrigger = (SHORT)0.0f;
	}

	//NormalizeReturn
	return (controllerState_.Gamepad.bRightTrigger - 0) / 255.0f;
}

bool XController::GetXcontrollerButtonPress(WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		return buttonStatus_.DPAD_UP;
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		return buttonStatus_.DPAD_DOWN;
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		return buttonStatus_.DPAD_LEFT;
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		return buttonStatus_.DPAD_RIGHT;
		break;
	case XINPUT_GAMEPAD_START:
		return buttonStatus_.START;
		break;
	case XINPUT_GAMEPAD_BACK:
		return buttonStatus_.BACK;
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		return buttonStatus_.LEFT_THUMB;
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		return buttonStatus_.RIGHT_THUMB;
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		return buttonStatus_.LEFT_SHOULDER;
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		return buttonStatus_.RIGHT_SHOULDER;
		break;
	case XINPUT_GAMEPAD_A:
		return buttonStatus_.A;
		break;
	case XINPUT_GAMEPAD_B:
		return buttonStatus_.B;
		break;
	case XINPUT_GAMEPAD_X:
		return buttonStatus_.X;
		break;
	case XINPUT_GAMEPAD_Y:
		return buttonStatus_.Y;
		break;
	default:
		return false;
		break;
	}
}
bool XController::GetXcontrollerButtonTrigger(WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		if (buttonStatusOld_.DPAD_UP == false)
		{
			return buttonStatus_.DPAD_UP;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		if (buttonStatusOld_.DPAD_DOWN == false)
		{
			return buttonStatus_.DPAD_DOWN;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		if (buttonStatusOld_.DPAD_LEFT == false)
		{
			return buttonStatus_.DPAD_LEFT;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		if (buttonStatusOld_.DPAD_RIGHT == false)
		{
			return buttonStatus_.DPAD_RIGHT;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_START:
		if (buttonStatusOld_.START == false)
		{
			return buttonStatus_.START;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_BACK:
		if (buttonStatusOld_.BACK == false)
		{
			return buttonStatus_.BACK;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		if (buttonStatusOld_.LEFT_THUMB == false)
		{
			return buttonStatus_.LEFT_THUMB;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		if (buttonStatusOld_.RIGHT_THUMB == false)
		{
			return buttonStatus_.RIGHT_THUMB;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		if (buttonStatusOld_.LEFT_SHOULDER == false)
		{
			return buttonStatus_.LEFT_SHOULDER;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		if (buttonStatusOld_.RIGHT_SHOULDER == false)
		{
			return buttonStatus_.RIGHT_SHOULDER;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_A:
		if (buttonStatusOld_.A == false)
		{
			return buttonStatus_.A;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_B:
		if (buttonStatusOld_.B == false)
		{
			return buttonStatus_.B;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_X:
		if (buttonStatusOld_.X == false)
		{
			return buttonStatus_.X;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_Y:
		if (buttonStatusOld_.Y == false)
		{
			return buttonStatus_.Y;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
		break;
	}
}
bool XController::GetXcontrollerButtonRelease(WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		if (buttonStatusOld_.DPAD_UP == true && buttonStatus_.DPAD_UP == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		if (buttonStatusOld_.DPAD_DOWN == true && buttonStatus_.DPAD_DOWN == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		if (buttonStatusOld_.DPAD_LEFT == true && buttonStatus_.DPAD_LEFT == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		if (buttonStatusOld_.DPAD_RIGHT == true && buttonStatus_.DPAD_RIGHT == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_START:
		if (buttonStatusOld_.START == true && buttonStatus_.START == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_BACK:
		if (buttonStatusOld_.BACK == true && buttonStatus_.BACK == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		if (buttonStatusOld_.LEFT_THUMB == true && buttonStatus_.LEFT_THUMB == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		if (buttonStatusOld_.RIGHT_THUMB == true && buttonStatus_.RIGHT_THUMB == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		if (buttonStatusOld_.LEFT_SHOULDER == true && buttonStatus_.LEFT_SHOULDER == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		if (buttonStatusOld_.RIGHT_SHOULDER == true && buttonStatus_.RIGHT_SHOULDER == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_A:
		if (buttonStatusOld_.A == true && buttonStatus_.A == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_B:
		if (buttonStatusOld_.B == true && buttonStatus_.B == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_X:
		if (buttonStatusOld_.X == true && buttonStatus_.X == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_Y:
		if (buttonStatusOld_.Y == true && buttonStatus_.Y == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
		break;
	}
}

bool	XController::GetConnectController()
{
	return connected_;
}