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
	};

	ID3D11Buffer*			pVertexBuffer_;
	ID3D11Buffer*			pIndexBuffer_;
public:
	bool Init();
	void Uninit();
	void Draw();
};

#endif // !_CUBE_H_
