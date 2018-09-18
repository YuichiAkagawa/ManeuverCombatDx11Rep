//**-------------------------------------------------------**
//**
//**                     cube.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
struct VS_IN
{
	float4 pos : POSITION0;
	float4 normal : NORMAL0;
	float2 texcoord : TEXCOORD0;
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
	output.pos = mul(input.pos, mtxWorld);
	output.pos = mul(output.pos, mtxView);
	output.pos = mul(output.pos, mtxProj);

	float3 normal = mul(input.normal, mtxWorld).xyz;
	normal = normalize(normal);

	float color = saturate(dot(normal, (float3)vecLight));
	color = color * 0.5f + 0.5f;

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