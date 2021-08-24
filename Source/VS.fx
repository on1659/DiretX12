
#include "Light.fx"
#include "GBuffer.fx"

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

struct PARTICLE_INPUT
{
    float3 position : POSITION;
    float3 velocity : VELOCITY;
    float2 size : SIZE;
    uint type : TYPE;
    float age : AGE;
};

struct PARTICLE_OUTPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    float4 color : COLOR;
    uint type : TYPE;
};

PARTICLE_OUTPUT VS(PARTICLE_INPUT input)
{
    PARTICLE_OUTPUT output;

    float t = input.age;
    output.position = (0.5f * gvAcceleration * t * t) + (input.velocity * t) + input.position;

    float fOpacity = 1.0f - smoothstep(0.0f, 1.0f, t);

    output.color = float4(1.0f, 1.0f, 1.0f, fOpacity);
    output.size = input.size;
    output.type = input.type;

    return output;
}
