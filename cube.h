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
	struct VERTEX2D
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	struct CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4 mtxWorld;
		DirectX::XMFLOAT4X4 mtxView;
		DirectX::XMFLOAT4X4 mtxProj;
	};

	ID3D11Buffer*			pVertexBuffer_;
	ID3D11Buffer*			pIndexBuffer_;
	ID3D11Buffer*			pConstantBuffer_;
public:
	bool Init();
	void Uninit();
	void Draw();
};

#endif // !_CUBE_H_
