//**-------------------------------------------------------**
//**
//**                 hornet_glass.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
};

struct VS_OUT_PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float3 vecLight : TEXCOORD1;
	float3 vecEye : TEXCOORD2;
	float2 specularData : TEXCOORD3;
};

cbuffer ConstantBuffer
{
	float4x4 mtxWorld;
	float4x4 mtxWorldInv;
	float4x4 mtxView;
	float4x4 mtxProj;
	float4 vecDirLight;
	float4 posEye;
	float4 specularData;
};

VS_OUT_PS_IN VS(VS_IN input)
{
	VS_OUT_PS_IN output = (VS_OUT_PS_IN)0;

	//ワールドビュープロジェクション行列
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	//頂点座標変換
	output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	//法線
	output.normal = input.normal;

	//ライト算出
	float3 posLight = -vecDirLight * 1000000;
	output.vecLight = mul(float4(posLight, 1.0f), mtxWorldInv).xyz;

	//視線ベクトル算出
	float3 posEyeLocal = mul(float4(posEye.xyz, 1.0f), mtxWorldInv).xyz;
	output.vecEye = posEyeLocal - input.pos.xyz;

	//スペキュラデータ
	output.specularData = specularData.xy;

	//色
	output.color = input.color;

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{

	//正規化
	float3 vecEye = normalize(input.vecEye);
	float3 vecLight = normalize(input.vecLight);
	float3 normal = normalize(input.normal);

	//ハーフベクトルの計算
	float3 vecHalf = normalize(vecLight + vecEye);

	//スペキュラカラー計算
	float specular = pow(max(0.0f, dot(normal, vecHalf)), input.specularData.x) * input.specularData.y;

	//ランバード拡散反射
	float ambient = 0.15f;
	float impact = 0.5f;
	float diffuse = min(max(ambient, dot(normal, vecLight) * impact), 1.0f);

	return input.color * diffuse + specular;
}