//**-------------------------------------------------------**
//**
//**                     grid.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
};

struct VS_OUT_PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

cbuffer ConstantBuffer
{
	float4x4 mtxWorld;
	float4x4 mtxView;
	float4x4 mtxProj;
};

VS_OUT_PS_IN VS(VS_IN input)
{
	VS_OUT_PS_IN output = (VS_OUT_PS_IN)0;

	//���[���h�r���[�v���W�F�N�V�����s��
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	//���_���W�ϊ�
	output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	//�F
	output.color = input.color;

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	return input.color;
}