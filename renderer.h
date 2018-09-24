//**-------------------------------------------------------**
//**
//**                     renderer.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>

static const DirectX::XMFLOAT3 DIRECTIONAL_LIGHT = { 0.0f, -1.0f, -0.5f };

class Renderer
{
private:
	static IDXGISwapChain*			pSwapChain_;
	static ID3D11Device*			pDevice_;
	static ID3D11DeviceContext*		pDeviceContext_;
	static ID3D11RenderTargetView*	pRenderTargetView_;
	static ID3D11Texture2D*			pDepthStencilTexture_;
	static ID3D11DepthStencilView*	pDepthStencilView_;
public:
	static bool					Init();
	static void					Uninit();
	static void					DrawBegin();
	static void					DrawEnd();
	static ID3D11Device*		GetDevice();
	static ID3D11DeviceContext*	GetDeviceContext();
};

#endif // !_RENDERER_H_