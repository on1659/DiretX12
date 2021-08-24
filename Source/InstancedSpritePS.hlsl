
#include "Light.fx"
#include "GBuffer.fx"

Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);

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

GBUFFER PS(PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{

    float2 tex = input.texCoord;

    tex.x *= gSpriteAnimation_Width;
    tex.x += gSpriteAnimation_Left;
    
    float4 cColor = gtxtDefault.Sample(gssDefault, tex);

    GBUFFER output = (GBUFFER) 0;
	
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(normalize(input.normalW), 5.0f);
    output.diffuse = cColor;
    return output;
}

