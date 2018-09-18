//**-------------------------------------------------------**
//**
//**                     cube.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include <d3d11.h>
#include "main.h"
#include "renderer.h"
#include "shader_manager.h"
#include "cube.h"

bool Cube::Init()
{
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth = sizeof(VERTEX2D) * 24;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		VERTEX2D vertex[] =
		{
			{ { -0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
			{ { -0.5f, -0.5f,  0.5f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
			{ { 0.5f,  0.5f,  0.5f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
			{ { 0.5f, -0.5f,  0.5f },{ 0.0f, 1.0f, 1.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f,  0.5f, -0.5f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f,  0.5f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f },{ 1.0f, 1.0f, 0.0f, 1.0f } },

			{ { 0.5f,  0.5f,  0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { 0.5f, -0.5f,  0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { 0.5f,  0.5f, -0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { 0.5f, -0.5f, -0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
			{ { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
			{ { -0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
			{ { 0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 1.0f, 1.0f } },

			{ { -0.5f, -0.5f,  0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { 0.5f, -0.5f,  0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { 0.5f, -0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
		};

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = vertex;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		HRESULT hr = Renderer::GetDevice()->CreateBuffer(
			&vertexDesc,
			&sd,
			&pVertexBuffer_);
		if (FAILED(hr))
		{
			return false;
		}
	}

	{
		//インデックスバッファ作成
		D3D11_BUFFER_DESC indexDesc;
		indexDesc.ByteWidth = sizeof(WORD) * 6 * 6;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;

		WORD index[] =
		{
			0,  1,  2,     3,  2,  1,
			4,  5,  6,     7,  6,  5,
			8,  9, 10,    11, 10,  9,
			12, 13, 14,    15, 14, 13,
			16, 17, 18,    19, 18, 17,
			20, 21, 22,    23, 22, 21,
		};

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = index;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		HRESULT hr = Renderer::GetDevice()->CreateBuffer(
			&indexDesc,
			&sd,
			&pIndexBuffer_);
		if (FAILED(hr))
		{
			return false;
		}
	}

	{
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(CONSTANT_BUFFER);
		cbDesc.Usage = D3D11_USAGE_DEFAULT;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = 0;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		HRESULT hr = Renderer::GetDevice()->CreateBuffer(&cbDesc, NULL, &pConstantBuffer_);
		if (FAILED(hr))
		{
			return false;
		}

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

		DirectX::XMVECTOR eye = DirectX::XMVectorSet(2.0f, 2.0f, -2.0f, 0.0f);
		DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);

		DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(CAMERA_FOV / 2),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			CAMERA_NEAR,
			CAMERA_FAR);

		CONSTANT_BUFFER cb;
		XMStoreFloat4x4(&cb.mtxWorld, XMMatrixTranspose(worldMatrix));
		XMStoreFloat4x4(&cb.mtxView, XMMatrixTranspose(viewMatrix));
		XMStoreFloat4x4(&cb.mtxProj, XMMatrixTranspose(projMatrix));
		Renderer::GetDeviceContext()->UpdateSubresource(pConstantBuffer_, 0, NULL, &cb, 0, 0);
	}

	return true;
}

void Cube::Uninit()
{

}

void Cube::Draw()
{
	//頂点バッファセット
	UINT stride = sizeof(VERTEX2D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//インデックスバッファセット
	Renderer::GetDeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	//描画方法
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//インプットレイアウト指定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::TEST));

	//シェーダセット
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::TEST), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::TEST), nullptr, 0);
	
	//コンスタントバッファセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer_);

	//描画
	Renderer::GetDeviceContext()->DrawIndexed(36, 0, 0);
}