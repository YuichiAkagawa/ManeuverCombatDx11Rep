//**-------------------------------------------------------**
//**
//**                     renderer.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <DirectXMath.h>

class Renderer
{
private:
	static IDXGISwapChain*			pSwapChain_;
	static ID3D11Device*			pDevice_;
	static ID3D11DeviceContext*		pDeviceContext_;
	static ID3D11RenderTargetView*	pRenderTargetView_;
	static ID3D11VertexShader*		pVertexShader_;
	static ID3D11PixelShader*		pPixelShader_;
	static ID3D11InputLayout*		pInputLayout_;
	static ID3D11Buffer*			pVertexBuffer_;

public:
	struct VERTEX2D
	{
		DirectX::XMFLOAT3 pos;
	};

	static bool					Init();
	static void					Uninit();
	static void					DrawBegin();
	static void					DrawEnd();
};

#endif // !_RENDERER_H_