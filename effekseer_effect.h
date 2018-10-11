//**-------------------------------------------------------**
//**
//**                effekseer_effect.cpp
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

#ifndef _EFFEKSEER_EFFECT_H_
#define _EFFEKSEER_EFFECT_H_

#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

static const wchar_t* effekseerFileName[] =
{
	L"resource/effekseer/vapour.efk",
	L"resource/effekseer/burner.efk",
	L"resource/effekseer/missile.efk",
	L"resource/effekseer/explosion.efk",
	L"resource/effekseer/hit.efk",
	L"resource/effekseer/trajectory.efk",

};

class EffekseerEffect
{
public:
	enum LIST
	{
		UNKNOWN = -1,
		VAPOUR,
		BURNER,
		MISSILE,
		EXPLOSION,
		HIT,
		TRAJECTORY,
		MAX,
	};

	EffekseerEffect(int list);
	~EffekseerEffect();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetMatrix(const DirectX::XMFLOAT4X4& mtx);
	void SetSpeed(float speed);
	void SetRepeat(bool repeat);
	void Play();
	void Paused(bool paused);
	void Stop();
	void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	static void SetViewData(const DirectX::XMFLOAT3& posCam, const DirectX::XMFLOAT3& posCamAt, const DirectX::XMFLOAT3& vecCamUp);

private:
	Effekseer::Manager*					effekseerManager_;
	EffekseerRendererDX11::Renderer*	effekseerRenderer_;
	Effekseer::Effect*					effekseerEffect_;
	Effekseer::Handle					effekseerHandle_;
	
	int		listNumber_ = UNKNOWN;
	bool	repeat_ = false;				//繰りかえし再生するか。
	bool	paused_ = false;				//ポーズ中か。

	Effekseer::Color color_ = { 255, 255, 255, 255 };
	Effekseer::Matrix43 mtx_;
	float speed_ = 1.0f;

	static DirectX::XMFLOAT3		posCam_;
	static DirectX::XMFLOAT3		posCamAt_;
	static DirectX::XMFLOAT3		vecCamUp_;

private:
	void LoadEffect();
};

#endif // !_EFFEKSEER_EFFECT_H_