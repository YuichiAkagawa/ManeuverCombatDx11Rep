//**-------------------------------------------------------**
//**
//**               actor_plane_glass_front.cpp
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
#include "shader_hornet_glass.h"
#include "shader_manager.h"
#include "texture.h"
#include "actor_plane_glass_front.h"

using namespace DirectX;

ActorPlaneGlassFront::ActorPlaneGlassFront(ActorManager*  pActorManager) : ActorHierarchy(pActorManager)
{
	name_ = ActorManager::NAME_PLANE_GLASS_FRONT;
	tag_ = ActorManager::TAG_PLANE;
	layer_ = ActorManager::LAYER_GLASS;
}

ActorPlaneGlassFront::~ActorPlaneGlassFront()
{

}

bool ActorPlaneGlassFront::Init()
{
	nameUnique_ = "ActorPlaneGlassFront" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	transOffset_ = { 0.0f, 0.0f, 0.0f };

	return true;
}

void ActorPlaneGlassFront::Uninit()
{

}

void ActorPlaneGlassFront::Update()
{
	ReflectionMatrix();
}

void ActorPlaneGlassFront::Stats()
{
	ActorHierarchy::Stats();
}

void ActorPlaneGlassFront::Draw()
{
	{
		//�A���t�@�u�����h�L��
		Renderer::SetAlphaBlendConfig(true, 0);

		{
			//�R���X�^���g�o�b�t�@�쐬
			ShaderHornetGlass::CONSTANT_BUFFER cb;
			EditMath::Transpose(cb.mtxWorld, mtxWorld_);
			XMFLOAT4X4 mtxWorldInv;
			EditMath::Inverse(mtxWorldInv, mtxWorld_);
			EditMath::Transpose(cb.mtxWorldInv, mtxWorldInv);
			EditMath::Transpose(cb.mtxView, pCameraSelecter_->GetSelectCamera()->GetMtxView());
			EditMath::Transpose(cb.mtxProj, pCameraSelecter_->GetSelectCamera()->GetMtxProjection());
			cb.vecDirLight = { DIRECTIONAL_LIGHT.x, DIRECTIONAL_LIGHT.y, DIRECTIONAL_LIGHT.z, 1.0f };
			XMFLOAT3 posEye = pCameraSelecter_->GetSelectCamera()->GetPos();
			cb.posEye = { posEye.x, posEye.y, posEye.z, 1.0f };
			cb.specularData.x = 500.0f;		//�X�y�L�����̑傫��
			cb.specularData.y = 1.0f;		//�X�y�L�����̋���
			Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::HORNET_GLASS), 0, nullptr, &cb, 0, 0);
		}

		//�C���v�b�g���C�A�E�g�w��
		Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::HORNET_GLASS));

		//�V�F�[�_�Z�b�g
		Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::HORNET_GLASS), nullptr, 0);
		Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::HORNET_GLASS), nullptr, 0);

		//�R���X�^���g�o�b�t�@�Z�b�g
		Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::HORNET_GLASS));

		//���f���`��
		pModelManager_->GetInstance(ModelManager::HORNET_GLASS_FRONT)->Draw();

		//�A���t�@�u�����h����
		Renderer::SetAlphaBlendConfig(false, 0);
	}
}
