
#include "FxDefine.fx"
#include "Light.fx"
#include "GBuffer.fx"

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_SKYBOX_CUBEMAP_INPUT
{
	float3	position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float4	position : SV_POSITION;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_DETAIL_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_DETAIL_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct VS_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_DETAIL_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_DETAIL_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_DIFFUSED_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR0;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
VS_DIFFUSED_OUTPUT VSDiffusedColor(VS_DIFFUSED_INPUT input)
{
    VS_DIFFUSED_OUTPUT output = (VS_DIFFUSED_OUTPUT) 0;
    output.position = mul(float4(input.position, 1.0f), mul(gmtxWorld, gmtxViewProjection));
    output.color = input.color;
    return (output);
}

GBUFFER PSDiffusedColor(VS_DIFFUSED_OUTPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = input.position.xyzw;
    output.diffuse = input.color;
    return output;
}


//Player
/*-------------------------------------------------------------------------------------------------------------------------------*/
VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLightingColor(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output = (VS_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;

	//KYT '16.01.29
	/*gmtx Change*/
	output.position = mul(float4(input.position, 1.0f), mul(gmtxWorld, gmtxViewProjection));
	//output.position = mul(float4(input.position, 1.0f), mul(mul(gmtxWorld, gmtxView), gmtxProjection));

	output.texCoord = input.texCoord;
	return(output);
}	

float4 PSTexturedLightingColor(VS_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	return (gtxtDefault.Sample(gssDefault, input.texCoord) * cIllumination);
}

GBUFFER PSTexturedLightingColor_GBUFFER(VS_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	GBUFFER output   = (GBUFFER)0;
	output.positionW = float4(input.positionW, 1.0f);
	output.diffuse   = gtxtDefault.Sample(gssDefault, input.texCoord);
	output.normal    = float4(normalize(input.normalW),1.0f);
	return output;
}

// 아직 노사용
/*-------------------------------------------------------------------------------------------------------------------------------*/
VS_DETAIL_TEXTURED_OUTPUT VSDetailTexturedColor(VS_DETAIL_TEXTURED_INPUT input)
{
	VS_DETAIL_TEXTURED_OUTPUT output = (VS_DETAIL_TEXTURED_OUTPUT)0;
	//output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = input.texCoordDetail;

	return(output);
}

float4 PSDetailTexturedColor(VS_DETAIL_TEXTURED_OUTPUT input) : SV_Target
{
	float4 cBaseTexColor = gtxtDefault.Sample(gssDefault, input.texCoordBase);
	float4 cDetailTexColor = gtxtDefaultDetail.Sample(gssDefaultDetail, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
	//    float4 cAlphaTexColor = gtxtTerrainAlphaTexture.Sample(gTerrainSamplerState, input.texcoord0);
	//    float4 cColor = cIllumination * lerp(cBaseTexColor, cDetailTexColor, cAlphaTexColor.r);
	return(cColor);
}

GBUFFER PSDetailTexturedColor_GBUFFER(VS_DETAIL_TEXTURED_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	float4 cBaseTexColor = gtxtDefault.Sample(gssDefault, input.texCoordBase);
	float4 cDetailTexColor = gtxtDefaultDetail.Sample(gssDefaultDetail, input.texCoordDetail);
	output.positionW = input.position;//position = float4
	output.diffuse = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
	return output;
}



//Player
/*-------------------------------------------------------------------------------------------------------------------------------*/
VS_DETAIL_TEXTURED_LIGHTING_OUTPUT VSDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_INPUT input)
{
	VS_DETAIL_TEXTURED_LIGHTING_OUTPUT output = (VS_DETAIL_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	//output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = input.texCoordDetail;

	return(output);
}

VS_DETAIL_TEXTURED_LIGHTING_OUTPUT VSWater(VS_DETAIL_TEXTURED_LIGHTING_INPUT input)
{
	VS_DETAIL_TEXTURED_LIGHTING_OUTPUT output = (VS_DETAIL_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;

	//KYT '16.01.29
	/*gmtx Change*/
	output.position = mul(float4(input.position, 1.0f), mul(gmtxWorld, gmtxViewProjection));
	//output.position = mul(float4(input.position, 1.0f), mul(mul(gmtxWorld, gmtxView), gmtxProjection));

	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = mul(float4(input.texCoordDetail, 0.0f, 1.0f), gmtxTexture).xy;

	return(output);
}

float4 PSWater(VS_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cBaseTexColor = gtxtDefault.Sample(gssDefault, input.texCoordBase);
	float4 cDetailTexColor = gtxtDefaultDetail.Sample(gssDefaultDetail, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
	return (cColor * cIllumination);
}

GBUFFER PSWater_GBUFFER(VS_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	float4 cBaseTexColor = gtxtDefault.Sample(gssDefault, input.texCoordBase);
	float4 cDetailTexColor = gtxtDefaultDetail.Sample(gssDefaultDetail, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
	//cColor = Fog(cColor, input.positionW);

	GBUFFER output = (GBUFFER)0;
	output.normal = float4(normalize(input.normalW), 0.0f);
	output.positionW = float4(input.positionW, 1.0f);
	output.diffuse = cColor;
	
	return output;
}
//Terrain
/*-------------------------------------------------------------------------------------------------------------------------------*/
VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT VSTerrainDetailTexturedLightingColor(VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_INPUT input)
{
	VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT output = (VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	//output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = input.texCoordDetail;

	return(output);
}

#ifdef _WITH_TERRAIN_TEXTURE_ARRAY
float4 PSTerrainDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cBaseTexColor = gtxtTerrain.Sample(gssTerrain, input.texCoordBase);
	float4 cDetailTexColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//	cDetailTexColor = gtxtTerrainDetails[gvTerrainTextureIndex.a].Sample(gssTerrainDetail, input.texCoordDetail); //Error
	///*
	switch (gvTerrainTextureIndex.a)
	{
	case 0:
		cDetailTexColor = gtxtTerrainDetails[0].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 1:
		cDetailTexColor = gtxtTerrainDetails[1].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 2:
		cDetailTexColor = gtxtTerrainDetails[2].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 3:
		cDetailTexColor = gtxtTerrainDetails[3].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 4:
		cDetailTexColor = gtxtTerrainDetails[4].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 5:
		cDetailTexColor = gtxtTerrainDetails[5].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 6:
		cDetailTexColor = gtxtTerrainDetails[6].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 7:
		cDetailTexColor = gtxtTerrainDetails[7].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 8:
		cDetailTexColor = gtxtTerrainDetails[8].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 9:
		cDetailTexColor = gtxtTerrainDetails[9].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	}
	//*/
	float4 cColor = saturate((cIllumination * cBaseTexColor * 0.7f) + (cDetailTexColor * 0.3f));
	return(cColor);
}
#else
float4 PSTerrainDetailTexturedLightingColor(VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cBaseTexColor = gtxtTerrain.Sample(gssTerrain, input.texCoordBase);
	float4 cDetailTexColor = gtxtTerrainDetail.Sample(gssTerrainDetail, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f)) * cIllumination;
	cColor = Fog(cColor, input.positionW);
	return(cColor);
}

GBUFFER PSTerrainDetailTexturedLightingColor_GBUFFER(VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	float4 cBaseTexColor = gtxtTerrain.Sample(gssTerrain, input.texCoordBase);
	float4 cDetailTexColor = gtxtTerrainDetail.Sample(gssTerrainDetail, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
	cColor = Fog(cColor, input.positionW);
	output.normal = float4(normalize(input.normalW), 0.0f);
	output.positionW = float4(input.positionW, 1.0f);
	output.diffuse = cColor;
	return(output);
}
#endif


VS_SKYBOX_CUBEMAP_OUTPUT VSSkyBoxTexturedColor(VS_SKYBOX_CUBEMAP_INPUT input)
{
	VS_SKYBOX_CUBEMAP_OUTPUT output = (VS_SKYBOX_CUBEMAP_OUTPUT)0;
	//output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProjection);
	output.positionL = input.position;

	return(output);
}

float4 PSSkyBoxTexturedColor(VS_SKYBOX_CUBEMAP_OUTPUT input) : SV_Target
{
	float4 cColor = gtxtSkyBox.Sample(gssSkyBox, input.positionL);
	cColor = FogSpace(cColor, 0.5f);
	return(cColor);
}

GBUFFER PSSkyBoxTexturedColor_GBUFFER(VS_SKYBOX_CUBEMAP_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	output.diffuse = gtxtSkyBox.Sample(gssSkyBox, input.positionL);
	output.normal = float4(0, 0, 0, 3);	
	return output;
}
/*-------------------------------------------------------------------------------------------------------------------------------*/


/*
fxc /E PSDiffusedColor /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo Effect.fx
fxc /E VSDiffusedColor /T vs_5_0 /Od /Zi /Fo CompiledGs.fxo Effect.fx
*/

