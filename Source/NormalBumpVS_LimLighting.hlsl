
#include "TSShader.fx"

struct TS_CUBE_VS_INPUT
{
    float3 positionW : POSITION;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct TS_CUBE_VS_OUTPUT
{
    float3 positionW : POSITION;
};

TS_CUBE_VS_OUTPUT main(TS_CUBE_VS_INPUT input)
{
    TS_CUBE_VS_OUTPUT output;
    output.positionW = mul(float4(input.positionW, 1.0f), input.mtxTransform).xyz;
    return output;
}
