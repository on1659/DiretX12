
#include "Light.fx"
#include "GBuffer.fx"

//-------------------------------------------------------VS Use-------------------------------//
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};
cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};
cbuffer cbTextureMatrix : register(b2)
{
	matrix gmtxTexture : packoffset(c0);
};
cbuffer cbShadowViewProjectionMatrix : register(b7)
{
    matrix gmtxShadowViewProjection : packoffset(c0);
}
//-------------------------------------------------------VS Use-------------------------------//


//-------------------------------------------------------PS Use-------------------------------//
Texture2D    gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);
//-------------------------------------------------------PS Use-------------------------------//



struct VS_INSTANCED_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT
{
	float4 position       : SV_POSITION;
	float3 positionW      : POSITION;
	float3 normalW        : NORMAL;
	float2 texCoord       : TEXCOORD0;
	float4 fDepth         : TEXCOORD1;
};

/*-------------------------------------------------------------------------------------------------------------------------------*/
VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT VSInstanced(VS_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
	VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)input.mtxTransform);
	output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.texCoord = input.texCoord;
	return(output);
}

float4 PSInstanced(VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord) * cIllumination;
	cColor = Fog(cColor, input.positionW);
	return(cColor);
}

GBUFFER PSInstanced_GBUFFER(VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
    output.positionW = float4(input.positionW, 1.0f);
	output.normal = float4(input.normalW, 1.0f);
    output.diffuse   = gtxtDefault.Sample(gssDefault, input.texCoord);
	return output;

}


/*
fxc /E PSInstanced /T ps_5_0 /Od /Zi /Fo PSInstanced.fxo Instanced.fx
*/

/*
fxc /Od /Zi /Ges /T vs_5_0 /E VS /Fo Instanced.fxo Instanced.fx
*/
