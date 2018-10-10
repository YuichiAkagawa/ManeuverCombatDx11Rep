//**-------------------------------------------------------**
//**
//**                  xcontroller.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _XCONTROLLER_H_
#define _XCONTROLLER_H_

#include <Xinput.h>

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

	static XCONT_BUTTON_STATUS buttonStatus_;
	static XCONT_BUTTON_STATUS buttonStatusOld_;

	static XINPUT_STATE controllerState_;
	static bool connected_;
	static bool isDrawImgui_;

	static void	XcontrollerValue(bool* isDrawImgui);

public:
	static void	InitXcontroller();
	static void	UninitXcontroller();
	static void	UpdateXcontroller();
	static void	UpdateXcontrollerCreateOld();
	static void DrawImgui();
	
	static double	GetXcontrollerLStickX();
	static double	GetXcontrollerLStickY();
	static bool	GetXcontrollerLStickUpPress();
	static bool	GetXcontrollerLStickDownPress();
	static bool	GetXcontrollerLStickRightPress();
	static bool	GetXcontrollerLStickLeftPress();
	static bool	GetXcontrollerLStickUpTrigger();
	static bool	GetXcontrollerLStickDownTrigger();
	static bool	GetXcontrollerLStickRightTrigger();
	static bool	GetXcontrollerLStickLeftTrigger();
	static bool	GetXcontrollerLStickUpRelease();
	static bool	GetXcontrollerLStickDownRelease();
	static bool	GetXcontrollerLStickRightRelease();
	static bool	GetXcontrollerLStickLeftRelease();
	static double	GetXcontrollerRStickX();
	static double	GetXcontrollerRStickY();
	static double	GetXcontrollerLTrigger();
	static double	GetXcontrollerRTrigger();
	static bool	GetXcontrollerButtonPress(WORD button);
	static bool	GetXcontrollerButtonTrigger(WORD button);
	static bool	GetXcontrollerButtonRelease(WORD button);
	static bool	GetConnectController();
};

#endif// !_XCONTROLLER_H_