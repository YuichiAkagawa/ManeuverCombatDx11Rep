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
	static void Addition(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static void Subtraction(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static void Multiplication(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& valueLeft, float valueRight);
	static void Multiplication(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static void Division(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);

	static void Normalize(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& value);
	static float Dot(const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	static void Cross(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& valueLeft, const DirectX::XMFLOAT3& valueRight);
	
	static void Transform(DirectX::XMFLOAT3& valueReturn, const DirectX::XMFLOAT3& value, const DirectX::XMFLOAT4X4& mtx);

	static void Identity(DirectX::XMFLOAT4X4& valueReturn);
	static void Inverse(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT4X4& value);
	static void Transpose(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT4X4& value);
	static void LookAtLH(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& posAt, const DirectX::XMFLOAT3& vecUp);
	static void PerspectiveFovLH(DirectX::XMFLOAT4X4& valueReturn, float fov, float aspect, float nearZ, float farZ);
	static void Scaling(DirectX::XMFLOAT4X4& valueReturn, float value);
	static void Scaling(DirectX::XMFLOAT4X4& valueReturn, float x, float y, float z);
	static void Scaling(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT3& value);
	static void RotationX(DirectX::XMFLOAT4X4& valueReturn, float angle);
	static void RotationY(DirectX::XMFLOAT4X4& valueReturn, float angle);
	static void RotationZ(DirectX::XMFLOAT4X4& valueReturn, float angle);
	static void RotationAxis(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT3& value, float angle);
	static void RotationQuaternion(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT3& value, float angle);
	static void Translation(DirectX::XMFLOAT4X4& valueReturn, float x, float y, float z);
	static void Translation(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT3& value);
	static void Multiplication(DirectX::XMFLOAT4X4& valueReturn, const DirectX::XMFLOAT4X4& valueLeft, const DirectX::XMFLOAT4X4& valueRight);
};

#endif // !_EDIT_MATH_H_
