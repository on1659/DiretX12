

#include "FxDefine.fx"

// --------------------------------------------------------------------------------------
// 정점 데이터
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
		* DX10부터 깊이버퍼를 접근하는 것이 가능하기 때문에
		* 셰도우 맵을 생성하는 코드는 단순하게 광원 기준의 View, Projection 행렬로 랜더링 하는 것으로 끝난다.
		* 이 랜더링이 수행되면서 갱신된 깊이버퍼가 곧 셰도우 맵이 된다.
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
