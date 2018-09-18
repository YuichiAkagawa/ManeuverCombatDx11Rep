//**-------------------------------------------------------**
//**
//**                  vertex_shader.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include <d3dcompiler.h>
#include "main.h"
#include "renderer.h"
#include "vertex_shader.h"

#if defined(DEBUG) || defined(_DEBUG)
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

bool VertexShader::Compile()
{
	//シェーダコンパイル
	ID3DBlob* pError = nullptr;
	HRESULT hr = D3DCompileFromFile(
		std::wstring(fileName_.begin(), fileName_.end()).c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS",
		"vs_5_0",
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

bool VertexShader::Create()
{
	//シェーダ生成
	HRESULT hr = Renderer::GetDevice()->CreateVertexShader(
		pCompiledShader_->GetBufferPointer(),
		pCompiledShader_->GetBufferSize(),
		nullptr,
		&pVertexShader_);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void VertexShader::Uninit()
{
	SafeRelease(pVertexShader_);
	SafeRelease(pInputLayout_);
}

ID3D11VertexShader* VertexShader::GetVertexShader()
{
	return pVertexShader_;
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return pInputLayout_;
}