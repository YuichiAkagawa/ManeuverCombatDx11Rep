//**-------------------------------------------------------**
//**
//**                  sampler_state.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include "main.h"
#include "renderer.h"
#include "sampler_state.h"

ID3D11SamplerState* SamplerState::pSamplerState_[SamplerState::SAMPLER_TYPE_MAX];

bool SamplerState::Init()
{
	//テクスチャサンプラー01
	{
		D3D11_SAMPLER_DESC smpDesc;

		::ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
		smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		HRESULT hr = Renderer::GetDevice()->CreateSamplerState(&smpDesc, &pSamplerState_[SAMPLER_TYPE_01]);
		if (FAILED(hr))
		{
			return false;
		}
	}
	return true;
}

void SamplerState::Uninit()
{
	for (int i = 0; i < SAMPLER_TYPE_MAX; i++)
	{
		SafeRelease(pSamplerState_[i]);
	}
}

ID3D11SamplerState* const* SamplerState::GetSamplerState(int samplerType)
{
	return &pSamplerState_[samplerType];
}