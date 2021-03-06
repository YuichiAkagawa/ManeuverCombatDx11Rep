//**-------------------------------------------------------**
//**
//**                     shader.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include <d3dcompiler.h>
#include "main.h"
#include "renderer.h"
#include "shader.h"

#if defined(DEBUG) || defined(_DEBUG)
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

bool Shader::CompileVS()
{
	{
		//頂点シェーダコンパイル
		ID3DBlob* pError = nullptr;
		HRESULT hr = D3DCompileFromFile(
			std::wstring(fileName_.begin(), fileName_.end()).c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"VS",
			"vs_5_0",
			SHADER_FLAGS,
			0,
			&pCompiledVertexShader_,
			&pError);
		if (FAILED(hr))
		{
			return false;
		}
	}
	return true;
}

bool Shader::CompilePS()
{
	{
		//ピクセルシェーダコンパイル
		ID3DBlob* pError = nullptr;
		HRESULT hr = D3DCompileFromFile(
			std::wstring(fileName_.begin(), fileName_.end()).c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"PS",
			"ps_5_0",
			SHADER_FLAGS,
			0,
			&pCompiledPixelShader_,
			&pError);
		if (FAILED(hr))
		{
			return false;
		}
	}
	return true;
}

bool Shader::CreateVS()
{
	{
		//頂点シェーダ生成
		HRESULT hr = Renderer::GetDevice()->CreateVertexShader(
			pCompiledVertexShader_->GetBufferPointer(),
			pCompiledVertexShader_->GetBufferSize(),
			nullptr,
			&pVertexShader_);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

bool Shader::CreatePS()
{
	{
		//シェーダ生成
		HRESULT hr = Renderer::GetDevice()->CreatePixelShader(
			pCompiledPixelShader_->GetBufferPointer(),
			pCompiledPixelShader_->GetBufferSize(),
			nullptr,
			&pPixelShader_);
		if (FAILED(hr))
		{
			return false;
		}
	}
	return true;
}

void Shader::Uninit()
{
	SafeRelease(pVertexShader_);
	SafeRelease(pCompiledVertexShader_);
	SafeRelease(pInputLayout_);
	SafeRelease(pPixelShader_);
	SafeRelease(pCompiledPixelShader_);
	SafeRelease(pConstantBuffer_);
}

ID3D11VertexShader* Shader::GetVertexShader()
{
	return pVertexShader_;
}

ID3D11InputLayout* Shader::GetInputLayout()
{
	return pInputLayout_;
}

ID3D11PixelShader* Shader::GetPixelShader()
{
	return pPixelShader_;
}

ID3D11Buffer* const* Shader::GetConstantBuffer()
{
	return &pConstantBuffer_;
}