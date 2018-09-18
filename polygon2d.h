//**-------------------------------------------------------**
//**
//**                     polygon2d.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_

#include <DirectXMath.h>

class Polygon2D
{
private:
	struct VERTEX2D
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	ID3D11Buffer*			pVertexBuffer_;
	ID3D11Buffer*			pIndexBuffer_;

public:
	bool Init();
	void Uninit();
	void Draw();
};

#endif // !_POLYGON2D_H_
