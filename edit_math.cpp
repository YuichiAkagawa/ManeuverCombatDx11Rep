//**-------------------------------------------------------**
//**
//**                    edit_math.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include <DirectXMath.h>
#include "edit_math.h"

using namespace DirectX;

XMFLOAT3 EditMath::Add(const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMFLOAT3 xmFloat3 = { valueLeft.x + valueRight.x, valueLeft.y + valueRight.y, valueLeft.z + valueRight.z };
	return xmFloat3;
}

XMFLOAT3 EditMath::Sub(const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMFLOAT3 xmFloat3 = { valueLeft.x - valueRight.x, valueLeft.y - valueRight.y, valueLeft.z - valueRight.z };
	return xmFloat3;
}

XMFLOAT3 EditMath::Mul(const DirectX::XMFLOAT3& valueLeft, float valueRight)
{
	XMFLOAT3 xmFloat3 = { valueLeft.x * valueRight, valueLeft.y * valueRight, valueLeft.z * valueRight };
	return xmFloat3;
}

XMFLOAT3 EditMath::Mul(const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMFLOAT3 xmFloat3 = { valueLeft.x * valueRight.x, valueLeft.y * valueRight.y, valueLeft.z * valueRight.z };
	return xmFloat3;
}

XMFLOAT3 EditMath::Div(const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMFLOAT3 xmFloat3 = { valueLeft.x / valueRight.x, valueLeft.y / valueRight.y, valueLeft.z / valueRight.z };
	return xmFloat3;
}

XMFLOAT3 EditMath::Normalize(const XMFLOAT3& value)
{
	XMVECTOR xmValue = XMLoadFloat3(&value);
	xmValue = XMVector3Normalize(xmValue);
	XMFLOAT3 valueReturn;
	XMStoreFloat3(&valueReturn, xmValue);
	return valueReturn;
}

float EditMath::Dot(const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMVECTOR xmValueLeft = XMLoadFloat3(&valueLeft);
	xmValueLeft = XMVector3Normalize(xmValueLeft);
	XMVECTOR xmValueRight = XMLoadFloat3(&valueRight);
	xmValueRight = XMVector3Normalize(xmValueRight);
	XMVECTOR xmInner = XMVector3Dot(xmValueLeft, xmValueRight);
	float inner = XMVectorGetX(xmInner);
	return inner;
}

XMFLOAT3 EditMath::Cross(const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMVECTOR xmValueLeft = XMLoadFloat3(&valueLeft);
	xmValueLeft = XMVector3Normalize(xmValueLeft);
	XMVECTOR xmValueRight = XMLoadFloat3(&valueRight);
	xmValueRight = XMVector3Normalize(xmValueRight);
	XMVECTOR xmOuter = XMVector3Cross(xmValueLeft, xmValueRight);
	XMFLOAT3 outer;
	XMStoreFloat3(&outer, xmOuter);
	return outer;
}

XMFLOAT3 EditMath::Transform(const XMFLOAT3& vec, const XMFLOAT4X4& mtx)
{
	XMVECTOR xmVec = XMLoadFloat3(&vec);
	XMMATRIX xmMtx = XMLoadFloat4x4(&mtx);
	XMVECTOR xmVecReturn = XMVector3Transform(xmVec, xmMtx);
	XMFLOAT3 vecReturn;
	XMStoreFloat3(&vecReturn, xmVecReturn);
	return vecReturn;
}

XMFLOAT4X4 EditMath::Identity()
{
	XMMATRIX xmMtx = XMMatrixIdentity();
	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, xmMtx);
	return mtx;
}

DirectX::XMFLOAT4X4 EditMath::Transpose(const DirectX::XMFLOAT4X4& mtx)
{
	XMMATRIX xmMtx = XMLoadFloat4x4(&mtx);
	xmMtx = XMMatrixTranspose(xmMtx);
	XMFLOAT4X4 mtxReturn;
	XMStoreFloat4x4(&mtxReturn, xmMtx);
	return mtxReturn;
}

XMFLOAT4X4 EditMath::LookAtLH(const XMFLOAT3& pos, const XMFLOAT3& posAt, const XMFLOAT3& vecUp)
{
	XMVECTOR xmPos = XMLoadFloat3(&pos);
	XMVECTOR xmPosAt = XMLoadFloat3(&posAt);
	XMVECTOR xmVecUp = XMLoadFloat3(&vecUp);
	XMMATRIX xmMtxView = XMMatrixLookAtLH(xmPos, xmPosAt, xmVecUp);
	XMFLOAT4X4 mtxView;
	XMStoreFloat4x4(&mtxView, xmMtxView);
	return mtxView;
}

XMFLOAT4X4 EditMath::PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ)
{
	XMMATRIX xmMtxProjection = XMMatrixPerspectiveFovLH(
		fov,
		aspect,
		nearZ,
		farZ);
	XMFLOAT4X4 mtxProjection;
	XMStoreFloat4x4(&mtxProjection, xmMtxProjection);
	return mtxProjection;
}

XMFLOAT4X4 EditMath::RotationX(float angle)
{
	XMMATRIX xmMtx = XMMatrixRotationX(angle);
	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, xmMtx);
	return mtx;
}

XMFLOAT4X4 EditMath::RotationY(float angle)
{
	XMMATRIX xmMtx = XMMatrixRotationY(angle);
	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, xmMtx);
	return mtx;
}

XMFLOAT4X4 EditMath::RotationZ(float angle)
{
	XMMATRIX xmMtx = XMMatrixRotationZ(angle);
	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, xmMtx);
	return mtx;
}

XMFLOAT4X4 EditMath::RotationAxis(const XMFLOAT3& vec, float angle)
{
	XMVECTOR xmVec = XMLoadFloat3(&vec);
	XMMATRIX xmMtx = XMMatrixRotationAxis(xmVec, angle);
	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, xmMtx);
	return mtx;
}