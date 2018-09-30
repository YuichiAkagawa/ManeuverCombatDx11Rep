//**-------------------------------------------------------**
//**
//**                actor_skydome.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
#include <DirectXMath.h>
#include "actor.h"
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_field.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "model.h"
#include "renderer.h"
#include "sampler_state.h"
#include "shader_manager.h"
#include "shader_skydome.h"
#include "texture.h"
#include "actor_skydome.h"

using namespace DirectX;

constexpr float SKYDOME_SCALE = 10000.0f;

ActorSkydome::ActorSkydome(ActorManager* pActorManager) : ActorField(pActorManager)
{
	name_ = ActorManager::NAME_SKYDOME;
	tag_ = ActorManager::TAG_FIELD;
	layer_ = ActorManager::LAYER_FIELD;
}

ActorSkydome::~ActorSkydome()
{

}

bool ActorSkydome::Init()
{
	nameUnique_ = "ActorSkydome" + std::to_string(number_);

	//スケール反転
	EditMath::Scaling(mtxWorld_, -SKYDOME_SCALE);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	return true;
}

void ActorSkydome::Uninit()
{

}

void ActorSkydome::Update()
{
	XMFLOAT3 posCam = pCameraSelecter_->GetSelectCamera()->GetPos();
	pos_.x = posCam.x;
	pos_.z = posCam.z;
	mtxWorld_._41 = pos_.x;
	mtxWorld_._43 = pos_.z;

	{
		//コンスタントバッファ作成
		ShaderSkydome::CONSTANT_BUFFER cb;
		EditMath::Transpose(cb.mtxWorld, mtxWorld_);
		EditMath::Transpose(cb.mtxView, pCameraSelecter_->GetSelectCamera()->GetMtxView());
		EditMath::Transpose(cb.mtxProj, pCameraSelecter_->GetSelectCamera()->GetMtxProjection());
		cb.colorValue = { 0.1f, 0.2f, 0.7f, 1.0f };
		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::SKYDOME), 0, NULL, &cb, 0, 0);
	}
}

void ActorSkydome::Stats()
{
	ActorField::Stats();
}

void ActorSkydome::Draw()
{
	//インプットレイアウト指定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::SKYDOME));

	//シェーダセット
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::SKYDOME), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::SKYDOME), nullptr, 0);

	//コンスタントバッファセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::SKYDOME));

	//サンプラーセット
	Renderer::GetDeviceContext()->PSSetSamplers(0, 1, SamplerState::GetSamplerState(SamplerState::SAMPLER_TYPE_01));

	//モデル描画
	pModelManager_->GetInstance(ModelManager::SKYDOME)->Draw();
}
