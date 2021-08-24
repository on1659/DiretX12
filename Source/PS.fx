
#include "Light.fx"
#include "GBuffer.fx"
#include "NormalCalc.hlsli"

#define PARTICLE_PAUSE	    0
#define PARTICLE_PLAY		1

cbuffer cbParticleInfos : register(b4) // 파티클 정보 상수 버퍼.
{
    float3 gvParticleEmitPosition;
    float gfGameTime;
    float3 gvParticleEmitDirection;
    float gfTimeStep;
    float3 gvAcceleration;
    uint guiTextureType;
    float gSpeed;
    float gSize;
    float2 padding;
};

Texture2D gtxtParticleTextureArray : register(t5);
SamplerState gParticleSamplerState : register(s5);

struct GS_PARTICLE_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

GBUFFER PS(GS_PARTICLE_OUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = input.position;
	output.normal.w = 4.0f;
    output.diffuse = gtxtParticleTextureArray.Sample(gParticleSamplerState, input.texCoord);
    output.ambient = gMaterial.m_cAmbient;
    output.specular = gMaterial.m_cSpecular;
    return output;
}

/*
fxc /E PSParticleDraw /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo SmashParticle.fx
*/