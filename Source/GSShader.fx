

#include "Light.fx"
#include "GBuffer.fx"

cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};

Texture2D gtxtDefault : register(t0);

SamplerState gssDefault : register(s0);

//---------------------------------------------- GeoMesh
struct GS_CUBE_VS_INPUT
{
	float3 size : SIZE;
	float2 texCoord : TEXCOORD0;
};

struct GS_CUBE_VS_OUTPUT
{
	float3 size : SIZE;
	float2 texCoord : TEXCOORD0;
};

struct GS_CUBE_GS_OUTPUT{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

//-------------------------------------GeoInstanceSphere----------------------------------------

struct VS_INSTANCED_SPHERE_IN
{
	float3 size : SIZE;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_SPHERE_OUT
{
	float3 size : SIZE;
	float4x4 mtxTransform: WORLDMTX;
};

struct GS_INSTANCED_SPHERE_OUT {
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

//--------------------------------------------------------------------------------------------

float3 GetNormal(float3 vPosition1, float3 vPosition2, float3 vPosition3){
	float3 Edge1 = float3(vPosition2.x - vPosition1.x, vPosition2.y - vPosition1.y, vPosition2.z - vPosition1.z);
		float3 Edge2 = float3(vPosition3.x - vPosition1.x, vPosition3.y - vPosition1.y, vPosition3.z - vPosition1.z);
		float3 vNormal = cross(Edge1, Edge2);
		float3 outvec = float3(vNormal.x, vNormal.y, vNormal.z);
		return outvec;

}

//--------------------------------------------------------------------------------------------
/*
fxc /E GS_GS_Instance_Sphere /T vs_5_0 /Od /Zi /Fo CompiledGs.fxo GS_Instance_Sphere.fx
*/