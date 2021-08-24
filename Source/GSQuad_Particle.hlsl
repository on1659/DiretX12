
#include "ParticleDefine.fx"

#define PARTICLE_PAUSE	    0
#define PARTICLE_PLAY		1

cbuffer cbCamera : register(b5)
{
    float4 gCameraPosition;
}
cbuffer cbAnimationSpriteBuffer : register(b6)
{
    float gSpriteAnimation_FPS;
    float gSpriteAnimation_Count;
    float gSpriteAnimation_Width;
    float gSpriteAnimation_Left;
}
//4°³: Æò¸é
[maxvertexcount(4)]
void GS_QuadDraw(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> triStream)
{
    float3 vLook = normalize(gCameraPosition.xyz - input[0].position);
    float3 vRight = normalize(cross(float3(0.0f, 1.0f, 1.0f), vLook));
    float3 vUp = cross(vRight, vLook);
    float fHalfWidth = 0.5f * input[0].size.x, fHalfHeight = 0.5f * input[0].size.y;

    float4 vQuad[4];
    vQuad[0] = float4(input[0].position + fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[1] = float4(input[0].position + fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
    vQuad[2] = float4(input[0].position - fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[3] = float4(input[0].position - fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
 


    float2 TexCoord[4] =
    {
        float2(0.0f, 0.0f), 
        float2(1.0f, 0.0f), 
        float2(0.0f, 1.0f), 
        float2(1.0f, 1.0f)
    };



    GS_PARTICLE_OUT output;
    for (int i = 0; i < 4; i++)
    {

        float2 tex = TexCoord[i];

        if (tex.x == 0)
            tex.x = gSpriteAnimation_Left;
        else
            tex.x = gSpriteAnimation_Left + gSpriteAnimation_Width;


        output.position = mul(vQuad[i], gmtxViewProjection);
        output.texCoord = tex;
        output.color = input[0].color;
        triStream.Append(output);
    }
}


/*
fxc /Od /Zi /Ges /T gs_5_0 /E GS_QuadDraw /Fo GSQuad_Particle.fxo GSQuad_Particle.fx
*/
