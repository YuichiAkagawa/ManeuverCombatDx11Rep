//**-------------------------------------------------------**
//**
//**                 effekseer_effect.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#include "actor.h"
#include "main.h"
#include "renderer.h"
#include "effekseer_effect.h"

using namespace DirectX;

XMFLOAT3		EffekseerEffect::posCam_;
XMFLOAT3		EffekseerEffect::posCamAt_;
XMFLOAT3		EffekseerEffect::vecCamUp_;

EffekseerEffect::EffekseerEffect(int list) : listNumber_(LIST::UNKNOWN)
{
	listNumber_ = list;
	Init();
}

EffekseerEffect::~EffekseerEffect()
{
	Uninit();
}

void EffekseerEffect::Init()
{
	effekseerRenderer_ = ::EffekseerRendererDX11::Renderer::Create(Renderer::GetDevice(), Renderer::GetDeviceContext(), 2000);	//スプライトの最大数

	// エフェクト管理用インスタンスの生成
	effekseerManager_ = ::Effekseer::Manager::Create(2000);

	// 描画用インスタンスから描画機能を設定
	effekseerManager_->SetSpriteRenderer(effekseerRenderer_->CreateSpriteRenderer());
	effekseerManager_->SetRibbonRenderer(effekseerRenderer_->CreateRibbonRenderer());
	effekseerManager_->SetRingRenderer(effekseerRenderer_->CreateRingRenderer());
	effekseerManager_->SetTrackRenderer(effekseerRenderer_->CreateTrackRenderer());
	effekseerManager_->SetModelRenderer(effekseerRenderer_->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	effekseerManager_->SetTextureLoader(effekseerRenderer_->CreateTextureLoader());
	effekseerManager_->SetModelLoader(effekseerRenderer_->CreateModelLoader());

	//左手座標系に設定
	effekseerManager_->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	LoadEffect();

	//投影行列の更新
	effekseerRenderer_->SetProjectionMatrix(Effekseer::Matrix44().PerspectiveFovLH(XMConvertToRadians(CAMERA_FOV / 2.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, CAMERA_NEAR, CAMERA_FAR));

}

void EffekseerEffect::Uninit()
{
	// エフェクトの停止
	effekseerManager_->StopEffect(effekseerHandle_);

	//エフェクトを解放。再生中の場合は再生が終了した際、自動的に解放
	ES_SAFE_RELEASE(effekseerEffect_);

	//エフェクト管理用インスタンスを破棄
	effekseerManager_->Destroy();

	//描画用インスタンスを破棄
	effekseerRenderer_->Destroy();
}

void EffekseerEffect::Update()
{
	//カメラ行列の更新
	effekseerRenderer_->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(
		Effekseer::Vector3D(posCam_.x, posCam_.y, posCam_.z),
		Effekseer::Vector3D(posCamAt_.x, posCamAt_.y, posCamAt_.z),
		Effekseer::Vector3D(vecCamUp_.x, vecCamUp_.y, vecCamUp_.z)
	));

	if (repeat_)
	{
		if (!effekseerManager_->Exists(effekseerHandle_))
		{
			//エフェクトの再生
			effekseerHandle_ = effekseerManager_->Play(effekseerEffect_, effekseerManager_->GetLocation(effekseerHandle_).X, effekseerManager_->GetLocation(effekseerHandle_).Y, effekseerManager_->GetLocation(effekseerHandle_).Z);
		}
	}

	//すべてのエフェクトの更新
	effekseerManager_->Flip();
	effekseerManager_->Update();
}

void EffekseerEffect::Draw()
{
	effekseerRenderer_->BeginRendering();
	effekseerManager_->Draw();
	effekseerRenderer_->EndRendering();
}

void EffekseerEffect::SetPos(const DirectX::XMFLOAT3& pos)
{
	effekseerManager_->SetLocation(effekseerHandle_, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

void EffekseerEffect::SetMatrix(const XMFLOAT4X4& mtx)
{
	Effekseer::Matrix43 matrix;
	matrix.Value[0][0] = mtx._11;
	matrix.Value[0][1] = mtx._12;
	matrix.Value[0][2] = mtx._13;
	matrix.Value[1][0] = mtx._21;
	matrix.Value[1][1] = mtx._22;
	matrix.Value[1][2] = mtx._23;
	matrix.Value[2][0] = mtx._31;
	matrix.Value[2][1] = mtx._32;
	matrix.Value[2][2] = mtx._33;
	matrix.Value[3][0] = mtx._41;
	matrix.Value[3][1] = mtx._42;
	matrix.Value[3][2] = mtx._43;
	effekseerManager_->SetMatrix(effekseerHandle_, matrix);
}

void EffekseerEffect::SetSpeed(float speed)
{
	effekseerManager_->SetSpeed(effekseerHandle_, speed);
}

void EffekseerEffect::SetRepeat(bool repeat)
{
	repeat_ = repeat;
}

void EffekseerEffect::Play()
{
	//エフェクトの再生
	effekseerHandle_ = effekseerManager_->Play(effekseerEffect_, effekseerManager_->GetLocation(effekseerHandle_).X, effekseerManager_->GetLocation(effekseerHandle_).Y, effekseerManager_->GetLocation(effekseerHandle_).Z);
}

void EffekseerEffect::Paused(bool paused)
{
	paused_ = paused;
	effekseerManager_->SetPaused(effekseerHandle_, paused);
}

void EffekseerEffect::Stop()
{
	//エフェクトの停止
	effekseerManager_->StopEffect(effekseerHandle_);
}

void EffekseerEffect::LoadEffect()
{
	//エフェクトの読み込み
	effekseerEffect_ = Effekseer::Effect::Create(effekseerManager_, (const EFK_CHAR*)effekseerFileName[listNumber_]);
}

void EffekseerEffect::SetViewData(const DirectX::XMFLOAT3& posCam, const DirectX::XMFLOAT3& posCamAt, const DirectX::XMFLOAT3& vecCamUp)
{
	posCam_ = posCam;
	posCamAt_ = posCamAt;
	vecCamUp_ = vecCamUp;
}