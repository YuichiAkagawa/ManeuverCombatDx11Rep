//**-------------------------------------------------------**
//**
//**                 hornet_body.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

Texture2D    colorMap : register(t0);		//カラーマップ
Texture2D    normalMap : register(t1);		//ノーマルマップ
SamplerState samplerState : register(s0);	//サンプラー

struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 tangent : TANGENT0;
};

struct VS_OUT_PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float3 vecLightTangent : TEXCOORD1;
	float3 vecEyeTangent : TEXCOORD2;
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
	float3 vecEyeLocal = posEyeLocal - input.pos.xyz;
	output.vecEyeTangent = mul(vecEyeLocal, mtxTransTBN);

	//UV座標
	output.texcoord = input.texcoord;

	//スペキュラデータ
	output.specularData = specularData.xy;

	//色
	output.color = input.color;

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	//ノーマルマップから値取得
	float3 normalMapValue = normalMap.Sample(samplerState, input.texcoord).rgb;

	//見え方修正
	normalMapValue.g = 1.0f - normalMapValue.g;

	//法線算出
	float3 normalTangent = normalMapValue * 2.0f - 1.0f;
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

	//色調調整
	float colorAdj = 2.0f;

	return colorMap.Sample(samplerState, input.texcoord) * colorAdj * input.color * diffuse + specular;
}