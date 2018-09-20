//**-------------------------------------------------------**
//**
//**                     fbx.hlsl
//**               Author: Akagawa Yuichi
//**
//**-------------------------------------------------------**

cbuffer ConstantBuffer
{
	float4x4 mtxWorld;
	float4x4 mtxView;
	float4x4 mtxProj;
};

float4 VS(float4 inPos : POSITION) : SV_POSITION
{
	float4 outPos;
	outPos = mul(inPos, mtxWorld);
	outPos = mul(outPos, mtxView);
	outPos = mul(outPos, mtxProj);

	return outPos;
}

// ピクセルシェーダ
float4 PS(float4 Pos : SV_POSITION) : SV_Target{
	return float4(0.5, 0.5, 1.0, 1.0);
}