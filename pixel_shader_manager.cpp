//**-------------------------------------------------------**
//**
//**                pixel_shader_manager.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include "main.h"
#include "pixel_shader.h"
#include "pixel_shader_test.h"
#include "pixel_shader_manager.h"

PixelShader* PixelShaderManager::pPixelShader_[PS_MAX];

bool PixelShaderManager::Create()
{
	pPixelShader_[PS_TEST] = new PixelShaderTest;

	return true;
}

bool PixelShaderManager::Init()
{
	if (!Create())
	{
		return false;
	}

	for (int i = 0; i < PS_MAX; i++)
	{
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

void PixelShaderManager::Uninit()
{
	for (int i = 0; i < PS_MAX; i++)
	{
		pPixelShader_[i]->Uninit();
		SafeDelete(pPixelShader_[i]);
	}
}

ID3D11PixelShader* PixelShaderManager::GetPixelShader(int numPS)
{
	return pPixelShader_[numPS]->GetPixelShader();
}
