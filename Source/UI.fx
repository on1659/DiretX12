
#include "GBuffer.fx"

//UI
cbuffer cbViewOrthoMatrix : register(b1)
{
	matrix gmtxOrthoViewProjection : packoffset(c0);
};
cbuffer cbWorldMatrix     : register(b2)
{
	matrix gmtxWorld : packoffset(c0);
};

Texture2D    gtxtDefault  : register(t0);
SamplerState gssDefault   : register(s0);


//-------------------------------------------------------------------------------------------------------------------------------
struct VS_UI_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_UI_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD0;
};

//---------------------------------------------------------------직교투영 JJY
VS_UI_OUTPUT VSInstancedTexturedColorUI(VS_UI_INPUT input)
{
	VS_UI_OUTPUT output = (VS_UI_OUTPUT)0;
	output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
	output.position = mul(float4(output.positionW, 1.0f), gmtxOrthoViewProjection);// , gmtxOrtho);
	output.texCoord = input.texCoord;
	return(output);
}

float4 PSInstancedTexturedColorUI(VS_UI_OUTPUT input) : SV_Target
{
	return gtxtDefault.Sample(gssDefault, input.texCoord);// *cIllumination;
}


float2 halfTexCoord(float2 texCoord, float2 fRato)
{
	return texCoord * fRato;
}

GBUFFER PSInstancedTexturedColorUI_GBUFFER(VS_UI_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	output.positionW = float4(input.positionW, 0.0f);
	output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);//halfTexCoord(input.texCoord, float2(0.5f, 1.5f)));
	output.normal.w = 10.0f;
	return output;
}


/*1
fxc /E PSInstancedTexturedColorUI_GBUFFER /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo UI.fx
*/