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

XController::XCONT_BUTTON_STATUS XController::buttonStatus_[MAX_CONTROLLERS];
XController::XCONT_BUTTON_STATUS XController::buttonStatusOld_[MAX_CONTROLLERS];

XINPUT_STATE XController::controllerState_[MAX_CONTROLLERS];
bool XController::connected_[MAX_CONTROLLERS];
bool XController::isDrawImgui_[MAX_CONTROLLERS];

void XController::InitXcontroller()
{

	for (UINT i = 0; i < MAX_CONTROLLERS; i++)
	{
		buttonStatus_[i].DPAD_UP = false;
		buttonStatus_[i].DPAD_DOWN = false;
		buttonStatus_[i].DPAD_LEFT = false;
		buttonStatus_[i].DPAD_RIGHT = false;
		buttonStatus_[i].START = false;
		buttonStatus_[i].BACK = false;
		buttonStatus_[i].LEFT_THUMB = false;
		buttonStatus_[i].RIGHT_THUMB = false;
		buttonStatus_[i].LEFT_SHOULDER = false;
		buttonStatus_[i].RIGHT_SHOULDER = false;
		buttonStatus_[i].A = false;
		buttonStatus_[i].B = false;
		buttonStatus_[i].X = false;
		buttonStatus_[i].Y = false;
		buttonStatus_[i].L_STICK_UP = false;
		buttonStatus_[i].L_STICK_DOWN = false;
		buttonStatus_[i].L_STICK_RIGHT = false;
		buttonStatus_[i].L_STICK_LEFT = false;

		buttonStatusOld_[i].DPAD_UP = false;
		buttonStatusOld_[i].DPAD_DOWN = false;
		buttonStatusOld_[i].DPAD_LEFT = false;
		buttonStatusOld_[i].DPAD_RIGHT = false;
		buttonStatusOld_[i].START = false;
		buttonStatusOld_[i].BACK = false;
		buttonStatusOld_[i].LEFT_THUMB = false;
		buttonStatusOld_[i].RIGHT_THUMB = false;
		buttonStatusOld_[i].LEFT_SHOULDER = false;
		buttonStatusOld_[i].RIGHT_SHOULDER = false;
		buttonStatusOld_[i].A = false;
		buttonStatusOld_[i].B = false;
		buttonStatusOld_[i].X = false;
		buttonStatusOld_[i].Y = false;
		buttonStatusOld_[i].L_STICK_UP = false;
		buttonStatusOld_[i].L_STICK_DOWN = false;
		buttonStatusOld_[i].L_STICK_RIGHT = false;
		buttonStatusOld_[i].L_STICK_LEFT = false;

		isDrawImgui_[i] = false;
	}
}

void XController::UninitXcontroller()
{
	XInputEnable(false);
}

