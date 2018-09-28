//**-------------------------------------------------------**
//**
//**                    imgui_manager.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _IMGUI_MANAGER_H_
#define _IMGUI_MANAGER_H_

class ImguiManager
{
private:
	static bool isDraw_;
public:
	static bool ImguiWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void Init(HWND hWnd);
	static void Uninit();
	static void UpdateStart();
	static void UpdateEnd();
	static void Draw();
	static bool GetIsDraw();
};

#endif // !_IMGUI_MANAGER_H_
