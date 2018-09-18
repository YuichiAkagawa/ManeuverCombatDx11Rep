//**-------------------------------------------------------**
//**
//**                  pixel_shader.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include <d3dcompiler.h>
#include "main.h"
#include "renderer.h"
#include "pixel_shader.h"

#if defined(DEBUG) || defined(_DEBUG)
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

bool PixelShader::Compile()
{
	//シェーダコンパイル
	ID3DBlob* pError = nullptr;
	HRESULT hr = D3DCompileFromFile(
		std::wstring(fileName_.begin(), fileName_.end()).c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS",
		"ps_5_0",
		SHADER_FLAGS,
		0,
		&pCompiledShader_,
		&pError);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool PixelShader::Create()
{
	//シェーダ生成
	HRESULT hr = Renderer::GetDevice()->CreatePixelShader(
		pCompiledShader_->GetBufferPointer(),
		pCompiledShader_->GetBufferSize(),
		nullptr,
		&pPixelShader_);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void PixelShader::Uninit()
{
	SafeRelease(pPixelShader_);
}

ID3D11PixelShader* PixelShader::GetPixelShader()
{
	return pPixelShader_;
}
