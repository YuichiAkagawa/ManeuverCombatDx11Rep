//**-------------------------------------------------------**
//**
//**                   actor_grid.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "renderer.h"
#include "shader_grid.h"
#include "shader_manager.h"
#include "actor_grid.h"

ActorGrid::ActorGrid(ActorManager* pActorManager) : ActorField(pActorManager)
{
	name_ = ActorManager::NAME_GRID;
	tag_ = ActorManager::TAG_FIELD;
	layer_ = ActorManager::LAYER_FIELD;
}

ActorGrid::~ActorGrid()
{

}

bool ActorGrid::Init()
{
	nameUnique_ = "ActorGrid" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

#if defined(_DEBUG) || defined(DEBUG)

	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth = sizeof(VERTEX_GRID) * 4;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;

		VERTEX_GRID vertex[] =
		{
			{ { 10.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { -10.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.0f, 0.0f, 10.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.0f, 0.0f, -10.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

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
		indexDesc.ByteWidth = sizeof(WORD) * 4;
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;

		WORD index[] =
		{
			0, 1, 2, 3
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
#endif
	return true;
}

void ActorGrid::Uninit()
{

}

void ActorGrid::Update()
{


}

void ActorGrid::Stats()
{
	ActorField::Stats();
}

void ActorGrid::Draw()
{
#if defined(_DEBUG) || defined(DEBUG)
	{
		ShaderGrid::CONSTANT_BUFFER cb;
		EditMath::Transpose(cb.mtxWorld, mtxWorld_);
		EditMath::Transpose(cb.mtxView, pCameraSelecter_->GetSelectCamera()->GetMtxView());
		EditMath::Transpose(cb.mtxProj, pCameraSelecter_->GetSelectCamera()->GetMtxProjection());

		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::GRID), 0, NULL, &cb, 0, 0);
	}

	//頂点バッファセット
	UINT stride = sizeof(VERTEX_GRID);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//インデックスバッファセット
	Renderer::GetDeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	//描画方法
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//インプットレイアウト指定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::GRID));

	//シェーダセット
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::GRID), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::GRID), nullptr, 0);

	//コンスタントバッファセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::GRID));

	//描画
	Renderer::GetDeviceContext()->DrawIndexed(4, 0, 0);
#endif
}