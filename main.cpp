//**-------------------------------------------------------**
//**
//**                     main.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <crtdbg.h>
#include <d3d11.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"

#include "cube.h"
#include "polygon2d.h"
#include "renderer.h"
#include "resource.h"
#include "input.h"
#include "main.h"

constexpr char*		CLASS_NAME = "ManeuverCombat";
constexpr char*		WINDOW_NAME = "ManeuverCombat";
constexpr double	FRAME_DIRAY = 1000.0f / 60.0f;

static HWND							g_hWnd;									
static LARGE_INTEGER				g_freq;
static double						g_freqFrame;
static Polygon2D*					g_pPolygon2d;
static Cube*						g_pCube;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
#if defined(_DEBUG) || defined(DEBUG)
	//メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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

	//初期化処理
	MSG msg;
	if (!Init(hInstance, g_hWnd))
	{
		MessageBox(g_hWnd, "初期化できませんでした", "Error", MB_OK);
		return -1;
	}

	//高精度タイマー単位取得
	QueryPerformanceFrequency(&g_freq);

	//待機時間計測用単位取得
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	g_freqFrame = freq / 1000.0f;

	//---GAME LOOP---//
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

			//--更新処理--//
			Update();

			//--描画処理--//
			Draw();

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
	//imguiウィンドウプロシージャハンドラー
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

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

bool Init(HINSTANCE hInstance, HWND hWnd)
{
	//キーボード初期化処理
	HRESULT hr = InitKeyboard(hInstance, hWnd);
	if (FAILED(hr))
	{
		return false;
	}

	//レンダラー初期化
	if (!Renderer::Init())
	{
		return false;
	}

	//ImGuiセットアップ
	ImGuiSetup();

	//ポリゴン生成
	g_pPolygon2d = new Polygon2D;
	if (!g_pPolygon2d->Init())
	{
		return false;
	}

	//キューブ生成
	g_pCube = new Cube;
	if (!g_pCube->Init())
	{
		return false;
	}

	return true;
}

void Uninit()
{
	//imgui終了処理
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	//ポリゴン破棄
	g_pPolygon2d->Uninit();
	SafeDelete(g_pPolygon2d);

	//キューブ破棄
	g_pCube->Uninit();
	SafeDelete(g_pCube);

	//キーボード終了処理
	UninitKeyboard();

	//レンダラー終了処理
	Renderer::Uninit();
}

void Update()
{
	//imgui更新
	ImGui_ImplDX11_NewFrame();

	//キーボード更新処理
	UpdateKeyboard();

	//imgui
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(380, 450), ImGuiSetCond_Once);

	ImGui::Begin("Stats");
	ImGui::Text("Application average %.3f ms / frame(%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();

	//imgui更新終了
	ImGui::EndFrame();
}

void Draw()
{
	//レンダラー描画開始処理
	Renderer::DrawBegin();

	//キューブ描画
	g_pCube->Draw();

	//ポリゴン描画
	//g_pPolygon2d->Draw();

	//ImGui描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//レンダラー描画終了処理
	Renderer::DrawEnd();
}

HWND GetHWND()
{
	return g_hWnd;
}

void WaitFrame()
{
	static __int64 currentTime = 0;
	static __int64 lastTime = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	const double WAIT_TIME = FRAME_DIRAY - (currentTime - lastTime) / g_freqFrame;

	do {
		Sleep(0);
		QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	} while ((lastTime - currentTime) / g_freqFrame < WAIT_TIME);
}

void ImGuiSetup()
{
	//imguiセットアップ
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplDX11_Init(g_hWnd, Renderer::GetDevice(), Renderer::GetDeviceContext());

	//imguiスタイルセットアップ
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
}