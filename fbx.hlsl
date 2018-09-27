//**-------------------------------------------------------**
//**
//**                     fbx.hlsl
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
	float4 boneIndex : TEXCOORD1;
	float4 weight : TEXCOORD2;
};

struct VS_OUT_PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
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
	VS_OUT_PS_IN output;

	////t, b, n�Z�o
	//float3 normal = normalize(input.normal);
	//float3 tangent = normalize(input.tangent);
	//float3 binormal = normalize(cross(normal, tangent));

	////���[���h�r���[�v���W�F�N�V�����s��
	//float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	////���_���W�ϊ�
	//output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	////���C�g�x�N�g���Z�o
	//float3 vecLight = mul(-vecDirLight.xyz, mtxWorld);
	//vecLight = normalize(vecLight);
	////output.vecLight.x = dot(vecLight, tangent);
	////output.vecLight.y = dot(vecLight, binormal);
	////output.vecLight.z = dot(vecLight, normal);
	////output.vecLight = normalize(output.vecLight);
	//output.vecLight = vecLight;

	////���_�ւ̃x�N�g���Z�o
	//float3 vecEye = posEye.xyz - mul(input.pos.xyz, mtxWorld);
	//vecEye = normalize(vecEye);
	////output.vecEye.x = dot(vecEye, tangent);
	////output.vecEye.y = dot(vecEye, binormal);
	////output.vecEye.z = dot(vecEye, normal);
	////output.vecEye = normalize(output.vecEye);
	//output.vecEye = vecEye;

	////���_�F
	//output.color = input.color;

	////UV���W
	//output.texcoord = input.texcoord;

	////�X�y�L�����f�[�^
	//output.specularData = specularData.xy;

	////�@��
	//output.normal = mul(normal, mtxWorld);
	//output.normal = normalize(output.normal);

	///////////////////////////////////////






	///////////////////////////////////////

	////���[���h�r���[�v���W�F�N�V�����s��
	//float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	////���_���W�ϊ�
	//output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	////���C�g�x�N�g���Z�o
	//float3 vecLight = -vecDirLight.xyz;
	//output.vecLight = normalize(vecLight);

	////���_�ւ̃x�N�g���Z�o
	//float3 vecEye = posEye.xyz - mul(input.pos.xyz, mtxWorld);
	//output.vecEye = normalize(vecEye);

	////UV���W
	//output.texcoord = input.texcoord;

	////�X�y�L�����f�[�^
	//output.specularData = specularData.xy;

	////�@���Z�o
	//float3 normal = input.normal;
	//output.normal = mul(normal, mtxWorld);
	//output.normal = normalize(output.normal);

	////�F
	//output.color = input.color;

	///////////////////////////////////////





	/////////////////////////////////////

	//���[���h�r���[�v���W�F�N�V�����s��
	float4x4 mtxWVP = mul(mul(mtxWorld, mtxView), mtxProj);

	//���_���W�ϊ�
	output.pos = mul(float4(input.pos, 1.0f), mtxWVP);

	//T, B, N�Z�o
	float3 normal = normalize(input.normal);
	float3 tangent = normalize(input.tangent);
	float3 binormal = normalize(cross(normal, tangent));

	float3x3 tbn;
	tbn[0] = tangent;
	tbn[1] = binormal;
	tbn[2] = normal;
	float3x3 toTangentSpace = transpose(tbn);

	//�����x�N�g���Z�o
	float3 posEyeLocal = mul(float4(posEye.xyz, 1.0f), mtxWorldInv).xyz;
	float3 vecEyeLocal = posEyeLocal - input.pos.xyz;
	output.vecEyeTangent = mul(vecEyeLocal, toTangentSpace);

	//���C�g�x�N�g���Z�o
	float3 vecLightLocal = mul(float4(-vecDirLight.xyz, 1.0f), mtxWorldInv).xyz;
	output.vecLightTangent = mul(vecLightLocal, toTangentSpace);

	//UV���W
	output.texcoord = input.texcoord;

	//�X�y�L�����f�[�^
	output.specularData = specularData.xy;

	//�F
	output.color = input.color;

	/////////////////////////////////////


	return output;
}

float4 PS(VS_OUT_PS_IN input) : SV_Target
{
	////�n�[�t�x�N�g���̌v�Z
	//float3 vecHalf = normalize(input.vecLight + input.vecEye);

	////�X�y�L�����J���[�v�Z
	//float specular = pow(max(0.0f, dot(input.normal, vecHalf)), input.specularData.x) * input.specularData.y;

	//return colorMap.Sample(samplerState, input.texcoord) * input.color * min(max(0.1f, dot(input.normal, input.vecLight)), 1.0f) + specular;

	///////////////////////////////////////





	///////////////////////////////////////

	//�m�[�}���}�b�v����@���擾
	float3 normalTangent = 2.0f * normalMap.Sample(samplerState, input.texcoord).xyz - 1.0f;
	normalTangent = normalize(normalTangent);

	//���K��
	float3 vecEyeTangent = normalize(input.vecEyeTangent);
	float3 vecLightTangent = normalize(input.vecLightTangent);

	//�n�[�t�x�N�g���̌v�Z
	float3 vecHalf = normalize(vecLightTangent + vecEyeTangent);

	//�X�y�L�����J���[�v�Z
	float specular = pow(max(0.0f, dot(normalTangent, vecHalf)), input.specularData.x) * input.specularData.y;

	return colorMap.Sample(samplerState, input.texcoord) * input.color *  min(max(0.1f, dot(normalTangent, vecLightTangent)), 1.0f) +specular;
}