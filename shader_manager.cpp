//**-------------------------------------------------------**
//**
//**                 shader_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include <d3d11.h>
#include "main.h"
#include "vertex_shader.h"
#include "vertex_shader_test.h"
#include "pixel_shader.h"
#include "pixel_shader_test.h"
#include "shader_manager.h"

VertexShader* ShaderManager::pVertexShader_[MAX];
PixelShader* ShaderManager::pPixelShader_[MAX];

bool ShaderManager::Create()
{
	pVertexShader_[TEST] = new VertexShaderTest;
	pPixelShader_[TEST] = new PixelShaderTest;

	return true;
}

bool ShaderManager::Init()
{
	if (!Create())
	{
		return false;
	}

	for (int i = 0; i < MAX; i++)
	{
		if (!pVertexShader_[i]->Init())
		{
			return false;
		}

		if (!pVertexShader_[i]->Compile())
		{
			return false;
		}

		if (!pVertexShader_[i]->Create())
		{
			return false;
		}

		if (!pVertexShader_[i]->InputLayout())
		{
			return false;
		}

		if (!pPixelShader_[i]->Init())
		{
			return false;
		}

		if (!pPixelShader_[i]->Compile())
		{
			return false;
		}

		if (!pPixelShader_[i]->Create())
		{
			return false;
		}
	}
	return true;
}

void ShaderManager::Uninit()
{
	for (int i = 0; i < MAX; i++)
	{
		pVertexShader_[i]->Uninit();
		SafeDelete(pVertexShader_[i]);

		pPixelShader_[i]->Uninit();
		SafeDelete(pPixelShader_[i]);
	}
}

ID3D11VertexShader* ShaderManager::GetVertexShader(int num)
{
	return pVertexShader_[num]->GetVertexShader();
}

ID3D11InputLayout* ShaderManager::GetInputLayout(int num)
{
	return pVertexShader_[num]->GetInputLayout();
}

ID3D11PixelShader* ShaderManager::GetPixelShader(int num)
{
	return pPixelShader_[num]->GetPixelShader();
}
