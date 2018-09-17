//**-------------------------------------------------------**
//**
//**                     main.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <crtdbg.h>
#include <d3d11.h>
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
#if defined(_DEBUG) || defined(DEBUG)
	//���������[�N�`�F�b�N
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

	//����������
	MSG msg;
	if (!Init(hInstance, g_hWnd))
	{
		MessageBox(g_hWnd, "�������ł��܂���ł���", "Error", MB_OK);
		return -1;
	}

	//�����x�^�C�}�[�P�ʎ擾
	QueryPerformanceFrequency(&g_freq);

	//�ҋ@���Ԍv���p�P�ʎ擾
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

			//--�X�V����--//
			Update();

			//--�`�揈��--//
			Draw();

			//�t���[���ҋ@
			WaitFrame();
		}
	}

	//�I������
	Uninit();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
	{
		int nID = MessageBox(hWnd, "�I�����܂����H", "Warning", MB_YESNO | MB_ICONASTERISK | MB_DEFBUTTON2);
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
			int nID = MessageBox(hWnd, "�I�����܂����H", "Warning", MB_YESNO | MB_ICONASTERISK | MB_DEFBUTTON2);
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
	//�L�[�{�[�h����������
	HRESULT hr = InitKeyboard(hInstance, hWnd);
	if (FAILED(hr))
	{
		return false;
	}

	//�����_���[������
	if (!Renderer::Init())
	{
		return false;
	}

	return true;
}

void Uninit()
{
	//�L�[�{�[�h�I������
	UninitKeyboard();

	//�����_���[�I������
	Renderer::Uninit();
}

void Update()
{
	//�L�[�{�[�h�X�V����
	UpdateKeyboard();
}

void Draw()
{
	//�����_���[�`��J�n����
	Renderer::DrawBegin();

	//�����_���[�`��I������
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