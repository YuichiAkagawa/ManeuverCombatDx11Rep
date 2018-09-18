//**-------------------------------------------------------**
//**
//**                     renderer.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _RENDERER_H_
#define _RENDERER_H_

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