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

struct VS_OUT_PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 vecLight : TEXCOORD1;
	float3 vecEye : TEXCOORD2;
	float2 specularData : TEXCOORD3;
};

cbuffer ConstantBuffer
{
	float4x4 mtxWorld;
	float4x4 mtxView;
	float4x4 mtxProj;
	float4 vecLight;
	float4 posEye;
	float4 specularData;
};

VS_OUT_PS_IN VS(VS_IN input)
{
	VS_OUT_PS_IN output;
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, mtxWorld);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);
	output.pos = pos;

	output.normal = mul(input.normal, mtxWorld);
	output.normal = normalize(output.normal);

	output.color = input.color;

	output.texcoord = input.texcoord;

	output.vecLight = normalize(-vecLight.xyz);

	output.vecEye = posEye.xyz - mul(input.pos.xyz, mtxWorld);
	output.vecEye = normalize(output.vecEye);

	output.specularData = specularData.xy;

	return output;
}

Texture2D    texture2d : register(t0);	//テクスチャー
SamplerState samplerState : register(s0);	//サンプラー

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	//ハーフベクトルの計算
	float3 vecHalf = normalize(input.vecLight + input.vecEye);

	//スペキュラカラー計算
	float specular = pow(max(0.0f, dot(input.normal, vecHalf)), input.specularData.x) * input.specularData.y;

	//明度算出
	float brightness = dot(input.normal, input.vecLight) + specular;

	return min(1.0f, max(0.0f, texture2d.Sample(samplerState, input.texcoord) * brightness));
}