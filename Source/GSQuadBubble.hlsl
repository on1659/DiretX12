
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

cbuffer cbFixed
{
    float2 gvQuadTexCoord[4] = { float2(0.0f, 1.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };
};

//4°³: Æò¸é
[maxvertexcount(4)]
void GS_QuadDraw(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> triStream)
{
    float3 vLook = normalize(gCameraPosition.xyz - input[0].position);
    float3 vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook));
    float3 vUp = cross(vRight, vLook);
    float fHalfWidth = 0.5f * input[0].size.x, fHalfHeight = 0.5f * input[0].size.y;


    float4 vQuad[4];
    vQuad[0] = float4(input[0].position + fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[1] = float4(input[0].position + fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
    vQuad[2] = float4(input[0].position - fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[3] = float4(input[0].position - fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);

	
    GS_PARTICLE_OUT output;
    for (int i = 0; i < 4; i++)
    {
        output.position = mul(vQuad[i], gmtxViewProjection);
        output.texCoord = gvQuadTexCoord[i];
        output.color = input[0].color;
        triStream.Append(output);
    }
}

