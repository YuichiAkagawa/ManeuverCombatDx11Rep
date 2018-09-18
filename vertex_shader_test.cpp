//**-------------------------------------------------------**
//**
//**                vertex_shader_test.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include "renderer.h"
#include "vertex_shader.h"
#include "vertex_shader_test.h"

static const std::string FILE_NAME = "test.hlsl";

bool VertexShaderTest::Init()
{
	fileName_ = FILE_NAME;

	return true;
}

bool VertexShaderTest::InputLayout()
{
	//�C���v�b�g���C�A�E�g�쐬
	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElememts = sizeof(desc) / sizeof(desc[0]);
	HRESULT hr = Renderer::GetDevice()->CreateInputLayout(
		desc,
		numElememts,
		pCompiledShader_->GetBufferPointer(),
		pCompiledShader_->GetBufferSize(),
		&pInputLayout_);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}