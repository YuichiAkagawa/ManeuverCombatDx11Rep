//**-------------------------------------------------------**
//**
//**                 hornet_body.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

Texture2D    colorMap : register(t0);		//�J���[�}�b�v
Texture2D    normalMap : register(t1);		//�m�[�}���}�b�v
SamplerState samplerState : register(s0);	//�T���v���[

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

	//���[���h�r���[�v���W�F�N�V�����s��
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	//���_���W�ϊ�
	output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	//T, B, N�Z�o
	float3 normal = normalize(input.normal);
	float3 tangent = normalize(input.tangent);
	float3 binormal = normalize(cross(normal, tangent));

	//�ڋ�ԕϊ��s��
	float3x3 mtxTBN;
	mtxTBN[0] = tangent;
	mtxTBN[1] = binormal;
	mtxTBN[2] = normal;
	float3x3 mtxTransTBN = transpose(mtxTBN);

	//���C�g�Z�o
	float3 posLight = -vecDirLight * 1000000;
	float3 vecLightLocal = mul(float4(posLight, 1.0f), mtxWorldInv).xyz;
	output.vecLightTangent = mul(vecLightLocal, mtxTransTBN);

	//�����x�N�g���Z�o
	float3 posEyeLocal = mul(float4(posEye.xyz, 1.0f), mtxWorldInv).xyz;
	float3 vecEyeLocal = posEyeLocal - input.pos.xyz;
	output.vecEyeTangent = mul(vecEyeLocal, mtxTransTBN);

	//UV���W
	output.texcoord = input.texcoord;

	//�X�y�L�����f�[�^
	output.specularData = specularData.xy;

	//�F
	output.color = input.color;

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	//�m�[�}���}�b�v����l�擾
	float3 normalMapValue = normalMap.Sample(samplerState, input.texcoord).rgb;

	//�������C��
	normalMapValue.g = 1.0f - normalMapValue.g;

	//�@���Z�o
	float3 normalTangent = normalMapValue * 2.0f - 1.0f;
	normalTangent = normalize(normalTangent);

	//���K��
	float3 vecEyeTangent = normalize(input.vecEyeTangent);
	float3 vecLightTangent = normalize(input.vecLightTangent);

	//�n�[�t�x�N�g���̌v�Z
	float3 vecHalf = normalize(vecLightTangent + vecEyeTangent);

	//�X�y�L�����J���[�v�Z
	float specular = pow(max(0.0f, dot(normalTangent, vecHalf)), input.specularData.x) * input.specularData.y;

	//�����o�[�h�g�U����
	float ambient = 0.15f;
	float diffuse = min(max(ambient, dot(normalTangent, vecLightTangent)), 1.0f);

	//�F������
	float colorAdj = 2.0f;

	return colorMap.Sample(samplerState, input.texcoord) * colorAdj * input.color * diffuse + specular;
}