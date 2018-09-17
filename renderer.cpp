//**-------------------------------------------------------**
//**
//**                     renderer.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include "main.h"
#include "renderer.h"

#if defined(DEBUG) || defined(_DEBUG)
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

IDXGISwapChain*				Renderer::pSwapChain_;
ID3D11Device*				Renderer::pDevice_;
ID3D11DeviceContext*		Renderer::pDeviceContext_;
ID3D11RenderTargetView*		Renderer::pRenderTargetView_;
ID3D11Texture2D*			Renderer::pDepthStencilTexture_;
ID3D11DepthStencilView*		Renderer::pDepthStencilView_;
ID3D11VertexShader*			Renderer::pVertexShader_;
ID3D11PixelShader*			Renderer::pPixelShader_;
ID3D11InputLayout*			Renderer::pInputLayout_;
ID3D11Buffer*				Renderer::pVertexBuffer_;

bool Renderer::Init()
{
	//エラーチェック
	HRESULT hr;

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// リフレッシュレート 60fps 
	scd.BufferDesc.RefreshRate.Numerator = 60;		// 分子
	scd.BufferDesc.RefreshRate.Denominator = 1;		// 分母
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = GetHWND();
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	//スワップチェーン作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL, 
		0, 
		&FeatureLevel, 
		1, 
		D3D11_SDK_VERSION, 
		&scd, 
		&pSwapChain_, 
		&pDevice_, 
		NULL, 
		&pDeviceContext_);
	if (FAILED(hr))
	{
		return false;
	}
	
	//レンダーターゲットビューの作成 
	ID3D11Texture2D *pBackBuffer;
	pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);
	pBackBuffer->Release();

	//深度・ステンシルバッファの作成
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = SCREEN_WIDTH;
	texDesc.Height = SCREEN_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	hr = pDevice_->CreateTexture2D(
		&texDesc,
		nullptr,
		&pDepthStencilTexture_);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = texDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	hr = pDevice_->CreateDepthStencilView(
		pDepthStencilTexture_,
		&dsDesc,
		&pDepthStencilView_);
	if (FAILED(hr))
	{
		return false;
	}

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext_->RSSetViewports(1, &vp);

	//ラスタライザ
	D3D11_RASTERIZER_DESC rd = {};
	rd.CullMode = D3D11_CULL_BACK;
	rd.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rs = nullptr;
	pDevice_->CreateRasterizerState(&rd, &rs);
	pDeviceContext_->RSSetState(rs);

	//頂点シェーダ
	{
		//シェーダコンパイル
		std::string str = "test.hlsl";
		ID3DBlob* pCompiledShader = nullptr;
		ID3DBlob* pError = nullptr;
		hr = D3DCompileFromFile(
			std::wstring(str.begin(), str.end()).c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"VS",
			"vs_5_0",
			SHADER_FLAGS,
			0,
			&pCompiledShader,
			&pError);
		if (FAILED(hr))
		{
			return false;
		}

		//シェーダ生成
		hr = pDevice_->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&pVertexShader_);
		if (FAILED(hr))
		{
			return false;
		}

		//インプットレイアウト作成
		D3D11_INPUT_ELEMENT_DESC desc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },/*
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/
		};
		UINT numElememts = sizeof(desc) / sizeof(desc[0]);
		hr = pDevice_->CreateInputLayout(
			desc,
			numElememts,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&pInputLayout_);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//ピクセルシェーダ
	{
		//シェーダコンパイル
		std::string str = "test.hlsl";
		ID3DBlob* pCompiledShader = nullptr;
		ID3DBlob* pError = nullptr;
		
		hr = D3DCompileFromFile(
			std::wstring(str.begin(), str.end()).c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"PS",
			"ps_5_0",
			SHADER_FLAGS,
			0,
			&pCompiledShader,
			&pError);
		if (FAILED(hr))
		{
			return false;
		}

		//シェーダ生成
		hr = pDevice_->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&pPixelShader_);
		if (FAILED(hr))
		{
			return false;
		}
	}

	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth = sizeof(VERTEX2D) * 3;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		VERTEX2D v2d[3];
		v2d[0].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		v2d[1].pos = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
		v2d[2].pos = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = v2d;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		hr = pDevice_->CreateBuffer(
			&vertexDesc,
			&sd,
			&pVertexBuffer_);
		if (FAILED(hr))
		{
			return false;
		}
	}



	return true;
}

void Renderer::Uninit()
{
	pDeviceContext_->ClearState();
	SafeRelease(pSwapChain_);
	SafeRelease(pDevice_);
	SafeRelease(pRenderTargetView_);

	SafeRelease(pVertexShader_);
	SafeRelease(pPixelShader_);
	SafeRelease(pInputLayout_);
	SafeRelease(pVertexBuffer_);
}

void Renderer::DrawBegin()
{
	//レンダーターゲット指定
	pDeviceContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView_);
	
	//レンダーターゲットクリア
	pDeviceContext_->ClearRenderTargetView(pRenderTargetView_, SCREEN_COLOR);
	pDeviceContext_->ClearDepthStencilView(pDepthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//インプットレイアウト指定
	pDeviceContext_->IASetInputLayout(pInputLayout_);

	//シェーダセット
	pDeviceContext_->VSSetShader(pVertexShader_, nullptr, 0);
	pDeviceContext_->PSSetShader(pPixelShader_, nullptr, 0);

	UINT stride = sizeof(VERTEX2D);
	UINT offset = 0;

	//頂点バッファセット
	pDeviceContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//描画方法
	pDeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pDeviceContext_->Draw(3, 0);
}

void Renderer::DrawEnd()
{
	//レンダリング結果表示
	pSwapChain_->Present(0, 0);
}