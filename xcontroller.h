//**-------------------------------------------------------**
//**
//**                  xcontroller.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _XCONTROLLER_H_
#define _XCONTROLLER_H_

#include <Xinput.h>

constexpr UINT MAX_CONTROLLERS = 4;

class XController
{
private:
	struct XCONT_BUTTON_STATUS
	{
		bool DPAD_UP;
		bool DPAD_DOWN;
		bool DPAD_LEFT;
		bool DPAD_RIGHT;
		bool START;
		bool BACK;
		bool LEFT_THUMB;
		bool RIGHT_THUMB;
		bool LEFT_SHOULDER;
		bool RIGHT_SHOULDER;
		bool A;
		bool B;
		bool X;
		bool Y;
		bool L_STICK_UP;
		bool L_STICK_DOWN;
		bool L_STICK_RIGHT;
		bool L_STICK_LEFT;
	};

	static XCONT_BUTTON_STATUS buttonStatus_[MAX_CONTROLLERS];
	static XCONT_BUTTON_STATUS buttonStatusOld_[MAX_CONTROLLERS];

	static XINPUT_STATE controllerState_[MAX_CONTROLLERS];
	static bool connected_[MAX_CONTROLLERS];
	static bool isDrawImgui_[MAX_CONTROLLERS];

	static void	XcontrollerValue(bool* isDrawImgui, int number);

public:
	static void	InitXcontroller();
	static void	UninitXcontroller();
	static void	UpdateXcontroller();
	static void	UpdateXcontrollerCreateOld();
	static void DrawImgui();
	
	static double	GetXcontrollerLStickX(int number);
	static double	GetXcontrollerLStickY(int number);
	static bool	GetXcontrollerLStickUpPress(int number);
	static bool	GetXcontrollerLStickDownPress(int number);
	static bool	GetXcontrollerLStickRightPress(int number);
	static bool	GetXcontrollerLStickLeftPress(int number);
	static bool	GetXcontrollerLStickUpTrigger(int number);
	static bool	GetXcontrollerLStickDownTrigger(int number);
	static bool	GetXcontrollerLStickRightTrigger(int number);
	static bool	GetXcontrollerLStickLeftTrigger(int number);
	static bool	GetXcontrollerLStickUpRelease(int number);
	static bool	GetXcontrollerLStickDownRelease(int number);
	static bool	GetXcontrollerLStickRightRelease(int number);
	static bool	GetXcontrollerLStickLeftRelease(int number);
	static double	GetXcontrollerRStickX(int number);
	static double	GetXcontrollerRStickY(int number);
	static double	GetXcontrollerLTrigger(int number);
	static double	GetXcontrollerRTrigger(int number);
	static bool	GetXcontrollerButtonPress(int number, WORD button);
	static bool	GetXcontrollerButtonTrigger(int number, WORD button);
	static bool	GetXcontrollerButtonRelease(int number, WORD button);
	static bool	GetConnectController(int number);
};

#endif// !_XCONTROLLER_H_