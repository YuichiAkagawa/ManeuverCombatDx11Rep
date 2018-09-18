//**-------------------------------------------------------**
//**
//**                     test.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**
struct VS_IN
{
	float4 pos : POSITION0;
	float4 color : COLOR0;
};

struct VS_OUT
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

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	output.pos = mul(input.pos, mtxWorld);
	output.pos = mul(output.pos, mtxView);
	output.pos = mul(output.pos, mtxProj);
	output.color = input.color;
	return output;
}

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

float4 PS(PS_IN input) : SV_Target
{
	return input.color;
}