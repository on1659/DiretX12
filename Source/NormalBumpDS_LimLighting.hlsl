
#include "TSShader.fx"

struct TS_CUBE_HS_INPUT
{
    float3 positionW : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct TS_CUBE_HSC_INPUT
{
    float fTessEdges[4] : SV_TessFactor;
    float fTessInsides[2] : SV_InsideTessFactor;
};
struct TS_CUBE_DS_PS_INPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};


[domain("quad")]
TS_CUBE_DS_PS_INPUT main(TS_CUBE_HSC_INPUT input, float2 uv : SV_DomainLocation, OutputPatch<TS_CUBE_HS_INPUT, 4> patch)
{
    TS_CUBE_DS_PS_INPUT output = (TS_CUBE_DS_PS_INPUT) 0;

    float3 fTangentW = normalize(patch[1].positionW - patch[0].positionW);
    float3 fBinormalW = normalize(patch[2].positionW - patch[0].positionW);
    float3 fNormalW = cross(fTangentW, fBinormalW);

    float3x3 TBN = float3x3(fTangentW, fBinormalW, fNormalW);

    output.texCoord = lerp(lerp(patch[0].texCoord, patch[1].texCoord, uv.x), lerp(patch[2].texCoord, patch[3].texCoord, uv.x), uv.y);


    float3 normal = gtxtNormal.SampleLevel(gssDefault, output.texCoord, 0).xyz;
    normal = normal * 2.0f - 1.0f;
    normal = mul(normal, TBN);
    normal = normalize(normal);

    output.positionW = lerp(lerp(patch[0].positionW, patch[1].positionW, uv.x), lerp(patch[2].positionW, patch[3].positionW, uv.x), uv.y);

    float2 tex = mul(float4(output.texCoord, 0.0f, 1.0f), gmtxTexture).xy;
    float fDM = gtxtDisplacement.SampleLevel(gssDefault, tex, 0).r * 2.0f;
	//float fDM = gtxtDisplacement.SampleLevel(gssDefault, output.texCoord, 0).r * 2.0f;

    if (output.texCoord.x == 0.0f || output.texCoord.y == 0.0f || output.texCoord.x == 1.0f || output.texCoord.y == 1.0f)
    {
    }
    else
    {
		//output.positionW += fNormalW * fDM * (cbSize.x);
        output.positionW += fNormalW * fDM * 0.2f;
    }
	//patch[0].size + patch[1].size + patch[2].size + patch[3].size
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.normal = normal;

    return output;
}