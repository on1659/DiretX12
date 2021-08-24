
#include "Light.fx"
#include "GBuffer.fx"

//-------------------------------------------------------VS Use-------------------------------//
cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxViewProjection : packoffset(c0);
};
cbuffer cbWorldMatrix : register(b1)
{
    matrix gmtxWorld : packoffset(c0);
};
cbuffer cbTextureMatrix : register(b2)
{
    matrix gmtxTexture : packoffset(c0);
};

cbuffer cbShaodwViewProjectionMatrix : register(b7)
{
    matrix gmtxShadowViewProjection : packoffset(c0);
};
//-------------------------------------------------------VS Use-------------------------------//

struct VS_INSTANCED_TEXTURED_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 tangentW : TANGENT;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 tangentW : TANGENT;
  //  float  fDepth : POSITION1;
};

/*-------------------------------------------------------------------------------------------------------------------------------*/
VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT main(VS_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
    VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT) 0;
    output.normalW = mul(float4(input.normal, 1.0f), input.mtxTransform).xyz;
    output.normalW = mul(input.normal, (float3x3) input.mtxTransform);
      
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.texCoord = input.texCoord;

    //output.tangentW = mul(float4(input.tangentW, 1.0f), input.mtxTransform).xyz;
    //float4 f4 = mul(float4(output.positionW, 1.0f), gmtxShadowViewProjection);
    //output.fDepth = 1.0f;
   //f4.w;
    return (output);
}
