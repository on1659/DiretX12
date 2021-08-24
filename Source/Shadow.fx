

#include "FxDefine.fx"

// --------------------------------------------------------------------------------------
// ���� ������
// --------------------------------------------------------------------------------------

struct VS_INPUT
{
	float4	position    : POSITION;
	float2  tex2dcoord	: TEXCOORD0;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_OUTPUT
{
	float4	position	: SV_POSITION;
	float2  tex2dcoord	: TEXCOORD0;
};



// --------------------------------------------------------------------------------------
// Vertex Shader
// --------------------------------------------------------------------------------------

VS_OUTPUT VSShadowMap(VS_INPUT input)
{
	/**
		* DX10���� ���̹��۸� �����ϴ� ���� �����ϱ� ������
		* �ε��� ���� �����ϴ� �ڵ�� �ܼ��ϰ� ���� ������ View, Projection ��ķ� ������ �ϴ� ������ ������.
		* �� �������� ����Ǹ鼭 ���ŵ� ���̹��۰� �� �ε��� ���� �ȴ�.
	*/

	VS_OUTPUT output = (VS_OUTPUT)0;
	matrix lightWVP = mul(mtxTransform, gmtxViewProjection);
	output.position = mul(float4(input.position, 1.0f), lightWVP);
	output.tex2dcoord = input.tex2dcoord;
	return(output);
}

// --------------------------------------------------------------------------------------
// Pixel Shader
//---------------------------------------------------------------------------------------


void PSShadowMap(VS_OUTPUT input)
{
}