void XController::UpdateXcontroller()
{
	DWORD dwResult;

	for (UINT i = 0; i < MAX_CONTROLLERS; i++)
	{
		ZeroMemory(&controllerState_[i], sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &controllerState_[i]);

		connected_[i] = false;
		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
			connected_[i] = true;
		}

		//コントローラー状態取得
		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			buttonStatus_[i].DPAD_UP = true;
		}
		else
		{
			buttonStatus_[i].DPAD_UP = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			buttonStatus_[i].DPAD_DOWN = true;
		}
		else
		{
			buttonStatus_[i].DPAD_DOWN = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			buttonStatus_[i].DPAD_LEFT = true;
		}
		else
		{
			buttonStatus_[i].DPAD_LEFT = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			buttonStatus_[i].DPAD_RIGHT = true;
		}
		else
		{
			buttonStatus_[i].DPAD_RIGHT = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			buttonStatus_[i].START = true;
		}
		else
		{
			buttonStatus_[i].START = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			buttonStatus_[i].BACK = true;
		}
		else
		{
			buttonStatus_[i].BACK = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			buttonStatus_[i].LEFT_THUMB = true;
		}
		else
		{
			buttonStatus_[i].LEFT_THUMB = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			buttonStatus_[i].RIGHT_THUMB = true;
		}
		else
		{
			buttonStatus_[i].RIGHT_THUMB = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			buttonStatus_[i].LEFT_SHOULDER = true;
		}
		else
		{
			buttonStatus_[i].LEFT_SHOULDER = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			buttonStatus_[i].RIGHT_SHOULDER = true;
		}
		else
		{
			buttonStatus_[i].RIGHT_SHOULDER = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			buttonStatus_[i].A = true;
		}
		else
		{
			buttonStatus_[i].A = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			buttonStatus_[i].B = true;
		}
		else
		{
			buttonStatus_[i].B = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			buttonStatus_[i].X = true;
		}
		else
		{
			buttonStatus_[i].X = false;
		}

		if (controllerState_[i].Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			buttonStatus_[i].Y = true;
		}
		else
		{
			buttonStatus_[i].Y = false;
		}

		//LStickX
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbLX < LEFT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbLX > -LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLX = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbLX >= LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLX -= (SHORT)LEFT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbLX <= -LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLX += (SHORT)LEFT_STICK_DEADZONE;
		}

		if (controllerState_[i].Gamepad.sThumbLX > LEFT_STICK_DEADZONE)
		{
			buttonStatus_[i].L_STICK_RIGHT = true;
		}
		else
		{
			buttonStatus_[i].L_STICK_RIGHT = false;
		}

		if (controllerState_[i].Gamepad.sThumbLX < -LEFT_STICK_DEADZONE)
		{
			buttonStatus_[i].L_STICK_LEFT = true;
		}
		else
		{
			buttonStatus_[i].L_STICK_LEFT = false;
		}

		//LStickY
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbLY < LEFT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbLY > -LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLY = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbLY >= LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLY -= (SHORT)LEFT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbLY <= -LEFT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbLY += (SHORT)LEFT_STICK_DEADZONE;
		}

		if (controllerState_[i].Gamepad.sThumbLY > LEFT_STICK_DEADZONE)
		{
			buttonStatus_[i].L_STICK_UP = true;
		}
		else
		{
			buttonStatus_[i].L_STICK_UP = false;
		}

		if (controllerState_[i].Gamepad.sThumbLY < -LEFT_STICK_DEADZONE)
		{
			buttonStatus_[i].L_STICK_DOWN = true;
		}
		else
		{
			buttonStatus_[i].L_STICK_DOWN = false;
		}

		//RStickX
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbRX < RIGHT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbRX > -RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRX = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbRX >= RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRX -= (SHORT)RIGHT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbRX <= -RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRX += (SHORT)RIGHT_STICK_DEADZONE;
		}

		//RStickY
		//DeadZone
		if (controllerState_[i].Gamepad.sThumbRY < RIGHT_STICK_DEADZONE && controllerState_[i].Gamepad.sThumbRY > -RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRY = (SHORT)0.0f;
		}
		else if (controllerState_[i].Gamepad.sThumbRY >= RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRY -= (SHORT)RIGHT_STICK_DEADZONE;
		}
		else if (controllerState_[i].Gamepad.sThumbRY <= -RIGHT_STICK_DEADZONE)
		{
			controllerState_[i].Gamepad.sThumbRY += (SHORT)RIGHT_STICK_DEADZONE;
		}
	}
}

void XController::UpdateXcontrollerCreateOld()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		buttonStatusOld_[i] = buttonStatus_[i];
	}
}

void XController::DrawImgui()
{
	//imgui
	ImGui::Begin("Stats");
	if (ImGui::BeginMenu("XInput"))
	{
		for (UINT i = 0; i < MAX_CONTROLLERS; i++)
		{
			std::string str = "Controller " + std::to_string(i + 1);
			if (ImGui::MenuItem(str.c_str()))
			{
				isDrawImgui_[i] = true;
			}
		}
		ImGui::EndMenu();
	}
	ImGui::End();

	for (UINT i = 0; i < MAX_CONTROLLERS; i++)
	{
		XcontrollerValue(&isDrawImgui_[i], i + 1);
	}
}

