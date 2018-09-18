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
	static ID3D11Texture2D*			pDepthStencilTexture_;
	static ID3D11DepthStencilView*	pDepthStencilView_;

	static ID3D11PixelShader*		pPixelShader_;

	static ID3D11Buffer*			pVertexBuffer_;
	static ID3D11Buffer*			pIndexBuffer_;
	static D3D11_VIEWPORT			viewport_;

public:
	struct VERTEX2D
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	static bool					Init();
	static void					Uninit();
	static void					DrawBegin();
	static void					DrawEnd();
	static ID3D11Device*		GetDevice();
};

#endif // !_RENDERER_H_