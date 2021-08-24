
#include "FxDefine.fx"

cbuffer cbSkinned : register(b11)
{
	// 한 캐릭터당 최대 뼈대 개수는 96
    float4x4 gmtxBoneTransforms[96] : packoffset(c0);
};

struct VS_SKINNED_PLAYER_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 tangentW : TANGENT;

    uint4 boneIndices : BONE_INDEICES;
    float3 weights : BONE_WEIGHTS;
};

struct VS_SKINNED_PLAYER_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
    
    float3 tangentW : TANGENT;

};

VS_SKINNED_PLAYER_OUTPUT main(VS_SKINNED_PLAYER_INPUT input)
{
    VS_SKINNED_PLAYER_OUTPUT output = (VS_SKINNED_PLAYER_OUTPUT) 0;
	
	// 배열을 초기화한다. 그렇지 않으면 SV_POSITION에 관련한 경고 메시지가 나온다.
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.weights.x;
    weights[1] = input.weights.y;
    weights[2] = input.weights.z;
    weights[3] = 1.0f - (weights[0] + weights[1] + weights[2]); //input.weights.w;
	
	// 정점 블랜딩 수행
    float3 posL = float3(0.0f, 0.03f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
	
    for (int i = 0; i < 4; ++i)
    {
        posL += weights[i] * mul(float4(input.position, 1.0f), gmtxBoneTransforms[input.boneIndices[i]]).xyz;
	
        normalL += weights[i] * mul(input.normal, (float3x3) gmtxBoneTransforms[input.boneIndices[i]]);
    }
	
    output.positionW = mul(float4(posL, 1.0f), gmtxWorld).xyz;
    output.normalW = mul(normalL, (float3x3) gmtxWorld);
	
    output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
    output.texCoord = input.texCoord;

    output.tangentW = mul(input.tangentW, (float3x3) gmtxWorld);

    //output.tangentW = mul(input.tangentW, (float3x3) gmtxWorld);

    return output;
}
