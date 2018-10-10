//**-------------------------------------------------------**
//**
//**				       sea.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

Texture2D    colorMap : register(t0);		//�J���[�}�b�v
Texture2D    normalMap01 : register(t1);	//�m�[�}���}�b�v
Texture2D    normalMap02 : register(t2);	//�m�[�}���}�b�v

SamplerState samplerState : register(s0);	//�T���v���[

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

	//���_���W�ϊ�
	float4 posLocal = float4(input.pos.xyz + input.offset, 1.0f);

	//�r���[�v���W�F�N�V�����s��
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);
	output.pos = mul(posLocal, mtxWVP);

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
	float3 vecEyeLocal = posEyeLocal - posLocal.xyz;
	output.vecEyeTangent = mul(vecEyeLocal, mtxTransTBN);

	//UV���W
	output.texcoord = input.texcoord;
	float2 texcoord01 = { time.x * -0.006f, time.x * 0.004f };
	output.texcoord01 = input.texcoord + texcoord01;
	float2 texcoord02 = { time.x * 0.004f, time.x * 0.002f };
	output.texcoord02 = input.texcoord + texcoord02;

	//�X�y�L�����f�[�^
	output.specularData = specularData.xy;

	//�F
	output.color = input.color;

	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	//�m�[�}���}�b�v����l�擾
	float3 normalMap01Value = normalMap01.Sample(samplerState, input.texcoord01).rgb;
	float3 normalMap02Value = normalMap02.Sample(samplerState, input.texcoord02).rgb;

	//�������C��
	normalMap01Value.g = 1.0f - normalMap01Value.g;
	normalMap02Value.g = 1.0f - normalMap02Value.g;

	//�@���Z�o
	float3 normalTangent = (normalMap01Value * 2.0f - 1.0f) + (normalMap02Value * 2.0f - 1.0f);
	normalTangent.y *= 3.0f;
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

	return colorMap.Sample(samplerState, input.texcoord)* input.color * diffuse + specular;
}