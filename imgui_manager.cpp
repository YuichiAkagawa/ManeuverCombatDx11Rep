//**-------------------------------------------------------**
//**
//**                    imgui_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <Windows.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "input.h"
#include "renderer.h"
#include "imgui_manager.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool ImguiManager::isDraw_ = false;

bool ImguiManager::ImguiWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//imguiウィンドウプロシージャハンドラー
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ImguiManager::Init(HWND hWnd)
{
	//imguiセットアップ
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplDX11_Init(hWnd, Renderer::GetDevice(), Renderer::GetDeviceContext());

	//imguiスタイルセットアップ
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
}

void ImguiManager::Uninit()
{
	//imgui終了処理
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::UpdateStart()
{
	//imgui更新
	ImGui_ImplDX11_NewFrame();

	//imgui描画切り替え
	if (GetKeyboardTrigger(DIK_TAB))
	{
		if (isDraw_)
		{
			isDraw_ = false;
		}
		else
		{
			isDraw_ = true;
		}
	}
}

void ImguiManager::UpdateEnd()
{
	//imgui更新終了
	ImGui::EndFrame();
}

void ImguiManager::Draw()
{
	//描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool ImguiManager::GetIsDraw()
{
	return isDraw_;
}