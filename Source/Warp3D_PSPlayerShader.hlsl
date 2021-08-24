


#include "Light.fx"
#include "LimLight.hlsli"
#include "GBuffer.fx"


SamplerState gssDefault : register(s0);

Texture2D gtxtDefault : register(t0);
Texture2D gtxtNormal : register(t1);
Texture2D gtxtDisplacement : register(t2);


SamplerState LinearSS
{
    Filter = MIN_MAG_MIP_LINEAR;

    AddressU = WRAP;
    AddressV = WRAP;
};


struct VS_SKINNED_PLAYER_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 tangentW : TANGENT;

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

GBUFFER main(VS_SKINNED_PLAYER_OUTPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 1.0f);
    output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);
    output.normal = float4(normalize(input.normalW), 1.0f);
    output.normal = float4(CalcNormal(input.normalW.xyz, input.tangentW, input.texCoord), 1.0f); 
    output.diffuse.a = 15.0f;

    output.diffuse += LimColor(output.positionW, output.normal.xyz, gvCameraPosition);

    return output;
}
