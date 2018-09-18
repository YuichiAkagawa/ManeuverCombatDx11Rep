//**-------------------------------------------------------**
//**
//**                     cube.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _CUBE_H_
#define _CUBE_H_

#include <DirectXMath.h>

class Cube
{
private:
	struct VERTEX3D
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	ID3D11Buffer*			pVertexBuffer_;
	ID3D11Buffer*			pIndexBuffer_;

	ID3D11ShaderResourceView *pShaderResourceView_ = nullptr;
	ID3D11Resource *pResource_ = nullptr;
	ID3D11SamplerState* pSamplerState_;
public:
	bool Init();
	void Uninit();
	void Draw();
};

#endif // !_CUBE_H_
