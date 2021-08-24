
#include "TSShader.fx"
#include "LimLight.hlsli"
struct TS_CUBE_DS_PS_INPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

GBUFFER main(TS_CUBE_DS_PS_INPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(input.normal, 1.0f);
    output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);
        
    output.diffuse += LimColor(output.positionW, output.normal.xyz, gvCameraPosition);

     //gvCameraPosition;

     //float rimWidth = 0.2f;
     //float3 vCameraPos = normalize(gvCameraPosition - output.positionW);
     //float RimLightColor = smoothstep(1.0 - rimWidth, 1.0f, 1.0f - max(0, dot(output.normal.xyz, vCameraPos)));
    //output.diffuse.r += RimLightColor * 0.8f;
   // output.diffuse.g += RimLightColor * 0.8f;
   // output.diffuse.b += RimLightColor * 0.8f;

    return output;
}


/*
fxc /E PS_TS_GBUFFER /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo NormalnBumpCube.fx
*/
