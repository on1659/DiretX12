
#include "Light.fx"
#include "GBuffer.fx"

//-------------------------------------------------------VS Use-------------------------------//
cbuffer cbViewProjectionMatrix	: register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};

cbuffer cbWorldMatrix			: register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};

cbuffer cbTextureMatrix			: register(b2)
{
	matrix gmtxTexture : packoffset(c0);
};

//-------------------------------------------------------VS Use-------------------------------//


//-------------------------------------------------------PS Use-------------------------------//
Texture2D    gtxtDefault : register(t0);
Texture2D    gtxAnimation : register(t10);	//SkinnedAnimation GlobalTransform Data
SamplerState gssDefault : register(s0);
//-------------------------------------------------------PS Use-------------------------------//

struct VS_SKINNED_INSTANCED_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;

	uint4 boneIndices	: BONE_INDEICES;
	float3 weights		: BONE_WEIGHTS;

	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_SKINNED_INSTANCED_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;
	float2 texCoordShadow : TEXCOORD1;
};

float4x4 decodeMatrix(float3x4 encodedMatrix)
{
	return float4x4(float4(encodedMatrix[0].xyz, 0),
		float4(encodedMatrix[1].xyz, 0),
		float4(encodedMatrix[2].xyz, 0),
		float4(encodedMatrix[0].w, encodedMatrix[1].w, encodedMatrix[2].w, 1));
}

float4x4 loadBoneMatrix(uint boneIndex, uint keyframeNum)
{
	float4x4 rval = float4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	uint baseU = keyframeNum;
	uint baseV = ((boneIndex * 3));

	float4 mat1 = gtxAnimation.Load(uint3(baseU, baseV, 0));
	float4 mat2 = gtxAnimation.Load(uint3(baseU, baseV + 1, 0));
	float4 mat3 = gtxAnimation.Load(uint3(baseU, baseV + 2, 0));

	rval = decodeMatrix(float3x4(mat1, mat2, mat3));

	return rval;
}


// --------------------------------------------------------------------------------------
// Vertex Shader
// --------------------------------------------------------------------------------------

VS_SKINNED_INSTANCED_OUTPUT VSSkinnedInstanced(VS_SKINNED_INSTANCED_INPUT input)
{
	VS_SKINNED_INSTANCED_OUTPUT output = (VS_SKINNED_INSTANCED_OUTPUT)0;

	// 배열을 초기화한다. 그렇지 않으면 SV_POSITION에 관련한 경고 메시지가 나온다.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.weights.x;
	weights[1] = input.weights.y;
	weights[2] = input.weights.z;
	weights[3] = 1.0f - (weights[0] + weights[1] + weights[2]);//input.weights.w;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		uint keyframeNum = (uint)input.mtxTransform[3][3];

		float4x4 gtmx = loadBoneMatrix(input.boneIndices[i], keyframeNum);

		posL += weights[i] * mul(float4(input.position, 1.0f), gtmx).xyz;

		normalL += weights[i] * mul(input.normal, (float3x3)gtmx);
	}

	// 월드 공간 변환 전에 현재 키프레임을 넣어두었던 4행 4열을 1로 수정  
	if (input.mtxTransform[3][3] != 1.0f) input.mtxTransform[3][3] = 1.0f;

	// 월드 공간으로 변환한다.
	output.positionW = mul(float4(posL, 1.0f), input.mtxTransform).xyz;
	output.normalW = mul(normalL, (float3x3)input.mtxTransform);
	output.normalW = output.normalW * 0.2f;

	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	output.texCoord = input.texCoord;

	return output;
}

// --------------------------------------------------------------------------------------
// Pixel Shader
//---------------------------------------------------------------------------------------

float4 PSSkinnedInstanced(VS_SKINNED_INSTANCED_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord) * cIllumination;
	cColor = Fog(cColor, input.positionW);
	return(cColor);
}

GBUFFER PSSkinnedInstanced_GBUFFER(VS_SKINNED_INSTANCED_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	output.positionW = float4(input.positionW, 1.0f);
	output.normal = float4(input.normalW, 1.0f);
	//output.normal = float4(0.0f,0.0f, 0.0f, 1.0f);
	output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);
	output.ambient = gMaterial.m_cAmbient;
	output.specular = gMaterial.m_cSpecular;
	return output;
}

/*
fxc /E PSSkinnedInstanced /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo MonsterSkinnedInstancing.fx
*/