
#include "Light.fx"
#include "GBuffer.fx"

cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxViewProjection : packoffset(c0);
};

Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);

/*
  Animation Sprite;
    x = FPS;
    y = frameCount;
    z = width;
    w = left;
*/
cbuffer cbAnimationSpriteBuffer
{
    float4 gf4AnimationSprite;
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

PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT VS_BillBoardColor(VS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
    PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT) 0;
    output.normalW = mul(float3(0, 0, 1), (float3x3) input.mtxTransform);
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);

    float2 tex = input.texCoord;
    if (tex.x == 0)
        tex.x = gf4AnimationSprite.w;
    else
        tex.x = gf4AnimationSprite.w + gf4AnimationSprite.z;
  
    output.texCoord = input.texCoord;
    return (output);
}

GBUFFER PS_BillBoardColor_GBUFFER(PS_BILLBOARD_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{

    float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord);

    GBUFFER output = (GBUFFER) 0;
	
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(normalize(input.normalW), 1.0f);
    output.diffuse = cColor;


    return output;
}
