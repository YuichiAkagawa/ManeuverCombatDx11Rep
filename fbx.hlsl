//**-------------------------------------------------------**
//**
//**                     fbx.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 binormal : BINORMAL0;
	float4 boneIndex : TEXCOORD1;
	float4 weight : TEXCOORD2;
};

struct VS_OUT
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
	float4 vecLight;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, mtxWorld);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);
	output.pos = pos;

	float3 normal = mul(input.normal, mtxWorld);
	normal = normalize(normal);

	float color = saturate(dot(normal, (float3)vecLight));
	color = (color * 0.5f + 0.5f) * input.color;

	output.color = float4(color, color, color, 1.0f);

	output.texcoord = input.texcoord;

	return output;
}

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

Texture2D    texture2d : register(t0);	//テクスチャー
SamplerState samplerState : register(s0);	//サンプラー

float4 PS(PS_IN input) : SV_Target
{
	return texture2d.Sample(samplerState, input.texcoord) * input.color;
}