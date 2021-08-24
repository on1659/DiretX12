
#include "PostProcessingDefine.hlsli"

SamplerState        gssSamplerState		: register(s0);

cbuffer FinalPassConstants : register(b9)
{
	// Tone mapping
    float MiddleGrey : packoffset(c0);
    float LumWhiteSqr : packoffset(c0.y);
    float BloomScale : packoffset(c0.z);
}

struct VS_TEXTURED_OUTPUT 
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float3 ToneMapping(float3 HDRColor) 
{
	float LScale = dot(HDRColor, LUM_FACTOR.xyz);
	LScale *= MiddleGrey / AvgLum[0];
	LScale = (LScale + LScale*LScale / LumWhiteSqr) / (1.0 + LScale);
	return HDRColor * LScale;
}

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target
{
    float3 cColor = HDRDownScaleTex.Sample(gssSamplerState, input.texCoord.xy).xyz;
	cColor += BloomScale * BloomTexture.Sample(gssSamplerState, input.texCoord.xy).xyz;
	return(float4(ToneMapping(cColor), 1.0));
}