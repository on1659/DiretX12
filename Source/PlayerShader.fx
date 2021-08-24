
#include "FxDefine.fx"
#include "Light.fx"
#include "GBuffer.fx"

//-------------------------------------------------------VS Use-------------------------------//

cbuffer cbSkinned				: register(b11)
{
	// 한 캐릭터당 최대 뼈대 개수는 96
	float4x4 gmtxBoneTransforms[96] : packoffset(c0);
};
//-------------------------------------------------------VS Use-------------------------------//


struct VS_SKINNED_PLAYER_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
 //   float3 tangentW : TANGENT;

	uint4 boneIndices	: BONE_INDEICES;
	float3 weights		: BONE_WEIGHTS;
};

struct VS_SKINNED_PLAYER_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;

 //   float3 tangentW : TANGENT;

};


float3 CalcNormal(float3 vNormal, float3 vTangent)
{
    vNormal = normalize(vNormal);
    vTangent = normalize(vTangent - dot(vTangent, vNormal) * vNormal);
    float3 vBinormal = normalize(cross(vNormal, vTangent));

    float3x3 TBN = float3x3(vTangent, vBinormal, vNormal);
	
    float3 vNormalW = vNormal;
    vNormalW = 2.0f * vNormalW - 1.0f;
	
    return mul(vNormalW, TBN);
}

// --------------------------------------------------------------------------------------
// Vertex Shader
// --------------------------------------------------------------------------------------


VS_SKINNED_PLAYER_OUTPUT VSSkinnedPlayer(VS_SKINNED_PLAYER_INPUT input)
{
	VS_SKINNED_PLAYER_OUTPUT output = (VS_SKINNED_PLAYER_OUTPUT)0;
	
	// 배열을 초기화한다. 그렇지 않으면 SV_POSITION에 관련한 경고 메시지가 나온다.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.weights.x;
	weights[1] = input.weights.y;
	weights[2] = input.weights.z;
	weights[3] = 1.0f - (weights[0] + weights[1] + weights[2]);//input.weights.w;
	
	// 정점 블랜딩 수행
	float3 posL = float3(0.0f, 0.03f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < 4; ++i)
	{
		posL += weights[i] * mul(float4(input.position, 1.0f), gmtxBoneTransforms[input.boneIndices[i]]).xyz;
	
		normalL += weights[i] * mul(input.normal, (float3x3)gmtxBoneTransforms[input.boneIndices[i]]);
	}
	
	output.positionW = mul(float4(posL, 1.0f), gmtxWorld).xyz;
	output.normalW = mul(normalL, (float3x3)gmtxWorld);
	
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	output.texCoord = input.texCoord;

    //output.tangentW = mul(input.tangentW, (float3x3) gmtxWorld);

	return output;
}

// --------------------------------------------------------------------------------------
// Pixel Shader
//---------------------------------------------------------------------------------------

float4 PSSkinnedPlayer(VS_SKINNED_PLAYER_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	return (gtxtDefault.Sample(gssDefault, input.texCoord) * cIllumination);
}

GBUFFER PSSkinnedPlayer_GBUFFER(VS_SKINNED_PLAYER_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	output.positionW = float4(input.positionW, 1.0f);
	output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);
	output.normal = float4(normalize(input.normalW),1.0f);

    // output.normal = CalcNormal(input.normalW, input.tangentW);

    float rimWidth = 0.8f;
    float3 vCameraPos = normalize(gvCameraPosition - output.positionW);
    float RimLightColor = smoothstep(1.0 - rimWidth, 1.0f, 1.0f - max(0, dot(output.normal.xyz, vCameraPos)));
    output.diffuse.r += RimLightColor * 0.1f;
    output.diffuse.g += RimLightColor * 0.1f;
    output.diffuse.b += RimLightColor * 0.1f;

	return output;
}

/*
fxc /E PSSkinnedInstanced /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo PlayerShader.fx
*/