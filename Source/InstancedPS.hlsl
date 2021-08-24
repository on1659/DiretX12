
#include "Light.fx"
#include "GBuffer.fx"
#include "NormalCalc.hlsli"
//-------------------------------------------------------PS Use-------------------------------//
Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);
//-------------------------------------------------------PS Use-------------------------------//

struct VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 tangentW : TANGENT;
  //  float fDepth : POSITION1;
};


GBUFFER main(VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 1.0f);
    output.normal    = float4(input.normalW, 1.0f);

  //  output.normal = float4(CalcNormal(input.normalW, input.tangentW), 1.0f);


    float fy = output.normal.y;
    output.normal.y = output.normal.z;
    output.normal.z = fy;

    //output.normal.y *= -1;
    //output.normal.z *= -1;
    //output.normal  = float4(CalcNormal(input.normalW, input.tangentW, input.texCoord), 1.0f);

    output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);

   // output.shadows.g = input.fDepth;
    //output.shadows.g = input.fDepth;
    //output.shadows.b = input.fDepth;

    return output;
}
