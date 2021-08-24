
#include "Light.fx"
#include "GBuffer.fx"

cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxViewProjection : packoffset(c0);
};

cbuffer cbAnimationSpriteBuffer : register(b6)
{
    float padding1;
    float gAlpha;
    float padding2;
    float padding3;
}
struct VS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
};


//ÀÎ½ºÅÏ½Ì ºôº¸µå¿ë
PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT VS(VS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
    PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT) 0;
    output.normalW = mul(float3(0, 0, 1), (float3x3) input.mtxTransform);
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.texCoord = input.texCoord;
    return (output);
}

