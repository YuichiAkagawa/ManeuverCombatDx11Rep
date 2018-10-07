//**-------------------------------------------------------**
//**
//**               actor_plane_canopy.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <DirectXMath.h>
#include "actor_3d.h"
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "sampler_state.h"
#include "shader_hornet_body.h"
#include "shader_manager.h"
#include "texture.h"
#include "actor_plane_canopy.h"

using namespace DirectX;

ActorPlaneCanopy::ActorPlaneCanopy(ActorManager*  pActorManager) : ActorHierarchy(pActorManager)
{
	name_ = ActorManager::NAME_PLANE_CANOPY;
	tag_ = ActorManager::TAG_PLANE;
	layer_ = ActorManager::LAYER_PLANE;
}

ActorPlaneCanopy::~ActorPlaneCanopy()
{

}

bool ActorPlaneCanopy::Init()
{
	nameUnique_ = "ActorPlaneCanopy" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	transOffset_ = { 0.0f, 111.0f, 200.5f };
	vecRotAxisOffset_ = { 1.0f, 0.0f, 0.0f };	//モデリングツールからの座標
	EditMath::Normalize(vecRotAxisOffset_, vecRotAxisOffset_);

	return true;
}

void ActorPlaneCanopy::Uninit()
{

}

void ActorPlaneCanopy::Update()
{
	ReflectionMatrix();
}

void ActorPlaneCanopy::Stats()
{
	ActorHierarchy::Stats();
}

void ActorPlaneCanopy::Draw()
{
	{
		{
			//コンスタントバッファ作成
			ShaderHornetBody::CONSTANT_BUFFER cb;
			EditMath::Transpose(cb.mtxWorld, mtxWorld_);
			XMFLOAT4X4 mtxWorldInv;
			EditMath::Inverse(mtxWorldInv, mtxWorld_);
			EditMath::Transpose(cb.mtxWorldInv, mtxWorldInv);
			EditMath::Transpose(cb.mtxView, pCameraSelecter_->GetSelectCamera()->GetMtxView());
			EditMath::Transpose(cb.mtxProj, pCameraSelecter_->GetSelectCamera()->GetMtxProjection());
			cb.vecDirLight = { DIRECTIONAL_LIGHT.x, DIRECTIONAL_LIGHT.y, DIRECTIONAL_LIGHT.z, 1.0f };
			XMFLOAT3 posEye = pCameraSelecter_->GetSelectCamera()->GetPos();
			cb.posEye = { posEye.x, posEye.y, posEye.z, 1.0f };
			cb.specularData.x = 150.0f;		//スペキュラの大きさ
			cb.specularData.y = 0.3f;		//スペキュラの強さ
			Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::HORNET_BODY), 0, nullptr, &cb, 0, 0);
		}

		//インプットレイアウト指定
		Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::HORNET_BODY));

		//シェーダセット
		Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::HORNET_BODY), nullptr, 0);
		Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::HORNET_BODY), nullptr, 0);

		//コンスタントバッファセット
		Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::HORNET_BODY));

		//ノーマルマップセット
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, pTextureManager_->GetInstance(TextureManager::NORMAL03)->GetTexture());

		//サンプラーセット
		Renderer::GetDeviceContext()->PSSetSamplers(0, 1, SamplerState::GetSamplerState(SamplerState::SAMPLER_TYPE_01));

		//モデル描画
		pModelManager_->GetInstance(ModelManager::HORNET_CANOPY)->Draw();
	}
}
