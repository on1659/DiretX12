
#include "GBuffer.fx"
#include "Light.fx"
cbuffer cbViewProjectionMatrix  : register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};
//cbuffer cbViewOrthoMatrix		: register(b1)			{	matrix		gmtxOrthoViewProjection : packoffset(c0);};
cbuffer cbWorldMatrix : register(b2)
{
    matrix gmtxWorld : packoffset(c0);
};
cbuffer cbTextureMatrix : register(b3)
{
    matrix gmtxTexture : packoffset(c0);
};

Texture2D gtxtDefault : register(t0);
Texture2D gtxtDefaultDetail : register(t1);

struct VS_WATER_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_WATER_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

VS_WATER_OUTPUT VSWaterColor(VS_WATER_INPUT input)
{
	VS_WATER_OUTPUT output = (VS_WATER_OUTPUT)0;
	return output;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = mul(float4(input.texCoordDetail, 0.0f, 1.0f), gmtxTexture).xy;

	return(output);
}
SamplerState gssDefault : register(s0);
SamplerState gssDefaultDetail : register(s1);

GBUFFER PSWaterFog_GBUFFER(VS_WATER_OUTPUT input) : SV_Target
{
    float4 cBaseTexColor = gtxtDefault.Sample(gssDefault, input.texCoordBase);
    float4 cDetailTexColor = gtxtDefaultDetail.Sample(gssDefaultDetail, input.texCoordDetail);
    
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(input.normalW, 1.0f);
    output.diffuse = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
    return output;
}
//-----------------
/*
fxc /E VSWaterColor /T vs_5_0 /Od /Zi /Fo CompiledGs.fxo Water.fx
*/

