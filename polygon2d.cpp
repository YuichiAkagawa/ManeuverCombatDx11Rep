//**-------------------------------------------------------**
//**
//**                     polygon2d.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include "main.h"
#include "renderer.h"
#include "vertex_shader_manager.h"
#include "pixel_shader_manager.h"
#include "polygon2d.h"

bool Polygon2D::Init()
{
	{
		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth = sizeof(VERTEX2D) * 4;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		VERTEX2D vertex[] =
		{
			{ { -0.5f,  0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.5f, -0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { 0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } },
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
		//�C���f�b�N�X�o�b�t�@�쐬
		D3D11_BUFFER_DESC indexDesc;
		indexDesc.ByteWidth = sizeof(WORD) * 6;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;

		WORD index[] =
		{
			0, 1, 2,
			0, 3, 1,
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
		//�r���[�|�[�g�쐬
		viewport_.TopLeftX = 0;
		viewport_.TopLeftY = 0;
		viewport_.Width = (FLOAT)SCREEN_WIDTH;
		viewport_.Height = (FLOAT)SCREEN_HEIGHT;
		viewport_.MinDepth = 0.0f;
		viewport_.MaxDepth = 1.0f;
	}

	return true;
}

void Polygon2D::Uninit()
{
	SafeRelease(pVertexBuffer_);
	SafeRelease(pIndexBuffer_);
}

void Polygon2D::Draw()
{
	//���_�o�b�t�@�Z�b�g
	UINT stride = sizeof(VERTEX2D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�Z�b�g
	Renderer::GetDeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	//�`����@
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�r���[�|�[�g�Z�b�g
	Renderer::GetDeviceContext()->RSSetViewports(1, &viewport_);

	//�C���v�b�g���C�A�E�g�w��
	Renderer::GetDeviceContext()->IASetInputLayout(VertexShaderManager::GetInputLayout(VertexShaderManager::VS_TEST));

	//�V�F�[�_�Z�b�g
	Renderer::GetDeviceContext()->VSSetShader(VertexShaderManager::GetVertexShader(VertexShaderManager::VS_TEST), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(PixelShaderManager::GetPixelShader(PixelShaderManager::PS_TEST), nullptr, 0);

	//�`��
	Renderer::GetDeviceContext()->DrawIndexed(6, 0, 0);
}