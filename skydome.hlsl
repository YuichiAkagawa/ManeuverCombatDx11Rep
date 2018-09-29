//**-------------------------------------------------------**
//**
//**                    skydome.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

Texture2D    colorMap : register(t0);		//�J���[�}�b�v
SamplerState samplerState : register(s0);	//�T���v���[

struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct VS_OUT_PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

cbuffer ConstantBuffer
{
	float4x4 mtxWorld;
	float4x4 mtxView;
	float4x4 mtxProj;
	float4 colorValue;
};

VS_OUT_PS_IN VS(VS_IN input)
{
	VS_OUT_PS_IN output = (VS_OUT_PS_IN)0;

	//���[���h�r���[�v���W�F�N�V�����s��
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	//���_���W�ϊ�
	output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	//UV���W
	output.texcoord = input.texcoord;

	//�X�J�C�h�[���F
	//�n�����ߕӂ𔒂�
	float whitePower = 0.7f;
	output.color = input.color * colorValue + (whitePower - abs(input.pos.y));

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	return colorMap.Sample(samplerState, input.texcoord) * input.color;
}