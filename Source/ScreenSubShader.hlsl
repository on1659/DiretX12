
#include "Light.fx"
#include "GBuffer.fx"

Texture2D    gtxtShadowMap : register(t5);
SamplerState gssSamplerState : register(s0);

struct VS_TEXTURED_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};


float4 main(VS_TEXTURED_OUTPUT input) : SV_Target
{
    float4 shadowMap = gtxtShadowMap.Sample(gssSamplerState, input.texCoord);
    return shadowMap;
}


