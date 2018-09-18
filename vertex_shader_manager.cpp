//**-------------------------------------------------------**
//**
//**                vertex_shader_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include <d3d11.h>
#include "main.h"
#include "vertex_shader.h"
#include "vertex_shader_test.h"
#include "vertex_shader_manager.h"

VertexShader* VertexShaderManager::pVertexShader_[VS_MAX];

bool VertexShaderManager::Create()
{
	pVertexShader_[VS_TEST] = new VertexShaderTest;

	return true;
}

bool VertexShaderManager::Init()
{
	if (!Create())
	{
		return false;
	}

	for (int i = 0; i < VS_MAX; i++)
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
	}
	return true;
}

void VertexShaderManager::Uninit()
{
	for (int i = 0; i < VS_MAX; i++)
	{
		pVertexShader_[i]->Uninit();
		SafeDelete(pVertexShader_[i]);
	}
}

ID3D11VertexShader* VertexShaderManager::GetVertexShader(int numVS)
{
	return pVertexShader_[numVS]->GetVertexShader();
}

ID3D11InputLayout* VertexShaderManager::GetInputLayout(int numVS)
{
	return pVertexShader_[numVS]->GetInputLayout();
}