//**-------------------------------------------------------**
//**
//**                  sampler_state.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SMAPLER_STATE_H_
#define _SMAPLER_STATE_H_

#include <d3d11.h>

class SamplerState
{
public:
	enum SAMPLER_TYPE
	{
		SAMPLER_TYPE_UNKNOWN = -1,
		SAMPLER_TYPE_01,
		SAMPLER_TYPE_MAX
	};

	static bool Init();
	static void Uninit();
	static ID3D11SamplerState* const* GetSamplerState(int samplerType);
private:
	static ID3D11SamplerState* pSamplerState_[SAMPLER_TYPE_MAX];
};

#endif // !_SMAPLER_STATE_H_
