
#include "Light.fx"
#include "GBuffer.fx"

Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);

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

    float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord);

    GBUFFER output = (GBUFFER) 0;
	
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(normalize(input.normalW), 99.0f);

  //  if(cColor.r + cColor.g + cColor.b == 3)
  //      cColor.rgb = float4(0.0f, 0.0f, 0.0f, 1.0f);

    output.diffuse = cColor;

    return output;
}

