
#include "Light.fx"
#include "GBuffer.fx"

#define PARTICLE_PAUSE	    0
#define PARTICLE_PLAY		1

///////////////////////////////////////////////////////////////////////////
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};

cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};

cbuffer cbParticleInfos : register(b4)// 파티클 정보 상수 버퍼.
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

///////////////////////////////////////////////////////////////////////////
Texture1D gtxtRandomTexture : register(t4);
Texture2D gtxtParticleTextureArray : register(t5);
SamplerState gParticleSamplerState : register(s5);
///////////////////////////////////////////////////////////////////////////

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

struct GS_PARTICLE_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};


PARTICLE_INPUT VSParticleStreamOut(PARTICLE_INPUT input)
{
	return input;
}

PARTICLE_OUTPUT VSParticleDraw(PARTICLE_INPUT input)
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

[maxvertexcount(2)]
void GSParticleStreamOut(point PARTICLE_INPUT input[1], inout PointStream<PARTICLE_INPUT> pointStream)
{
	input[0].age += gfTimeStep;

	if (input[0].type == PARTICLE_PAUSE)
	{
		if (input[0].age > 0.000f)
		{
			PARTICLE_INPUT particle = (PARTICLE_INPUT)0;
			particle.position = gvParticleEmitPosition.xyz;

            particle.velocity.x = 0.0f; //gSpeed * vRandom; //속도
            particle.velocity.y = -gSpeed; //* vRandom; //속도
            particle.velocity.z = 0.0f; //gSpeed * vRandom; //속도
  
            particle.size = float2(gSize, gSize);
            particle.age = 0.0f;
			particle.type = PARTICLE_PLAY;
			pointStream.Append(particle);

			input[0].age = 0.0f;
			input[0].type = PARTICLE_PAUSE;	// 필요한가...
		}
		pointStream.Append(input[0]);
	}
	else
		if (input[0].age <= 2.0f)
		{
			pointStream.Append(input[0]);
		}
}

