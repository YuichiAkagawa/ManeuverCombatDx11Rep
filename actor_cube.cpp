//**-------------------------------------------------------**
//**
//**                actor_cube.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor_3d.h"
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "renderer.h"
#include "sampler_state.h"
#include "shader.h"
#include "shader_cube.h"
#include "shader_manager.h"
#include "texture.h"
#include "actor_cube.h"

using namespace DirectX;

ActorCube::ActorCube(ActorManager* pActorManager) : Actor3d(pActorManager)
{
	name_ = ActorManager::NAME_CUBE;
	tag_ = ActorManager::TAG_CUBE;
	layer_ = ActorManager::LAYER_CUBE;
}

ActorCube::~ActorCube()
{

}

bool ActorCube::Init()
{
	nameUnique_ = "ActorCube" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth = sizeof(VERTEX3D) * 24;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		VERTEX3D vertex[] =
		{
			{ { -0.5f,  0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 0.0f, 0.0f } },
			{ { 0.5f,  0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 0.0f, 1.0f } },
			{ { 0.5f, -0.5f, -0.5f },{ 0.0f,  0.0f, -1.0f },{ 1.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 0.0f, 0.0f } },
			{ { -0.5f, -0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 1.0f, 0.0f } },
			{ { 0.5f,  0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 0.0f, 1.0f } },
			{ { 0.5f, -0.5f,  0.5f },{ 0.0f,  0.0f,  1.0f },{ 1.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f },{ -1.0f,  0.0f,  0.0f },{ 0.0f, 0.0f } },
			{ { -0.5f,  0.5f, -0.5f },{ -1.0f,  0.0f,  0.0f },{ 1.0f, 0.0f } },
			{ { -0.5f, -0.5f,  0.5f },{ -1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f },{ -1.0f,  0.0f,  0.0f },{ 1.0f, 1.0f } },

			{ { 0.5f,  0.5f,  0.5f },{ 1.0f,  0.0f,  0.0f },{ 0.0f, 0.0f } },
			{ { 0.5f, -0.5f,  0.5f },{ 1.0f,  0.0f,  0.0f },{ 1.0f, 0.0f } },
			{ { 0.5f,  0.5f, -0.5f },{ 1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f } },
			{ { 0.5f, -0.5f, -0.5f },{ 1.0f,  0.0f,  0.0f },{ 1.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f },{ 0.0f,  1.0f,  0.0f },{ 0.0f, 0.0f } },
			{ { 0.5f,  0.5f,  0.5f },{ 0.0f,  1.0f,  0.0f },{ 1.0f, 0.0f } },
			{ { -0.5f,  0.5f, -0.5f },{ 0.0f,  1.0f,  0.0f },{ 0.0f, 1.0f } },
			{ { 0.5f,  0.5f, -0.5f },{ 0.0f,  1.0f,  0.0f },{ 1.0f, 1.0f } },

			{ { -0.5f, -0.5f,  0.5f },{ 0.0f, -1.0f,  0.0f },{ 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f,  0.0f },{ 1.0f, 0.0f } },
			{ { 0.5f, -0.5f,  0.5f },{ 0.0f, -1.0f,  0.0f },{ 0.0f, 1.0f } },
			{ { 0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f,  0.0f },{ 1.0f, 1.0f } },
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

	XMFLOAT4X4 mtxScaling;
	EditMath::Scaling(mtxScaling, 5.0f, 5.0f, 5.0f);
	mtxWorld_ = mtxScaling;

	return true;
}

void ActorCube::Uninit()
{

}

void ActorCube::Update()
{

}

void ActorCube::Stats()
{
	Actor3d::Stats();
}

void ActorCube::Draw()
{
	{
		ShaderCube::CONSTANT_BUFFER cb;
		EditMath::Transpose(cb.mtxWorld, mtxWorld_);
		EditMath::Transpose(cb.mtxView, pCameraSelecter_->GetSelectCamera()->GetMtxView());
		EditMath::Transpose(cb.mtxProj, pCameraSelecter_->GetSelectCamera()->GetMtxProjection());
		XMFLOAT3 vecLight = { 1.0f, -1.0f, 1.0f };
		EditMath::Normalize(vecLight, vecLight);
		cb.vecLight = { -vecLight.x, -vecLight.y, -vecLight.z, 1.0f };

		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::CUBE), 0, NULL, &cb, 0, 0);
	}

	//頂点バッファセット
	UINT stride = sizeof(VERTEX3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//インデックスバッファセット
	Renderer::GetDeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	//描画方法
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//インプットレイアウト指定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::CUBE));

	//シェーダセット
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::CUBE), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::CUBE), nullptr, 0);

	//コンスタントバッファセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::CUBE));

	//テクスチャセット
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, pTextureManager_->GetInstance(TextureManager::SAMPLE)->GetTexture());
	Renderer::GetDeviceContext()->PSSetSamplers(0, 1, SamplerState::GetSamplerState(SamplerState::SAMPLER_TYPE_01));

	//描画
	Renderer::GetDeviceContext()->DrawIndexed(36, 0, 0);
}