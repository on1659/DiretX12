
#include "GBuffer.fx"

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
    output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord); //halfTexCoord(input.texCoord, float2(0.5f, 1.5f)));
    output.normal.w = 20.0f;
    return output;
}
