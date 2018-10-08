//**-------------------------------------------------------**
//**
//**                actor_sea.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <d3d11.h>
#include <DirectXMath.h>
#include "actor.h"
#include "actor_3d.h"
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_field.h"
#include "actor_free_camera.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "main.h"
#include "renderer.h"
#include "sampler_state.h"
#include "scene.h"
#include "scene_manager.h"
#include "shader_manager.h"
#include "shader_sea.h"
#include "texture.h"
#include "actor_sea.h"

using namespace DirectX;

constexpr float SCALE = 500.0f;
constexpr DWORD WIDTH = 200;

ActorSea::ActorSea(ActorManager* pActorManager) : ActorField(pActorManager)
{
	name_ = ActorManager::NAME_SEA;
	tag_ = ActorManager::TAG_FIELD;
	layer_ = ActorManager::LAYER_FIELD;
}

ActorSea::~ActorSea()
{

}

bool ActorSea::Init()
{
	nameUnique_ = "ActorSea" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth = sizeof(VERTEX_SEA) * 4;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		//法線、接線算出
		XMFLOAT3 vecNormal = { 0.0f, 1.0f, 0.0001f };
		EditMath::Normalize(vecNormal, vecNormal);
		XMFLOAT3 vecTangent;
		EditMath::Cross(vecTangent, vecNormal, VEC_Y);
		EditMath::Normalize(vecTangent, vecTangent);

		//色指定
		XMFLOAT4 color = { 0.2f, 0.2f, 0.4f, 1.0f };

		VERTEX_SEA vertex[] =
		{
			{	
				//左手前
				{ -0.5f * SCALE, 0.0f, -0.5f * SCALE },
				vecNormal,
				color,
				{ 0.0f, 1.0f },
				vecTangent
			},
			{
				//左奥
				{ -0.5f * SCALE, 0.0f, 0.5f * SCALE },
				vecNormal,
				color,
				{ 0.0f, 0.0f },
				vecTangent
			},
			{
				//右手前
				{ 0.5f * SCALE, 0.0f, -0.5f * SCALE },
				vecNormal,
				color,
				{ 1.0f, 1.0f },
				vecTangent
			},
			{
				//右奥
				{ 0.5f * SCALE, 0.0f, 0.5f * SCALE },
				vecNormal,
				color,
				{ 1.0f, 0.0f },
				vecTangent
			},
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
		indexDesc.ByteWidth = sizeof(WORD) * 6;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;

		WORD index[] =
		{
			0, 1, 2, 2, 1, 3,
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
		//座標点バッファ作成
		D3D11_BUFFER_DESC posWorldDesc;
		posWorldDesc.ByteWidth = sizeof(POS_WORLD) * (WIDTH * WIDTH);
		posWorldDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		posWorldDesc.CPUAccessFlags = 0;
		posWorldDesc.MiscFlags = 0;
		posWorldDesc.StructureByteStride = 0;
		posWorldDesc.Usage = D3D11_USAGE_DEFAULT;

		unsigned int numPos = 0;
		POS_WORLD pw[WIDTH * WIDTH];
		for (UINT i = 0; i < WIDTH; i++)
		{
			for (UINT j = 0; j < WIDTH; j++)
			{
				pw[numPos].x = SCALE * j - (SCALE * WIDTH) / 2.0f;
				pw[numPos].y = 0.0f;
				pw[numPos].z = SCALE * i - (SCALE * WIDTH) / 2.0f;
				numPos++;
			}
		}

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = pw;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		HRESULT hr = Renderer::GetDevice()->CreateBuffer(
			&posWorldDesc,
			&sd,
			&pPosWorldBuffer_);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ActorSea::Uninit()
{
	SafeRelease(pVertexBuffer_);
	SafeRelease(pIndexBuffer_);
	SafeRelease(pPosWorldBuffer_);
}

void ActorSea::Update()
{

}

void ActorSea::Stats()
{
	ActorField::Stats();
}

void ActorSea::Draw()
{
	{
		//コンスタントバッファ作成
		ShaderSea::CONSTANT_BUFFER cb;
		EditMath::Transpose(cb.mtxWorld, mtxWorld_);
		XMFLOAT4X4 mtxWorldInv;
		EditMath::Inverse(mtxWorldInv, mtxWorld_);
		EditMath::Transpose(cb.mtxWorldInv, mtxWorldInv);
		EditMath::Transpose(cb.mtxView, pCameraSelecter_->GetSelectCamera()->GetMtxView());
		EditMath::Transpose(cb.mtxProj, pCameraSelecter_->GetSelectCamera()->GetMtxProjection());
		cb.vecDirLight = { DIRECTIONAL_LIGHT.x, DIRECTIONAL_LIGHT.y, DIRECTIONAL_LIGHT.z, 1.0f };
		XMFLOAT3 posEye = pCameraSelecter_->GetSelectCamera()->GetPos();
		cb.posEye = { posEye.x, posEye.y, posEye.z, 1.0f };
		cb.specularData.x = 85.0f;		//スペキュラの大きさ
		cb.specularData.y = 0.7f;		//スペキュラの強さ
		cb.time.x = SceneManager::GetScene()->GetTime();
		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::SEA), 0, nullptr, &cb, 0, 0);
	}

	//頂点バッファセット
	{
		UINT stride = sizeof(VERTEX_SEA);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	}

	//インデックスバッファセット
	Renderer::GetDeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	//座標バッファセット
	{
		UINT stride = sizeof(POS_WORLD);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(1, 1, &pPosWorldBuffer_, &stride, &offset);
	}

	//描画方法
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//インプットレイアウト指定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::SEA));

	//シェーダセット
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::SEA), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::SEA), nullptr, 0);

	//コンスタントバッファセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::SEA));

	//テクスチャセット
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, pTextureManager_->GetInstance(TextureManager::SEA)->GetTexture());
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, pTextureManager_->GetInstance(TextureManager::SEA_NORMAL01)->GetTexture());
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, pTextureManager_->GetInstance(TextureManager::SEA_NORMAL02)->GetTexture());

	//サンプラーセット
	Renderer::GetDeviceContext()->PSSetSamplers(0, 1, SamplerState::GetSamplerState(SamplerState::SAMPLER_TYPE_01));

	//描画
	Renderer::GetDeviceContext()->DrawIndexedInstanced(6, (WIDTH * WIDTH), 0, 0, 0);
}
