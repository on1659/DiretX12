
#include "Light.fx"
#include "GBuffer.fx"

cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxViewProjection : packoffset(c0);
};

/*
  Animation Sprite;
    x = FPS;
    y = frameCount;
    z = width;
    w = left;
*/
cbuffer cbAnimationSpriteBuffer : register(b6)
{
    float gSpriteAnimation_FPS;
    float gSpriteAnimation_Count;
    float gSpriteAnimation_Width;
    float gSpriteAnimation_Left;
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


//�ν��Ͻ� �������
PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT VS(VS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
    PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT) 0;
    output.normalW   = mul(float3(0, 0, 1), (float3x3) input.mtxTransform);
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position  = mul(float4(output.positionW, 1.0f), gmtxViewProjection);

    float2 tex = input.texCoord;

    if (tex.x == 0)
        tex.x = gSpriteAnimation_Left;
    else
        tex.x = gSpriteAnimation_Left + gSpriteAnimation_Width;

    output.texCoord = tex;
    return (output);
}