void XController::XcontrollerValue(bool* isDrawImgui, int number)
{
	if (*isDrawImgui && ImguiManager::GetIsDraw())
	{
		ImGui::SetNextWindowPos(ImVec2(880, 20), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiSetCond_Once);

		std::string str = "XInput Controller " + std::to_string(number);
		ImGui::Begin(str.c_str(), isDrawImgui);

		ImGui::Text("%s", connected_[number - 1] ? "Connected" : "No Connection");

		if (ImGui::TreeNode("Axis"))
		{
			ImGui::Text("LeftStick X : %6d (%.6f)", controllerState_[number - 1].Gamepad.sThumbLX, (controllerState_[number - 1].Gamepad.sThumbLX - (-32768)) / 32768.0f - 1.0f);
			ImGui::Text("LeftStick Y : %6d (%.6f)", controllerState_[number - 1].Gamepad.sThumbLY, (controllerState_[number - 1].Gamepad.sThumbLY - (-32768)) / 32768.0f - 1.0f);
			ImGui::Text("RightStick X : %6d (%.6f)", controllerState_[number - 1].Gamepad.sThumbRX, (controllerState_[number - 1].Gamepad.sThumbRX - (-32768)) / 32768.0f - 1.0f);
			ImGui::Text("RightStick Y : %6d (%.6f)", controllerState_[number - 1].Gamepad.sThumbRY, (controllerState_[number - 1].Gamepad.sThumbRY - (-32768)) / 32768.0f - 1.0f);
			ImGui::Text("LeftTrigger : %6d (%.6f)", controllerState_[number - 1].Gamepad.bLeftTrigger, (controllerState_[number - 1].Gamepad.bLeftTrigger - 0) / 255.0f);
			ImGui::Text("RightTrigger : %6d (%.6f)", controllerState_[number - 1].Gamepad.bRightTrigger, (controllerState_[number - 1].Gamepad.bRightTrigger - 0) / 255.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Button"))
		{
			ImGui::Text("DPAD_UP : %s", buttonStatus_[number - 1].DPAD_UP ? "true" : "false");
			ImGui::Text("DPAD_DOWN : %s", buttonStatus_[number - 1].DPAD_DOWN ? "true" : "false");
			ImGui::Text("DPAD_LEFT : %s", buttonStatus_[number - 1].DPAD_LEFT ? "true" : "false");
			ImGui::Text("DPAD_RIGHT : %s", buttonStatus_[number - 1].DPAD_RIGHT ? "true" : "false");
			ImGui::Text("START : %s", buttonStatus_[number - 1].START ? "true" : "false");
			ImGui::Text("BACK : %s", buttonStatus_[number - 1].BACK ? "true" : "false");
			ImGui::Text("LEFT_THUMB : %s", buttonStatus_[number - 1].LEFT_THUMB ? "true" : "false");
			ImGui::Text("RIGHT_THUMB : %s", buttonStatus_[number - 1].RIGHT_THUMB ? "true" : "false");
			ImGui::Text("LEFT_SHOULDER : %s", buttonStatus_[number - 1].LEFT_SHOULDER ? "true" : "false");
			ImGui::Text("RIGHT_SHOULDER : %s", buttonStatus_[number - 1].RIGHT_SHOULDER ? "true" : "false");
			ImGui::Text("A : %s", buttonStatus_[number - 1].A ? "true" : "false");
			ImGui::Text("B : %s", buttonStatus_[number - 1].B ? "true" : "false");
			ImGui::Text("X : %s", buttonStatus_[number - 1].X ? "true" : "false");
			ImGui::Text("Y : %s", buttonStatus_[number - 1].Y ? "true" : "false");
			ImGui::Text("LEFT_STICK_UP : %s", buttonStatus_[number - 1].L_STICK_UP ? "true" : "false");
			ImGui::Text("LEFT_STICK_DOWN : %s", buttonStatus_[number - 1].L_STICK_DOWN ? "true" : "false");
			ImGui::Text("LEFT_STICK_LEFT : %s", buttonStatus_[number - 1].L_STICK_LEFT ? "true" : "false");
			ImGui::Text("LEFT_STICK_RIGHT : %s", buttonStatus_[number - 1].L_STICK_RIGHT ? "true" : "false");

			ImGui::TreePop();
		}
		ImGui::End();
	}
}

double XController::GetXcontrollerLStickX(int number)
{
	//NormalizeReturn
	return (controllerState_[number - 1].Gamepad.sThumbLX - (-(32768 - LEFT_STICK_DEADZONE))) / (32768.0f - LEFT_STICK_DEADZONE) - 1.0f;
}

double XController::GetXcontrollerLStickY(int number)
{
	//NormalizeReturn
	return (controllerState_[number - 1].Gamepad.sThumbLY - (-(32768 - LEFT_STICK_DEADZONE))) / (32768.0f - LEFT_STICK_DEADZONE) - 1.0f;
}

bool XController::GetXcontrollerLStickUpPress(int number)
{
	return buttonStatus_[number - 1].L_STICK_UP;
}

bool XController::GetXcontrollerLStickDownPress(int number)
{
	return buttonStatus_[number - 1].L_STICK_DOWN;
}

bool XController::GetXcontrollerLStickRightPress(int number)
{
	return buttonStatus_[number - 1].L_STICK_RIGHT;
}

bool XController::GetXcontrollerLStickLeftPress(int number)
{
	return buttonStatus_[number - 1].L_STICK_LEFT;
}

bool XController::GetXcontrollerLStickUpTrigger(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_UP == false)
	{
		return buttonStatus_[number - 1].L_STICK_UP;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickDownTrigger(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_DOWN == false)
	{
		return buttonStatus_[number - 1].L_STICK_DOWN;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickRightTrigger(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_RIGHT == false)
	{
		return buttonStatus_[number - 1].L_STICK_RIGHT;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickLeftTrigger(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_LEFT == false)
	{
		return buttonStatus_[number - 1].L_STICK_LEFT;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickUpRelease(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_UP == true && buttonStatus_[number - 1].L_STICK_UP == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickDownRelease(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_DOWN == true && buttonStatus_[number - 1].L_STICK_DOWN == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickRightRelease(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_RIGHT == true && buttonStatus_[number - 1].L_STICK_RIGHT == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XController::GetXcontrollerLStickLeftRelease(int number)
{
	if (buttonStatusOld_[number - 1].L_STICK_LEFT == true && buttonStatus_[number - 1].L_STICK_LEFT == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double XController::GetXcontrollerRStickX(int number)
{
	//NormalizeReturn
	return (controllerState_[number - 1].Gamepad.sThumbRX - (-(32768 - RIGHT_STICK_DEADZONE))) / (32768.0f - RIGHT_STICK_DEADZONE) - 1.0f;
}

double XController::GetXcontrollerRStickY(int number)
{
	//NormalizeReturn
	return (controllerState_[number - 1].Gamepad.sThumbRY - (-(32768 - RIGHT_STICK_DEADZONE))) / (32768.0f - RIGHT_STICK_DEADZONE) - 1.0f;
}

double XController::GetXcontrollerLTrigger(int number)
{
	//DeadZone
	if (controllerState_[number - 1].Gamepad.bLeftTrigger < TRIGGER_DEADZONE)
	{
		controllerState_[number - 1].Gamepad.bLeftTrigger = (SHORT)0.0f;
	}
	
	//NormalizeReturn
	return (controllerState_[number - 1].Gamepad.bLeftTrigger - 0) / 255.0f;
}

double XController::GetXcontrollerRTrigger(int number)
{
	//DeadZone
	if (controllerState_[number - 1].Gamepad.bRightTrigger < TRIGGER_DEADZONE)
	{
		controllerState_[number - 1].Gamepad.bRightTrigger = (SHORT)0.0f;
	}

	//NormalizeReturn
	return (controllerState_[number - 1].Gamepad.bRightTrigger - 0) / 255.0f;
}

bool XController::GetXcontrollerButtonPress(int number, WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		return buttonStatus_[number - 1].DPAD_UP;
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		return buttonStatus_[number - 1].DPAD_DOWN;
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		return buttonStatus_[number - 1].DPAD_LEFT;
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		return buttonStatus_[number - 1].DPAD_RIGHT;
		break;
	case XINPUT_GAMEPAD_START:
		return buttonStatus_[number - 1].START;
		break;
	case XINPUT_GAMEPAD_BACK:
		return buttonStatus_[number - 1].BACK;
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		return buttonStatus_[number - 1].LEFT_THUMB;
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		return buttonStatus_[number - 1].RIGHT_THUMB;
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		return buttonStatus_[number - 1].LEFT_SHOULDER;
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		return buttonStatus_[number - 1].RIGHT_SHOULDER;
		break;
	case XINPUT_GAMEPAD_A:
		return buttonStatus_[number - 1].A;
		break;
	case XINPUT_GAMEPAD_B:
		return buttonStatus_[number - 1].B;
		break;
	case XINPUT_GAMEPAD_X:
		return buttonStatus_[number - 1].X;
		break;
	case XINPUT_GAMEPAD_Y:
		return buttonStatus_[number - 1].Y;
		break;
	default:
		return false;
		break;
	}
}
bool XController::GetXcontrollerButtonTrigger(int number, WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		if (buttonStatusOld_[number - 1].DPAD_UP == false)
		{
			return buttonStatus_[number - 1].DPAD_UP;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		if (buttonStatusOld_[number - 1].DPAD_DOWN == false)
		{
			return buttonStatus_[number - 1].DPAD_DOWN;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		if (buttonStatusOld_[number - 1].DPAD_LEFT == false)
		{
			return buttonStatus_[number - 1].DPAD_LEFT;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		if (buttonStatusOld_[number - 1].DPAD_RIGHT == false)
		{
			return buttonStatus_[number - 1].DPAD_RIGHT;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_START:
		if (buttonStatusOld_[number - 1].START == false)
		{
			return buttonStatus_[number - 1].START;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_BACK:
		if (buttonStatusOld_[number - 1].BACK == false)
		{
			return buttonStatus_[number - 1].BACK;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		if (buttonStatusOld_[number - 1].LEFT_THUMB == false)
		{
			return buttonStatus_[number - 1].LEFT_THUMB;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		if (buttonStatusOld_[number - 1].RIGHT_THUMB == false)
		{
			return buttonStatus_[number - 1].RIGHT_THUMB;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		if (buttonStatusOld_[number - 1].LEFT_SHOULDER == false)
		{
			return buttonStatus_[number - 1].LEFT_SHOULDER;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		if (buttonStatusOld_[number - 1].RIGHT_SHOULDER == false)
		{
			return buttonStatus_[number - 1].RIGHT_SHOULDER;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_A:
		if (buttonStatusOld_[number - 1].A == false)
		{
			return buttonStatus_[number - 1].A;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_B:
		if (buttonStatusOld_[number - 1].B == false)
		{
			return buttonStatus_[number - 1].B;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_X:
		if (buttonStatusOld_[number - 1].X == false)
		{
			return buttonStatus_[number - 1].X;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_Y:
		if (buttonStatusOld_[number - 1].Y == false)
		{
			return buttonStatus_[number - 1].Y;
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
bool XController::GetXcontrollerButtonRelease(int number, WORD button)
{
	switch (button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		if (buttonStatusOld_[number - 1].DPAD_UP == true && buttonStatus_[number - 1].DPAD_UP == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		if (buttonStatusOld_[number - 1].DPAD_DOWN == true && buttonStatus_[number - 1].DPAD_DOWN == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		if (buttonStatusOld_[number - 1].DPAD_LEFT == true && buttonStatus_[number - 1].DPAD_LEFT == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		if (buttonStatusOld_[number - 1].DPAD_RIGHT == true && buttonStatus_[number - 1].DPAD_RIGHT == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_START:
		if (buttonStatusOld_[number - 1].START == true && buttonStatus_[number - 1].START == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_BACK:
		if (buttonStatusOld_[number - 1].BACK == true && buttonStatus_[number - 1].BACK == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		if (buttonStatusOld_[number - 1].LEFT_THUMB == true && buttonStatus_[number - 1].LEFT_THUMB == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		if (buttonStatusOld_[number - 1].RIGHT_THUMB == true && buttonStatus_[number - 1].RIGHT_THUMB == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		if (buttonStatusOld_[number - 1].LEFT_SHOULDER == true && buttonStatus_[number - 1].LEFT_SHOULDER == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		if (buttonStatusOld_[number - 1].RIGHT_SHOULDER == true && buttonStatus_[number - 1].RIGHT_SHOULDER == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_A:
		if (buttonStatusOld_[number - 1].A == true && buttonStatus_[number - 1].A == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_B:
		if (buttonStatusOld_[number - 1].B == true && buttonStatus_[number - 1].B == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_X:
		if (buttonStatusOld_[number - 1].X == true && buttonStatus_[number - 1].X == false)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case XINPUT_GAMEPAD_Y:
		if (buttonStatusOld_[number - 1].Y == true && buttonStatus_[number - 1].Y == false)
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

bool	XController::GetConnectController(int number)
{
	return connected_[number];
}