//**-------------------------------------------------------**
//**
//**                     main.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>

constexpr UINT SCREEN_WIDTH = 1280;
constexpr UINT SCREEN_HEIGHT = 720;
static const float SCREEN_COLOR[] = { 0.2f, 0.2f, 0.2f, 1.0f };
constexpr bool WINDOW_MODE = true;

constexpr float	CAMERA_NEAR = 0.1f;
constexpr float	CAMERA_FAR = 200000.0f;
constexpr int	CAMERA_FOV = 90;

LRESULT CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool				Init(HINSTANCE hInstance, HWND hWnd);
void				Uninit();
void				Update();
void				Draw();
HWND				GetHWND();
void				WaitFrame();

template <typename T>
inline void SafeDelete(T*& p) {
	if (p != nullptr) {
		delete (p);
		(p) = nullptr;
	}
}

template <typename T>
inline void SafeDeleteArray(T*& p) {
	if (p != nullptr) {
		delete[](p);
		(p) = nullptr;
	}
}

template <typename T>
inline void SafeRelease(T*& p) {
	if (p != nullptr) {
		(p)->Release();
		(p) = nullptr;
	}
}

#endif // _MAIN_H_