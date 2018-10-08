//**-------------------------------------------------------**
//**
//**                 shader_sea_body.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SHADER_SEA_H_
#define _SHADER_SEA_H_

#include <DirectXMath.h>

#include "shader.h"

class ShaderSea : public Shader
{
public:
	struct CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4 mtxWorld;
		DirectX::XMFLOAT4X4 mtxWorldInv;
		DirectX::XMFLOAT4X4 mtxView;
		DirectX::XMFLOAT4X4 mtxProj;
		DirectX::XMFLOAT4 vecDirLight;
		DirectX::XMFLOAT4 posEye;
		DirectX::XMFLOAT4 specularData;
		DirectX::XMFLOAT4 time;
	};

	bool Init()override;
	bool InputLayout()override;
	bool CreateConstantBuffer()override;
};

#endif // !_SHADER_SEA_H_
