//**-------------------------------------------------------**
//**
//**                    edit_math.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include <DirectXMath.h>
#include "edit_math.h"

using namespace DirectX;

void EditMath::Addition(XMFLOAT3& valueReturn, const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	valueReturn = { valueLeft.x + valueRight.x, valueLeft.y + valueRight.y, valueLeft.z + valueRight.z };
}

void EditMath::Subtraction(XMFLOAT3& valueReturn, const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	valueReturn = { valueLeft.x - valueRight.x, valueLeft.y - valueRight.y, valueLeft.z - valueRight.z };
}

void EditMath::Multiplication(XMFLOAT3& valueReturn, const XMFLOAT3& valueLeft, float valueRight)
{
	valueReturn = { valueLeft.x * valueRight, valueLeft.y * valueRight, valueLeft.z * valueRight };
}

void EditMath::Multiplication(XMFLOAT3& valueReturn, const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	valueReturn = { valueLeft.x * valueRight.x, valueLeft.y * valueRight.y, valueLeft.z * valueRight.z };
}

void EditMath::Division(XMFLOAT3& valueReturn, const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	valueReturn = { valueLeft.x / valueRight.x, valueLeft.y / valueRight.y, valueLeft.z / valueRight.z };
}

void EditMath::Normalize(XMFLOAT3& valueReturn, const XMFLOAT3& value)
{
	XMVECTOR xmValue = XMLoadFloat3(&value);
	xmValue = XMVector3Normalize(xmValue);
	XMStoreFloat3(&valueReturn, xmValue);
}

float EditMath::Dot(const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMVECTOR xmValueLeft = XMLoadFloat3(&valueLeft);
	xmValueLeft = XMVector3Normalize(xmValueLeft);
	XMVECTOR xmValueRight = XMLoadFloat3(&valueRight);
	xmValueRight = XMVector3Normalize(xmValueRight);
	XMVECTOR xmInner = XMVector3Dot(xmValueLeft, xmValueRight);
	return XMVectorGetX(xmInner);
}

void EditMath::Cross(XMFLOAT3& valueReturn, const XMFLOAT3& valueLeft, const XMFLOAT3& valueRight)
{
	XMVECTOR xmValueLeft = XMLoadFloat3(&valueLeft);
	xmValueLeft = XMVector3Normalize(xmValueLeft);
	XMVECTOR xmValueRight = XMLoadFloat3(&valueRight);
	xmValueRight = XMVector3Normalize(xmValueRight);
	XMVECTOR xmOuter = XMVector3Cross(xmValueLeft, xmValueRight);
	XMStoreFloat3(&valueReturn, xmOuter);
}

void EditMath::Transform(XMFLOAT3& valueReturn, const XMFLOAT3& value, const XMFLOAT4X4& mtx)
{
	XMVECTOR xmVec = XMLoadFloat3(&value);
	XMMATRIX xmMtx = XMLoadFloat4x4(&mtx);
	XMVECTOR xmVecReturn = XMVector3Transform(xmVec, xmMtx);
	XMStoreFloat3(&valueReturn, xmVecReturn);
}

void EditMath::Identity(XMFLOAT4X4& valueReturn)
{
	XMMATRIX xmMtx = XMMatrixIdentity();
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::Inverse(XMFLOAT4X4& valueReturn, const XMFLOAT4X4& value)
{
	XMMATRIX xmMtx = XMLoadFloat4x4(&value);
	xmMtx = XMMatrixInverse(nullptr, xmMtx);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::Transpose(XMFLOAT4X4& valueReturn, const XMFLOAT4X4& value)
{
	XMMATRIX xmMtx = XMLoadFloat4x4(&value);
	xmMtx = XMMatrixTranspose(xmMtx);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::LookAtLH(XMFLOAT4X4& valueReturn, const XMFLOAT3& pos, const XMFLOAT3& posAt, const XMFLOAT3& vecUp)
{
	XMVECTOR xmPos = XMLoadFloat3(&pos);
	XMVECTOR xmPosAt = XMLoadFloat3(&posAt);
	XMVECTOR xmVecUp = XMLoadFloat3(&vecUp);
	XMMATRIX xmMtxView = XMMatrixLookAtLH(xmPos, xmPosAt, xmVecUp);
	XMStoreFloat4x4(&valueReturn, xmMtxView);
}

void EditMath::PerspectiveFovLH(XMFLOAT4X4& valueReturn, float fov, float aspect, float nearZ, float farZ)
{
	XMMATRIX xmMtxProjection = XMMatrixPerspectiveFovLH(
		fov,
		aspect,
		nearZ,
		farZ);
	XMStoreFloat4x4(&valueReturn, xmMtxProjection);
}

void EditMath::Scaling(XMFLOAT4X4& valueReturn, float value)
{
	XMMATRIX xmMtx = XMMatrixScaling(value, value, value);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::Scaling(XMFLOAT4X4& valueReturn, float x, float y, float z)
{
	XMMATRIX xmMtx = XMMatrixScaling(x, y, z);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::Scaling(XMFLOAT4X4& valueReturn, const XMFLOAT3& value)
{
	XMMATRIX xmMtx = XMMatrixScaling(value.x, value.y, value.z);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::RotationX(XMFLOAT4X4& valueReturn, float angle)
{
	XMMATRIX xmMtx = XMMatrixRotationX(angle);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::RotationY(XMFLOAT4X4& valueReturn, float angle)
{
	XMMATRIX xmMtx = XMMatrixRotationY(angle);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::RotationZ(XMFLOAT4X4& valueReturn, float angle)
{
	XMMATRIX xmMtx = XMMatrixRotationZ(angle);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::RotationAxis(XMFLOAT4X4& valueReturn, const XMFLOAT3& value, float angle)
{
	XMVECTOR xmVec = XMLoadFloat3(&value);
	XMMATRIX xmMtx = XMMatrixRotationAxis(xmVec, angle);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::Translation(XMFLOAT4X4& valueReturn, float x, float y, float z)
{
	XMMATRIX xmMtx = XMMatrixTranslation(x, y, z);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::Translation(XMFLOAT4X4& valueReturn, const XMFLOAT3& value)
{
	XMMATRIX xmMtx = XMMatrixTranslation(value.x, value.y, value.z);
	XMStoreFloat4x4(&valueReturn, xmMtx);
}

void EditMath::Multiplication(XMFLOAT4X4& valueReturn, const XMFLOAT4X4& valueLeft, const XMFLOAT4X4& valueRight)
{
	XMMATRIX xmMtxLeft = XMLoadFloat4x4(&valueLeft);
	XMMATRIX xmMtxRight = XMLoadFloat4x4(&valueRight);
	XMMATRIX xmMtxReturn = XMMatrixMultiply(xmMtxLeft, xmMtxRight);
	XMStoreFloat4x4(&valueReturn, xmMtxReturn);
}