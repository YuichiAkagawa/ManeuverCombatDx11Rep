//**-------------------------------------------------------**
//**
//**                    edit_math.h
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#ifndef _EDIT_MATH_H_
#define _EDIT_MATH_H_

#include <DirectXMath.h>

class EditMath
{
public:
	static DirectX::XMFLOAT3 Addition(const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static DirectX::XMFLOAT3 Subtraction(const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static DirectX::XMFLOAT3 Multiplication(const DirectX::XMFLOAT3& valueLeft, float valueRight);
	static DirectX::XMFLOAT3 Multiplication(const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static DirectX::XMFLOAT3 Division(const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);

	static DirectX::XMFLOAT3 Normalize(const DirectX::XMFLOAT3& value);
	static float Dot(const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static DirectX::XMFLOAT3 Cross(const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	
	static DirectX::XMFLOAT3 Transform(const DirectX::XMFLOAT3& vec, const DirectX::XMFLOAT4X4& mtx);

	static DirectX::XMFLOAT4X4 Identity();
	static DirectX::XMFLOAT4X4 Transpose(const DirectX::XMFLOAT4X4& mtx);
	static DirectX::XMFLOAT4X4 LookAtLH(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& posAt, const DirectX::XMFLOAT3& vecUp);
	static DirectX::XMFLOAT4X4 PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ);
	static DirectX::XMFLOAT4X4 Scaling(float x, float y, float z);
	static DirectX::XMFLOAT4X4 RotationX(float angle);
	static DirectX::XMFLOAT4X4 RotationY(float angle);
	static DirectX::XMFLOAT4X4 RotationZ(float angle);
	static DirectX::XMFLOAT4X4 RotationAxis(const DirectX::XMFLOAT3& vec, float angle);
	static DirectX::XMFLOAT4X4 Translation(float x, float y, float z);
	static DirectX::XMFLOAT4X4 Translation(const DirectX::XMFLOAT3& value);
	static DirectX::XMFLOAT4X4 Multiplication(const DirectX::XMFLOAT4X4& valueLeft, const DirectX::XMFLOAT4X4& valueRight);
};

#endif // !_EDIT_MATH_H_
