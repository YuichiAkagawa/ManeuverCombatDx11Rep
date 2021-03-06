//**-------------------------------------------------------**
//**
//**               actor_plane_radder_left.cpp
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
#include "actor_plane_radder_left.h"

using namespace DirectX;

ActorPlaneRadderLeft::ActorPlaneRadderLeft(ActorManager* pActorManager) : ActorHierarchy(pActorManager)
{
	name_ = ActorManager::NAME_PLANE_RADDER_LEFT;
	tag_ = ActorManager::TAG_PLANE;
	layer_ = ActorManager::LAYER_PLANE;
}

ActorPlaneRadderLeft::~ActorPlaneRadderLeft()
{

}

bool ActorPlaneRadderLeft::Init()
{
	nameUnique_ = "ActorPlaneRadderLeft" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	transOffset_ = { -109.0, 55.0f, -605.0f };
	EditMath::Subtraction(vecRotAxisOffset_, XMFLOAT3(-0.39751f, 1.05011f, -0.35119f), XMFLOAT3(-0.01435f, -0.00262f, -0.0011f));
	EditMath::Normalize(vecRotAxisOffset_, vecRotAxisOffset_);
	return true;
}

void ActorPlaneRadderLeft::Uninit()
{

}

void ActorPlaneRadderLeft::Update()
{
	ReflectionMatrix();
}

void ActorPlaneRadderLeft::Stats()
{
	ActorHierarchy::Stats();
}

void ActorPlaneRadderLeft::Draw()
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
		pModelManager_->GetInstance(ModelManager::HORNET_RADDER_LEFT)->Draw();
	}
}
