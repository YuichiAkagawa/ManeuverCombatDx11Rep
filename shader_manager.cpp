//**-------------------------------------------------------**
//**
//**                 shader_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include "main.h"
#include "renderer.h"
#include "shader.h"
#include "shader_cube.h"
#include "shader_fbx.h"
#include "shader_skydome.h"
#include "shader_test.h"
#include "shader_manager.h"

Shader* ShaderManager::pShader_[MAX];

bool ShaderManager::Create()
{
	pShader_[CUBE] = new ShaderCube;
	pShader_[TEST] = new ShaderTest;
	pShader_[FBX] = new ShaderFbx;
	pShader_[SKYDOME] = new ShaderSkydome;

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
		if (!pShader_[i]->Init())
		{
			return false;
		}

		if (!pShader_[i]->CompileVS())
		{
			return false;
		}

		if (!pShader_[i]->CompilePS())
		{
			return false;
		}

		if (!pShader_[i]->CreateVS())
		{
			return false;
		}

		if (!pShader_[i]->CreatePS())
		{
			return false;
		}

		if (!pShader_[i]->InputLayout())
		{
			return false;
		}

		if (!pShader_[i]->CreateConstantBuffer())
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
		pShader_[i]->Uninit();
		SafeDelete(pShader_[i]);
	}
}

ID3D11VertexShader* ShaderManager::GetVertexShader(int num)
{
	return pShader_[num]->GetVertexShader();
}

ID3D11InputLayout* ShaderManager::GetInputLayout(int num)
{
	return pShader_[num]->GetInputLayout();
}

ID3D11PixelShader* ShaderManager::GetPixelShader(int num)
{
	return pShader_[num]->GetPixelShader();
}

ID3D11Buffer* const* ShaderManager::GetConstantBuffer(int num)
{
	return pShader_[num]->GetConstantBuffer();
}
