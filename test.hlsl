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

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	output.pos = input.pos;
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