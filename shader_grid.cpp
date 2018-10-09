//**-------------------------------------------------------**
//**
//**                  shader_grid.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include "renderer.h"
#include "shader.h"
#include "shader_grid.h"

static const std::string FILE_NAME = "grid.hlsl";

bool ShaderGrid::Init()
{
	fileName_ = FILE_NAME;

	return true;
}

bool ShaderGrid::InputLayout()
{
	//インプットレイアウト作成
	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElememts = sizeof(desc) / sizeof(desc[0]);
	HRESULT hr = Renderer::GetDevice()->CreateInputLayout(
		desc,
		numElememts,
		pCompiledVertexShader_->GetBufferPointer(),
		pCompiledVertexShader_->GetBufferSize(),
		&pInputLayout_);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool ShaderGrid::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(CONSTANT_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = Renderer::GetDevice()->CreateBuffer(&cbDesc, NULL, &pConstantBuffer_);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
