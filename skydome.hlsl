//**-------------------------------------------------------**
//**
//**                    skydome.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

Texture2D    colorMap : register(t0);		//カラーマップ
SamplerState samplerState : register(s0);	//サンプラー

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

	//ワールドビュープロジェクション行列
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	//頂点座標変換
	output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	//UV座標
	output.texcoord = input.texcoord;

	//スカイドーム色
	//地平線近辺を白く
	float whitePower = 0.7f;
	output.color = input.color * colorValue + (whitePower - abs(input.pos.y));

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	return colorMap.Sample(samplerState, input.texcoord) * input.color;
}