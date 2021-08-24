
#include "GBuffer.fx"
#include "Light.fx"


#define PARTICLE_TYPE_EMITTER	0
#define PARTICLE_TYPE_FLARE		1

//-------------------------------------------------------VS Use-------------------------------//
cbuffer cbViewProjectionMatrix			: register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};
cbuffer cbWorldMatrix					: register(b2)
{
	matrix gmtxWorld : packoffset(c0);
};
cbuffer NoiseBuffer						: register(b8)
{
	float frameTime;
	float3 scrollSpeeds;
	float3 scales;
	float padding;
};
cbuffer cbParticleInfo					: register(b9)
{
    float3  gvParticleEmitPosition;
    float   gfGameTime;
    float3  gvParticleEmitDirection;
    float   gfTimeStep;
    float3  gvAcceleration;
    float   gOnoff;
   // float3  gfRandom;
};
//-------------------------------------------------------VS Use-------------------------------//


//-------------------------------------------------------PS Use-------------------------------//

cbuffer DistortionBuffer				: register(b8)
{
	float2 distortion1;
	float2 distortion2;
	float2 distortion3;
	float distortionScale;
	float distortionBias;
};

SamplerState gssPariticle				: register(s2);
SamplerState gssPariticleDetial			: register(s3);
SamplerState gParticleSamplerState		: register(s4);

Texture2D gtxtDefault					: register(t0); //원래 012로 써야하지만
Texture2D gtxtNoise						: register(t2); //왠진모르겠지만
Texture2D gtxtAlpha						: register(t1); //021로 써야 된다..

//-------------------------------------------------------PS Use-------------------------------//

//-------------------------------------------------------GS Use-------------------------------//
Texture1D      gtxtRandomTexture		: register(t4);
Texture2DArray gtxtParticleTextureArray	: register(t5);
//-------------------------------------------------------GS Use-------------------------------//


cbuffer cbFixed
{
    float2 gvQuadTexCoord[4] = { float2(0.0f, 1.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };
};

struct VS_PARTICLE_FIRE
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct PS_PARTICLE_FIRE
{
	float4 position   : SV_POSITION;
	float3 positionW  : POSITION;
	float2 texCoord   : TEXCOORD0;
	float2 texCoords1 : TEXCOORD1;
	float2 texCoords2 : TEXCOORD2;
	float2 texCoords3 : TEXCOORD3;
};

//--------------------------------------------------------------------------------------------//

struct PARTICLE_INPUT
{
	float3 position : POSITION;
	float3 velocity : VELOCITY;
	float2 size : SIZE;
	float age : AGE;
	uint type : TYPE;
};

struct PARTICLE_OUTPUT
{
	float3 position : POSITION;
	float2 size : SIZE;
	float4 color : COLOR;
	   uint type : TYPE;
};

struct GS_PARTICLE_OUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

struct VS_INS_OUTPUT
{
    float3 size : SIZE;
    float3 positionW : POSITION;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
	//float4 color : COLOR0;
};

//--------------------------------------------------------------------------------------------//

float3 RandUnitVec3(float offset)
{
	// 게임 시간 더하기 오프셋을 무작위 텍스처 추출을 위한
	// 텍스처 좌표로 사용한다.
    float u = (gfTimeStep + offset);

	// 벡터 성분들의 범위는 [-1,1]이다.
    float3 v = gtxtRandomTexture.SampleLevel(gParticleSamplerState, u, 0).xyz;

	// 단위 구로 투영.
    return normalize(v);
}

float3 RandVec3(float offset)
{
	// 게임 시간 더하기 오프셋을 무작위 텍스처 추출을 위한
	// 텍스처 좌표로 사용한다.
    float u = (gfTimeStep + offset);

	// 벡터 성분들의 범위는 [-1,1]이다.
    float3 v = gtxtRandomTexture.SampleLevel(gParticleSamplerState, u, 0).xyz;

    return v;
}
