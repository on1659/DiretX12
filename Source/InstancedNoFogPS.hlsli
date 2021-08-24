
#include "Light.fx"
#include "GBuffer.fx"

//-------------------------------------------------------PS Use-------------------------------//
Texture2D gtxtDefault : register(t0);
Texture2D gtxtNormal : register(t1);

SamplerState gssDefault : register(s0);
//-------------------------------------------------------PS Use-------------------------------//


SamplerState LinearSS
{
    Filter = MIN_MAG_MIP_LINEAR;

    AddressU = WRAP;
    AddressV = WRAP;
};


struct VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 tangentW : TANGENT;
    float fDepth : TEXCOORD1;
};

float3 CalcNormal(float3 vNormal, float3 vTangent, float2 vTexcoord)
{
    vNormal = normalize(vNormal);
    vTangent = normalize(vTangent - dot(vTangent, vNormal) * vNormal);
    float3 vBinormal = normalize(cross(vNormal, vTangent));

    float3x3 TBN = float3x3(vTangent, vBinormal, vNormal);
	
    float3 vNormalW = gtxtNormal.Sample(LinearSS, vTexcoord).rgb;
    vNormalW = 2.0f * vNormalW - 1.0f;
	
    return mul(vNormalW, TBN);
}

GBUFFER main(VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(input.normalW, 1.0f);

   // output.normal = float4(CalcNormal(input.normalW, input.tangentW, input.texCoord), 3.0f);

   // float fy = output.normal.y;
   // output.normal.y = output.normal.z;
   // output.normal.z = fy;

    //output.normal.y *= -1;
    //output.normal.z *= -1; 
    //output.normal  = float4(CalcNormal(input.normalW, input.tangentW), 1.0f);

    output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);

    output.shadows.r = input.fDepth;
    output.shadows.g = input.fDepth;
    output.shadows.b = input.fDepth;

    return output;
}
