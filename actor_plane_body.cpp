//**-------------------------------------------------------**
//**
//**               actor_plane_body.h
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
#include "actor_plane_body.h"

using namespace DirectX;

ActorPlaneBody::ActorPlaneBody(ActorManager* pActorManager) : ActorHierarchy(pActorManager)
{
	name_ = ActorManager::NAME_PLANE_BODY;
	tag_ = ActorManager::TAG_PLANE;
	layer_ = ActorManager::LAYER_PLANE;
}

ActorPlaneBody::~ActorPlaneBody()
{

}

bool ActorPlaneBody::Init()
{
	nameUnique_ = "ActorPlaneBody" + std::to_string(number_);

	pCameraSelecter_ = dynamic_cast <ActorCameraSelecter*> (pActorManager_->GetActor(ActorManager::NAME_CAMERA_SELECTER, 0));
	if (pCameraSelecter_ == nullptr) { return false; }

	//�X�P�[������
	scaleOffset_ = { 0.0075f, 0.0075f, 0.0075f };

	return true;
}

void ActorPlaneBody::Uninit()
{

}

void ActorPlaneBody::Update()
{
	ReflectionMatrix();
}

void ActorPlaneBody::Stats()
{
	ActorHierarchy::Stats();
}

void ActorPlaneBody::Draw()
{
	{
		//�R���X�^���g�o�b�t�@�쐬
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
		cb.specularData.x = 150.0f;		//�X�y�L�����̑傫��
		cb.specularData.y = 0.3f;		//�X�y�L�����̋���
		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::HORNET_BODY), 0, nullptr, &cb, 0, 0);
	}

	//�C���v�b�g���C�A�E�g�w��
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::GetInputLayout(ShaderManager::HORNET_BODY));

	//�V�F�[�_�Z�b�g
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(ShaderManager::HORNET_BODY), nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(ShaderManager::HORNET_BODY), nullptr, 0);

	//�R���X�^���g�o�b�t�@�Z�b�g
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, ShaderManager::GetConstantBuffer(ShaderManager::HORNET_BODY));

	//�m�[�}���}�b�v�Z�b�g
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, pTextureManager_->GetInstance(TextureManager::NORMAL01)->GetTexture());

	//�T���v���[�Z�b�g
	Renderer::GetDeviceContext()->PSSetSamplers(0, 1, SamplerState::GetSamplerState(SamplerState::SAMPLER_TYPE_01));

	//���f���`��
	pModelManager_->GetInstance(ModelManager::HORNET_BODY)->Draw();

	//�m�[�}���}�b�v�Z�b�g
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, pTextureManager_->GetInstance(TextureManager::NORMAL02)->GetTexture());

	//���f���`��
	pModelManager_->GetInstance(ModelManager::HORNET_BODY_PARTS)->Draw();

	//�m�[�}���}�b�v�Z�b�g
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, pTextureManager_->GetInstance(TextureManager::NORMAL03)->GetTexture());

	//���f���`��
	pModelManager_->GetInstance(ModelManager::HORNET_BODY_PARTS2)->Draw();

	//���f���`��
	pModelManager_->GetInstance(ModelManager::HORNET_AIM120)->Draw();

	//���f���`��
	pModelManager_->GetInstance(ModelManager::HORNET_AIM9_RIGHT)->Draw();

	//���f���`��
	pModelManager_->GetInstance(ModelManager::HORNET_AIM9_LEFT)->Draw();

	{
		//�R���X�^���g�o�b�t�@�쐬
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
		cb.specularData.x = 1.0f;		//�X�y�L�����̑傫��
		cb.specularData.y = 0.1f;		//�X�y�L�����̋���
		Renderer::GetDeviceContext()->UpdateSubresource(*ShaderManager::GetConstantBuffer(ShaderManager::HORNET_BODY), 0, nullptr, &cb, 0, 0);
	}

	//�m�[�}���}�b�v�Z�b�g
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, pTextureManager_->GetInstance(TextureManager::NORMAL04)->GetTexture());

	//���f���`��
	pModelManager_->GetInstance(ModelManager::HORNET_PILOT)->Draw();

}
