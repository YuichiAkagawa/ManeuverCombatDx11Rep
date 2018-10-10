//**-------------------------------------------------------**
//**
//**				       sea.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

Texture2D    colorMap : register(t0);		//カラーマップ
Texture2D    normalMap01 : register(t1);	//ノーマルマップ
Texture2D    normalMap02 : register(t2);	//ノーマルマップ

SamplerState samplerState : register(s0);	//サンプラー

struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 tangent : TANGENT0;
	float3 offset : TEXCOORD1;
};

struct VS_OUT_PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float2 texcoord01 : TEXCOORD1;
	float2 texcoord02 : TEXCOORD2;
	float3 vecLightTangent : TEXCOORD3;
	float3 vecEyeTangent : TEXCOORD4;
	float2 specularData : TEXCOORD5;
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
	float4 time;
};

VS_OUT_PS_IN VS(VS_IN input)
{
	VS_OUT_PS_IN output = (VS_OUT_PS_IN)0;

	//頂点座標変換
	float4 posLocal = float4(input.pos.xyz + input.offset, 1.0f);

	//ビュープロジェクション行列
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);
	output.pos = mul(posLocal, mtxWVP);

	//T, B, N算出
	float3 normal = normalize(input.normal);
	float3 tangent = normalize(input.tangent);
	float3 binormal = normalize(cross(normal, tangent));

	//接空間変換行列
	float3x3 mtxTBN;
	mtxTBN[0] = tangent;
	mtxTBN[1] = binormal;
	mtxTBN[2] = normal;
	float3x3 mtxTransTBN = transpose(mtxTBN);

	//ライト算出
	float3 posLight = -vecDirLight * 1000000;
	float3 vecLightLocal = mul(float4(posLight, 1.0f), mtxWorldInv).xyz;
	output.vecLightTangent = mul(vecLightLocal, mtxTransTBN);

	//視線ベクトル算出
	float3 posEyeLocal = mul(float4(posEye.xyz, 1.0f), mtxWorldInv).xyz;
	float3 vecEyeLocal = posEyeLocal - posLocal.xyz;
	output.vecEyeTangent = mul(vecEyeLocal, mtxTransTBN);

	//UV座標
	output.texcoord = input.texcoord;
	float2 texcoord01 = { time.x * -0.006f, time.x * 0.004f };
	output.texcoord01 = input.texcoord + texcoord01;
	float2 texcoord02 = { time.x * 0.004f, time.x * 0.002f };
	output.texcoord02 = input.texcoord + texcoord02;

	//スペキュラデータ
	output.specularData = specularData.xy;

	//色
	output.color = input.color;

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	//ノーマルマップから値取得
	float3 normalMap01Value = normalMap01.Sample(samplerState, input.texcoord01).rgb;
	float3 normalMap02Value = normalMap02.Sample(samplerState, input.texcoord02).rgb;

	//見え方修正
	normalMap01Value.g = 1.0f - normalMap01Value.g;
	normalMap02Value.g = 1.0f - normalMap02Value.g;

	//法線算出
	float3 normalTangent = (normalMap01Value * 2.0f - 1.0f) + (normalMap02Value * 2.0f - 1.0f);
	normalTangent.y *= 3.0f;
	normalTangent = normalize(normalTangent);

	//正規化
	float3 vecEyeTangent = normalize(input.vecEyeTangent);
	float3 vecLightTangent = normalize(input.vecLightTangent);

	//ハーフベクトルの計算
	float3 vecHalf = normalize(vecLightTangent + vecEyeTangent);

	//スペキュラカラー計算
	float specular = pow(max(0.0f, dot(normalTangent, vecHalf)), input.specularData.x) * input.specularData.y;

	//ランバード拡散反射
	float ambient = 0.15f;
	float diffuse = min(max(ambient, dot(normalTangent, vecLightTangent)), 1.0f);

	return colorMap.Sample(samplerState, input.texcoord)* input.color * diffuse + specular;
}