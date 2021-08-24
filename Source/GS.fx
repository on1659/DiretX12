
#include "GBuffer.fx"
#include "Light.fx"

#define PARTICLE_PAUSE	    0
#define PARTICLE_PLAY		1

cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxViewProjection : packoffset(c0);
};

cbuffer cbCamera : register(b5)
{
    float4 gCameraPosition;
}

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


struct PARTICLE_OUTPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    float4 color : COLOR;
    uint type : TYPE;
};

struct GS_PARTICLE_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

//4개: 평면
[maxvertexcount(4)]
void GS(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> triStream)
{
    float3 vLook = normalize(float3(123, 1, 111) - input[0].position);
    float3 vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook));
    float3 vUp = cross(vLook, vRight);

    float fHalfWidth = 0.5f * gSize;
    float fHalfHeight = 0.5f * gSize;

    float4 vQuad[4];
    vQuad[0] = float4(input[0].position + fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[1] = float4(input[0].position + fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
    vQuad[2] = float4(input[0].position - fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[3] = float4(input[0].position - fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);

    GS_PARTICLE_OUT output = (GS_PARTICLE_OUT) 0;

    float2 TexCoord[4] =
    {
        float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f)
    };

    for (int i = 0; i < 4; i++)
    {
        output.position = mul(vQuad[i], gmtxViewProjection);
        output.texCoord = TexCoord[i];
        output.color = input[0].color;
        triStream.Append(output);
    }
}

/*
fxc /Od /Zi /Ges /T gs_5_0 /E GS /Fo GS_QuadDraw.fxo GS.fx
*/
