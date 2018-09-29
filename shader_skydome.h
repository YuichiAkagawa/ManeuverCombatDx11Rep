//**-------------------------------------------------------**
//**
//**                   shader_skydome.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SHADER_SKYDOME_H_
#define _SHADER_SKYDOME_H_

#include <DirectXMath.h>

#include "shader.h"

class ShaderSkydome : public Shader
{
public:
	struct CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4 mtxWorld;
		DirectX::XMFLOAT4X4 mtxView;
		DirectX::XMFLOAT4X4 mtxProj;
		DirectX::XMFLOAT4 colorValue;
	};

	bool Init()override;
	bool InputLayout()override;
	bool CreateConstantBuffer()override;
};

#endif // !_SHADER_SKYDOME_H_
