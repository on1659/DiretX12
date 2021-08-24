
#include "GBuffer.fx"

/*
  Animation Sprite;
    x = FPS;
    y = frameCount;
    z = width;
    w = left;
*/
cbuffer cbUIGageBuffer : register(b6)
{
    float gUIGage_RIGHT;
    float3 padding;
}

Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);

struct VS_UI_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float2 texCoord : TEXCOORD0;
};


GBUFFER PS(VS_UI_OUTPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 0.0f);

    float2 tex = input.texCoord;

    output.diffuse = gtxtDefault.Sample(gssDefault, tex); //halfTexCoord(input.texCoord, float2(0.5f, 1.5f)));
   if (tex.x > gUIGage_RIGHT)
   {
        output.diffuse = float4(0, 0, 0, 0);

    }
    output.normal.w = 20.0f;
    return output;
}
