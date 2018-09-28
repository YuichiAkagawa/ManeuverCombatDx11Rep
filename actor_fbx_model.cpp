//**-------------------------------------------------------**
//**
//**                actor_fbx_model.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include <fbxsdk.h>
#include <string>
#include "actor_3d.h"
#include "actor_camera.h"
#include "actor_camera_selecter.h"
#include "actor_manager.h"
#include "edit_math.h"
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "sampler_state.h"
#include "shader.h"
#include "shader_fbx.h"
#include "shader_manager.h"
#include "texture.h"
#include "actor_fbx_model.h"

using namespace DirectX;

ActorFbxModel::ActorFbxModel(ActorManager* pActorManager) : Actor3d(pActorManager)
{
	name_ = ActorManager::NAME_FBX_MODEL;
	tag_ = ActorManager::TAG_FBX_MODEL;
	layer_ = ActorManager::LAYER_FBX_MODEL;
}

ActorFbxModel::~ActorFbxModel()
{

}

bool ActorFbxModel::Init()
{
	nameUnique_ = "ActorFbxModel" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	EditMath::Scaling(mtxWorld_, 0.1f, 0.1f, 0.1f);

	return true;
}

void ActorFbxModel::Uninit()
{

}

void ActorFbxModel::Update()
{
	{
		ShaderFbx::CONSTANT_BUFFER cb;
		EditMath::Transpose(cb.mtxWorld, mtxWorld_);
		XMFLOAT4X4 mtxWorldInv;
		EditMath::Inverse(mtxWorldInv, mtxWorld_);
		EditMath::Transpose(cb.mtxWorldInv, mtxWorldInv);
		mtxWorld_ = mtxWorld_;
		mtxWorldInv = mtxWorldInv;

		EditMath::Transpose(cb.mtxView, pCameraSelecter_->GetSelectCamera()->GetMtxView());
		EditMath::Transpose(cb.mtxProj, pCameraSelecter_->GetSelectCamera()->GetMtxProjection());
		cb.vecDirLight = { DIRECTIONAL_LIGHT.x, DIRECTIONAL_LIGHT.y, DIRECTIONAL_LIGHT.z, 1.0f };
		XMFLOAT3 posEye = pCameraSelecter_->GetSelectCamera()->GetPos();
		cb.posEye = { posEye.x, posEye.y, posEye.z, 1.0f };
		cb.specularData.x = 150.0f;		//スペキュラの大きさ
		cb.specularData.y = 1.0f;		//スペキュラの強さ
		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::FBX), 0, NULL, &cb, 0, 0);

	}
	{
		XMFLOAT4X4 mtxRotX;
		EditMath::RotationX(mtxRotX, XMConvertToRadians(0.1f));
		//EditMath::Multiplication(mtxWorld_, mtxRotX, mtxWorld_);

		XMFLOAT4X4 mtxRotY;
		EditMath::RotationY(mtxRotY, XMConvertToRadians(0.2f));
		EditMath::Multiplication(mtxWorld_, mtxRotY, mtxWorld_);
	
		XMFLOAT4X4 mtxRotZ;
		EditMath::RotationZ(mtxRotZ, XMConvertToRadians(0.3f));
		//EditMath::Multiplication(mtxWorld_, mtxRotZ, mtxWorld_);
	}
}

void ActorFbxModel::Stats()
{
	Actor3d::Stats();
}

void ActorFbxModel::Draw()
{
	//インプットレイアウト指定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::FBX));

	//シェーダセット
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::FBX), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::FBX), nullptr, 0);

	//コンスタントバッファセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::FBX));

	//ノーマルマップセット
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, pTextureManager_->GetInstance(TextureManager::NORMAL01)->GetTexture());

	//サンプラーセット
	Renderer::GetDeviceContext()->PSSetSamplers(0, 1, SamplerState::GetSamplerState(SamplerState::SAMPLER_TYPE_01));

	//モデル描画
	pModelManager_->GetInstance(ModelManager::SAMPLE)->Draw();
}