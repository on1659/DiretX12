
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
    float gDuringTime;
    float gEmitCount;
};

Texture1D gtxtRandomTexture : register(t4);
SamplerState gParticleSamplerState : register(s5);

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

[maxvertexcount(2)]
void GSStream(point PARTICLE_INPUT input[1], inout PointStream<PARTICLE_INPUT> pointStream)
{
    input[0].age += gfTimeStep;

    if (input[0].type == PARTICLE_PAUSE)
    {
        if (input[0].age > 0.200f)
        {
            PARTICLE_INPUT particle = (PARTICLE_INPUT) 0;
            particle.position = gvParticleEmitPosition.xyz;
            particle.velocity.x = 0.0f; //gSpeed * vRandom; //속도
            particle.velocity.y = -gSpeed; //* vRandom; //속도
            particle.velocity.z = 0.0f; //gSpeed * vRandom; //속도
            particle.size = float2(gSize, gSize);
            particle.age = 0.0f;
            particle.type = PARTICLE_PLAY;
            pointStream.Append(particle);

            input[0].age = 0.0f;
            input[0].type = PARTICLE_PAUSE; // 필요한가...
        }
        pointStream.Append(input[0]);
    }
    else if (input[0].age <= gDuringTime)
    {
        pointStream.Append(input[0]);
    }
}
