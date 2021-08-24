
#include "TSShader.fx"

struct TS_CUBE_VS_INPUT
{
	float3 positionW : POSITION;
    float   size : SIZE;
	float4x4 mtxTransform : INSTANCEPOS;
};
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
struct TS_CUBE_DS_PS_INPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};
TS_CUBE_VS_OUTPUT VS_TS(TS_CUBE_VS_INPUT input)
{
	TS_CUBE_VS_OUTPUT output;
	output.positionW = mul(float4(input.positionW, 1.0f), input.mtxTransform).xyz;
	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HSConstant_Cube")]
[maxtessfactor(64.0f)]
TS_CUBE_HS_INPUT main(InputPatch<TS_CUBE_VS_OUTPUT, 4> input, uint i : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	TS_CUBE_HS_INPUT output = (TS_CUBE_HS_INPUT)0;
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
	output.fTessEdges[0]   = 64;
	output.fTessEdges[1]   = 64;
	output.fTessEdges[2]   = 64;
	output.fTessEdges[3]   = 64;
	output.fTessInsides[0] = 64;
	output.fTessInsides[1] = 64;

	return output;
}

[domain("quad")]
TS_CUBE_DS_PS_INPUT DS_TS(TS_CUBE_HSC_INPUT input, float2 uv : SV_DomainLocation, OutputPatch<TS_CUBE_HS_INPUT, 4> patch)
{
	TS_CUBE_DS_PS_INPUT output = (TS_CUBE_DS_PS_INPUT)0;

	float3 fTangentW = normalize(patch[1].positionW - patch[0].positionW);
	float3 fBinormalW = normalize(patch[2].positionW - patch[0].positionW);
	float3   fNormalW = cross(fTangentW, fBinormalW);

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

float4 PS_TS(TS_CUBE_DS_PS_INPUT input) : SV_Target
{
	float4 cIllumination = Lighting(input.positionW, input.normal);
	float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord);
	return cColor * cIllumination;
}

GBUFFER PS_TS_GBUFFER(TS_CUBE_DS_PS_INPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
    output.positionW = float4(input.positionW, 1.0f);
    output.normal = float4(input.normal, 1.0f);
    output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);

    output.ambient = gMaterial.m_cAmbient;
    output.specular = gMaterial.m_cSpecular;
    
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
