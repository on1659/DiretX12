
#include "Light.fx"
#include "GBuffer.fx"

Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);

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

float4 PS(PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
   float4 cColor =  gtxtDefault.Sample(gssDefault, input.texCoord);
    return cColor;

}

