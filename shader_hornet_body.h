//**-------------------------------------------------------**
//**
//**                 shader_hornet_body.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _SHADER_HORNET_BODY_H_
#define _SHADER_HORNET_BODY_H_

#include <DirectXMath.h>

#include "shader.h"

class ShaderHornetBody : public Shader
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
	};

	bool Init()override;
	bool InputLayout()override;
	bool CreateConstantBuffer()override;
};

#endif // !_SHADER_HORNET_BODY_H_
