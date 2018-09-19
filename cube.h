//**-------------------------------------------------------**
//**
//**                     cube.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _CUBE_H_
#define _CUBE_H_

#include <d3d11.h>
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
public:
	bool Init();
	void Uninit();
	void Draw(TextureManager* pTextureManager);
};

#endif // !_CUBE_H_
