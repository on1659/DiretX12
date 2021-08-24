
#include "Light.fx"
#include "GBuffer.fx"

cbuffer cbSkinned : register(b11)
{
// 한 캐릭터당 최대 뼈대 개수는 96
    float4x4 gmtxBoneTransforms[96] : packoffset(c0);
};
//-------------------------------------------------------VS Use-------------------------------//


//-------------------------------------------------------PS Use-------------------------------//
Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);
//-------------------------------------------------------PS Use-------------------------------//


struct VS_SKINNED_INSTANCED_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
    float2 texCoordShadow : TEXCOORD1;
};


float4 PSSkinnedInstanced(VS_SKINNED_INSTANCED_OUTPUT input) : SV_Target
{
    input.normalW = normalize(input.normalW);
    float4 cIllumination = Lighting(input.positionW, input.normalW);
    float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord) * cIllumination;
    cColor = Fog(cColor, input.positionW);
    return (cColor);
}

GBUFFER main(VS_SKINNED_INSTANCED_OUTPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(input.normalW, 1.0f);
    output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);
    return output;
}
