
#include "Light.fx"
#include "GBuffer.fx"

cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxViewProjection : packoffset(c0);
};

cbuffer cbShadowsViewProjectionMatrix : register(b7)
{
    matrix gmtxShadowViewProjection : packoffset(c0);
};

cbuffer cbAnimationSpriteBuffer : register(b6)
{
    float gSpriteAnimation_FPS;
    float gSpriteAnimation_Count;
    float gSpriteAnimation_Width;
    float gSpriteAnimation_Left;
}

struct VS_INSTANCED_TEXTURED_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
    float fDepth : TEXCOORD1;
};

VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT VS(VS_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
    VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT) 0;
    output.normalW = mul(input.normal, (float3x3) input.mtxTransform);
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(float4(output.positionW, 1.0f), gmtxShadowViewProjection);
    output.texCoord = input.texCoord;
    return (output);
}
