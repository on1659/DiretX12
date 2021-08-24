
#include "Light.fx"
#include "GBuffer.fx"

#define PARTICLE_PAUSE	    0
#define PARTICLE_PLAY		1

struct PARTICLE_INPUT
{
    float3 position : POSITION;
    float3 velocity : VELOCITY;
    float2 size : SIZE;
    uint type : TYPE;
    float age : AGE;
};

PARTICLE_INPUT VSStream(PARTICLE_INPUT input)
{
    return input;
}
