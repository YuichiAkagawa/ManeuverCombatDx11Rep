//**-------------------------------------------------------**
//**
//**                     main.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <crtdbg.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "imgui/imgui.h"
#include "imgui_manager.h"
#include "input.h"
#include "process_calculator.h"
#include "renderer.h"
#include "resource.h"
#include "scene_game01.h"
#include "scene_manager.h"

#include "main.h"

constexpr char*		CLASS_NAME = "ManeuverCombat";
constexpr char*		WINDOW_NAME = "ManeuverCombat";

LRESULT CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool				Init(HINSTANCE hInstance, int nCmdShow);
void				Uninit();
void				Update();
void				Draw();
void				WaitFrame();

static HWND							g_hWnd;									
static LARGE_INTEGER				g_freq;
static double						g_freqFrame;
static ProcessCalculator			g_allProcessCal, g_updateProcessCal, g_drawProcessCal;
static Scene*						g_pCurrentScene;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
#if defined(_DEBUG) || defined(DEBUG)
	//メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//初期化処理
	if (!Init(hInstance, nCmdShow))
	{
		MessageBox(g_hWnd, "初期化できませんでした", "Error", MB_OK);
		return -1;
	}

	//---GAME LOOP---//
	MSG msg;
	for (;;)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//全処理時間計測開始
			g_allProcessCal.StartCalculate();

			//更新処理時間計測開始
			g_updateProcessCal.StartCalculate();

			//--更新処理--//
			Update();

			//更新処理時間計測終了
			g_updateProcessCal.EndCalculate();

			//描画処理時間計測開始
			g_drawProcessCal.StartCalculate();

			//--描画処理--//
			Draw();

			//描画処理時間計測終了
			g_drawProcessCal.EndCalculate();

			//処理時間計測終了
			g_allProcessCal.EndCalculate();

			//フレーム待機
			WaitFrame();
		}
	}

	//終了処理
	Uninit();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//imguiウィンドウプロシージャ
	ImguiManager::ImguiWndProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
	{
		int nID = MessageBox(hWnd, "終了しますか？", "Warning", MB_YESNO | MB_ICONASTERISK | MB_DEFBUTTON2);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			int nID = MessageBox(hWnd, "終了しますか？", "Warning", MB_YESNO | MB_ICONASTERISK | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		default:
			break;
		}
		break;

	case WM_LBUTTONDOWN:
	{
		SetFocus(hWnd);
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Init(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCSTR)IDI_ICON1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = nullptr;

	RegisterClassEx(&wcex);

	RECT dw;
	int nWindowWidth = SCREEN_WIDTH;
	int	nWindowHeight = SCREEN_HEIGHT;
	DWORD dwWindowStyle;
	int nWindowTop, nWindowLeft;

	GetWindowRect(GetDesktopWindow(), &dw);
	if (((dw.bottom - nWindowHeight) > 0) && ((dw.right - nWindowWidth) > 0))
	{
		dwWindowStyle = WS_OVERLAPPEDWINDOW;
		RECT wr = { 0, 0, nWindowWidth, nWindowHeight };
		AdjustWindowRect(&wr, dwWindowStyle, false);
		nWindowWidth -= wr.left;
		nWindowHeight -= wr.top;
		nWindowTop = (dw.bottom - nWindowHeight) / 2;
		nWindowLeft = (dw.right - nWindowWidth) / 2;
	}
	else
	{
		dwWindowStyle = WS_POPUPWINDOW;
		nWindowTop = dw.top;
		nWindowLeft = dw.left;
		nWindowHeight = dw.bottom;
		nWindowWidth = dw.right;
	}

	g_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		dwWindowStyle,
		nWindowLeft,
		nWindowTop,
		nWindowWidth,
		nWindowHeight,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	//高精度タイマー単位取得
	QueryPerformanceFrequency(&g_freq);

	//待機時間計測用単位取得
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	g_freqFrame = freq / 1000.0f;

	//キーボード初期化処理
	HRESULT hr = InitKeyboard(hInstance, g_hWnd);
	if (FAILED(hr))
	{
		return false;
	}

	//レンダラー初期化
	if (!Renderer::Init())
	{
		return false;
	}
	
	//DirectXMath対応チェック
	if (!DirectX::XMVerifyCPUSupport())
	{
		return false;
	}

	//ImGui初期化
	ImguiManager::Init(g_hWnd);

	//現在のシーン初期化
	g_pCurrentScene = nullptr;

	//シーン初期化処理
	if (!SceneManager::SetScene(new SceneGame01))
	{
		return false;
	}

	return true;
}

void Uninit()
{
	//ImGui終了処理
	ImguiManager::Uninit();

	//シーン終了処理
	SceneManager::Uninit();

	//キーボード終了処理
	UninitKeyboard();

	//レンダラー終了処理
	Renderer::Uninit();
}

void Update()
{
	//キーボード更新処理
	UpdateKeyboard();

	//imgui更新開始
	ImguiManager::UpdateStart();

	//ImGui
	if (ImguiManager::GetIsDraw())
	{
		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiSetCond_Once);

		ImGui::Begin("Stats");
		ImGui::Text("Application average %.3f ms / frame(%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Spacing();

		ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
		if (ImGui::CollapsingHeader("ProcessTime"))
		{
			ImGui::Text("ProcessTime : %.3f ms", g_allProcessCal.GetProcessTimeMS());
			ImGui::Text("UpdateProcessTime : %.3f ms", g_updateProcessCal.GetProcessTimeMS());
			ImGui::Text("DrawProcessTime : %.3f ms", g_drawProcessCal.GetProcessTimeMS());

			bool processTimeClear = false;
			ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
			if (ImGui::TreeNode("ProcessTimeMax"))
			{
				ImGui::Text("ProcessTimeMax : %.3f ms", g_allProcessCal.GetProcessTimeMaxMS());
				ImGui::Text("UpdateProcessTimeMax : %.3f ms", g_updateProcessCal.GetProcessTimeMaxMS());
				ImGui::Text("DrawProcessTimeMax : %.3f ms", g_drawProcessCal.GetProcessTimeMaxMS());
				processTimeClear = ImGui::Button("Reset");
				ImGui::TreePop();
			}

			//シーンが遷移したかどうか
			//リセットボタンが押されたかどうか
			if (SceneManager::GetScene() != g_pCurrentScene || processTimeClear)
			{
				//処理時間計測値初期化
				g_allProcessCal.Reset();
				g_updateProcessCal.Reset();
				g_drawProcessCal.Reset();
			}
			g_pCurrentScene = SceneManager::GetScene();
		}
		ImGui::End();
	}

	//シーン更新処理
	SceneManager::Update();

	//Imgui更新終了
	ImguiManager::UpdateEnd();
}

void Draw()
{
	//レンダラー描画開始処理
	Renderer::DrawBegin();

	//シーン描画
	SceneManager::Draw();

	//imgui描画
	if (ImguiManager::GetIsDraw())
	{
		ImguiManager::Draw();
	}

	//レンダラー描画終了処理
	Renderer::DrawEnd();
}

void WaitFrame()
{
	static __int64 currentTime = 0;
	static __int64 lastTime = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	const double WAIT_TIME = (1000.0f / FRAME_RATE) - (currentTime - lastTime) / g_freqFrame;

	do {
		Sleep(0);
		QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	} while ((lastTime - currentTime) / g_freqFrame < WAIT_TIME);
}

HWND GetHWND()
{
	return g_hWnd;
}