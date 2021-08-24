
#include "TSShader.fx"

struct TS_CUBE_VS_OUTPUT
{
    float3 positionW : POSITION;
};
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

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HSConstant_Cube")]
[maxtessfactor(64.0f)]
TS_CUBE_HS_INPUT main(InputPatch<TS_CUBE_VS_OUTPUT, 4> input, uint i : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
    TS_CUBE_HS_INPUT output = (TS_CUBE_HS_INPUT) 0;
    output.positionW = input[i].positionW;
    switch (i)
    {
        case 0:
            output.texCoord = float2(0.0f, 0.0f);
            break;
        case 1:
            output.texCoord = float2(1.0f, 0.0f);
            break;
        case 2:
            output.texCoord = float2(0.0f, 1.0f);
            break;
        case 3:
            output.texCoord = float2(1.0f, 1.0f);
            break;
    }

    return output;
}

TS_CUBE_HSC_INPUT HSConstant_Cube(InputPatch<TS_CUBE_VS_OUTPUT, 4> input, uint patchID : SV_PrimitiveID)
{
    TS_CUBE_HSC_INPUT output;
    output.fTessEdges[0] = 64;
    output.fTessEdges[1] = 64;
    output.fTessEdges[2] = 64;
    output.fTessEdges[3] = 64;
    output.fTessInsides[0] = 64;
    output.fTessInsides[1] = 64;

    return output;
}