//24개: 정육면체
[maxvertexcount(24)]
void GSParticleDraw(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> triStream)
{
	if (input[0].type == PARTICLE_PAUSE) return;
	//float3 vLook = normalize(gvCameraPosition_Particle.xyz - input[0].position);
	float3 vLook = float3(0.0f, 0.0f, 1.0f);
	float3 vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook));
	float3 vUp = cross(vLook, vRight);

	float fHalfSize = 0.5f * input[0].size.x;

	float4 vQuad[24];

	float2 TexCoord[24];

	//앞면
	vQuad[0] = float4(input[0].position + (fHalfSize * vRight) - (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[1] = float4(input[0].position + (fHalfSize * vRight) + (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[2] = float4(input[0].position - (fHalfSize * vRight) - (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[3] = float4(input[0].position - (fHalfSize * vRight) + (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);

	//뒷면
	vQuad[4] = float4(input[0].position - (fHalfSize * vRight) - (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[5] = float4(input[0].position - (fHalfSize * vRight) + (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[6] = float4(input[0].position + (fHalfSize * vRight) - (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[7] = float4(input[0].position + (fHalfSize * vRight) + (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);

	//우측면
	vQuad[8] = float4(input[0].position + (fHalfSize * vRight) - (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[9] = float4(input[0].position + (fHalfSize * vRight) + (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[10] = float4(input[0].position + (fHalfSize * vRight) - (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[11] = float4(input[0].position + (fHalfSize * vRight) + (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);

	//좌측면
	vQuad[12] = float4(input[0].position - (fHalfSize * vRight) - (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[13] = float4(input[0].position - (fHalfSize * vRight) + (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[14] = float4(input[0].position - (fHalfSize * vRight) - (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[15] = float4(input[0].position - (fHalfSize * vRight) + (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);

	//윗면
	vQuad[16] = float4(input[0].position + (fHalfSize * vRight) + (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[17] = float4(input[0].position + (fHalfSize * vRight) + (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[18] = float4(input[0].position - (fHalfSize * vRight) + (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[19] = float4(input[0].position - (fHalfSize * vRight) + (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);

	//아랫면
	vQuad[20] = float4(input[0].position + (fHalfSize * vRight) - (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[21] = float4(input[0].position + (fHalfSize * vRight) - (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);
	vQuad[22] = float4(input[0].position - (fHalfSize * vRight) - (fHalfSize * vUp) + (fHalfSize * vLook), 1.0f);
	vQuad[23] = float4(input[0].position - (fHalfSize * vRight) - (fHalfSize * vUp) - (fHalfSize * vLook), 1.0f);

	float u1, u2, v1, v2;

	if (guiTextureType == 0)
	{
		u1 = 0.0f, u2 = 0.5f, v1 = 0.0f, v2 = 0.5f;
	}
	else if (guiTextureType == 1)
	{
		u1 = 0.5f, u2 = 1.0f, v1 = 0.5f, v2 = 1.0f;
	}
	else if (guiTextureType == 2)
	{
		u1 = 0.5f, u2 = 1.0f, v1 = 0.0f, v2 = 0.5f;
	}
	else if (guiTextureType == 3)
	{
		u1 = 0.0f, u2 = 0.5f, v1 = 0.5f, v2 = 1.0f;
	}

	TexCoord[0] = float2(u1, v1);
	TexCoord[1] = float2(u2, v1);
	TexCoord[2] = float2(u1, v2);
	TexCoord[3] = float2(u2, v2);

	TexCoord[4] = float2(u1, v1);
	TexCoord[5] = float2(u2, v1);
	TexCoord[6] = float2(u1, v2);
	TexCoord[7] = float2(u2, v2);

	TexCoord[8] = float2(u1, v1);
	TexCoord[9] = float2(u2, v1);
	TexCoord[10] = float2(u1, v2);
	TexCoord[11] = float2(u2, v2);

	TexCoord[12] = float2(u1, v1);
	TexCoord[13] = float2(u2, v1);
	TexCoord[14] = float2(u1, v2);
	TexCoord[15] = float2(u2, v2);

	TexCoord[16] = float2(u1, v1);
	TexCoord[17] = float2(u2, v1);
	TexCoord[18] = float2(u1, v2);
	TexCoord[19] = float2(u2, v2);

	TexCoord[20] = float2(u1, v1);
	TexCoord[21] = float2(u2, v1);
	TexCoord[22] = float2(u1, v2);
	TexCoord[23] = float2(u2, v2);

	
	//TexCoord[0] = float2(0.0f, 0.0f);
	//TexCoord[1] = float2(1.0f, 0.0f);
	//TexCoord[2] = float2(0.0f, 1.0f);
	//TexCoord[3] = float2(1.0f, 1.0f);
	//
	//TexCoord[4] = float2(0.0f, 0.0f);
	//TexCoord[5] = float2(1.0f, 0.0f);
	//TexCoord[6] = float2(0.0f, 1.0f);
	//TexCoord[7] = float2(1.0f, 1.0f);
	//
	//TexCoord[8] = float2(0.0f, 0.0f);
	//TexCoord[9] = float2(1.0f, 0.0f);
	//TexCoord[10] = float2(0.0f, 1.0f);
	//TexCoord[11] = float2(1.0f, 1.0f);
	//
	//TexCoord[12] = float2(0.0f, 0.0f);
	//TexCoord[13] = float2(1.0f, 0.0f);
	//TexCoord[14] = float2(0.0f, 1.0f);
	//TexCoord[15] = float2(1.0f, 1.0f);
	//
	//TexCoord[16] = float2(0.0f, 0.0f);
	//TexCoord[17] = float2(1.0f, 0.0f);
	//TexCoord[18] = float2(0.0f, 1.0f);
	//TexCoord[19] = float2(1.0f, 1.0f);
	//
	//TexCoord[20] = float2(0.0f, 0.0f);
	//TexCoord[21] = float2(1.0f, 0.0f);
	//TexCoord[22] = float2(0.0f, 1.0f);
	//TexCoord[23] = float2(1.0f, 1.0f);
	

	GS_PARTICLE_OUT output = (GS_PARTICLE_OUT)0;

	for (int i = 0; i < 24; i++)
	{
		output.position = mul(vQuad[i], gmtxViewProjection);
		output.texCoord = TexCoord[i];
		output.color = input[0].color;
		triStream.Append(output);

		if ((i + 1) % 4 == 0 && (i + 1) != 24) triStream.RestartStrip();
	}
}

float4 PSParticleDraw(GS_PARTICLE_OUT input) : SV_Target
{
	float4 cColor = gtxtParticleTextureArray.Sample(gParticleSamplerState, input.texCoord);

	return (cColor * input.color);
}

GBUFFER PSParticleDraw_GBUFFER(GS_PARTICLE_OUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	output.positionW = input.position;
	output.normal = float4(0.0f, 0.0f, 0.0f, 10.0f);
	output.diffuse = gtxtParticleTextureArray.Sample(gParticleSamplerState, input.texCoord);
	output.diffuse.z = 1.0f;
	return output;
}
