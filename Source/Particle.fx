
#include "ParticleDefine.fx"

PARTICLE_INPUT VSParticleStreamOut(PARTICLE_INPUT input)
{
    return (input);
}

[maxvertexcount(10)]
void GSParticleStreamOut(point PARTICLE_INPUT input[1], inout PointStream<PARTICLE_INPUT> pointStream)
{
    input[0].age += gfTimeStep;

    float fSpeed = 50.0f;
	//초기 상태면
    if (input[0].type == PARTICLE_TYPE_EMITTER)
    {
        //새 입자 방출 시간이 되었니?
        if (input[0].age > 0.01f)
        {
            float3 vDir = { 1, 0, 0 };
            for (int i = 0; i < 10; i+=1)
            {
                vDir.x = sin(i);
                vDir.y = cos(i);
                PARTICLE_INPUT particle;
                float3 dir = vDir;

                vDir = normalize(vDir);
                particle.position = gvParticleEmitPosition.xyz;
                particle.velocity = fSpeed * vDir;
                particle.size = float2(5.0f, 5.0f);
                particle.age = 0.0f;

               if(gOnoff == 1.0f) 
                    particle.type = PARTICLE_TYPE_FLARE;
                else
                    particle.type = PARTICLE_TYPE_EMITTER;
             
                   pointStream.Append(particle);

                vDir.x = sin(i + 1);
                vDir.y = cos(i + 1);
                particle.velocity = fSpeed * vDir;
                pointStream.Append(particle);

            }
        }
		//한개더 출력
        pointStream.Append(input[0]);

    }
    else if (input[0].type == PARTICLE_TYPE_FLARE)
    {
        if (input[0].age <= 1.5)
        {
            pointStream.Append(input[0]);
        }
    }
}

PARTICLE_OUTPUT VSParticleDraw(PARTICLE_INPUT input)
{
    PARTICLE_OUTPUT output;
    float t = input.age;
    output.position = (0.5f * gvAcceleration * t * t) + (input.velocity * t) + input.position;

    float fOpacity = 1.0f;
    //-smoothstep(0.0f, 1.0f, t);
    output.color = float4(1.0f, 1.0f, 1.0f, fOpacity);

    output.size.x = input.age;
    output.size.y = input.size.x;
    output.type = input.type;


    return (output);
}

[maxvertexcount(4)]
void GSParticleDraw(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> triStream)
{
    if (input[0].type == PARTICLE_TYPE_EMITTER)
        return;

    float3 vLook = float3(0, 0, 1);
    float3 vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook));
    float3 vUp = cross(vRight, vLook);
    float fHalfWidth = 0.5f * input[0].size.y, fHalfHeight = 0.5f * input[0].size.y;


	//float3 vLook = normalize(input[0].position - cameraPos.xyz);
	//	float3 vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook));
	//	float3 vUp = cross(vLook, vRight);
	//	float fHalfWidth = 0.5f * input[0].size.x, fHalfHeight = 0.5f * input[0].size.y;
    float4 vQuad[4];
    vQuad[0] = float4(input[0].position + fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[1] = float4(input[0].position + fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
    vQuad[2] = float4(input[0].position - fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[3] = float4(input[0].position - fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);

	//matrix mtxViewProjection = mul(gmtxView, gmtxProjection);
    GS_PARTICLE_OUT output;
    for (int i = 0; i < 4; i++)
    {
        output.position = mul(vQuad[i], gmtxViewProjection);
        output.texCoord = gvQuadTexCoord[i];
        output.color   = input[0].color;
        triStream.Append(output);
    }
}

[maxvertexcount(24)]
void GSCubeInstancing(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> stream)
{
    if (input[0].type == PARTICLE_TYPE_EMITTER) return;

    GS_PARTICLE_OUT v[24];

	//월드 좌표
    float3 position = input[0].position;


    float fHalfW = input[0].size.x * 0.5f;
    float fHalfH = input[0].size.x * 0.5f;
    float fHalfZ = input[0].size.x * 0.5f;


	///////////////////////////////11
    v[0].position = mul(float4(float3(position.x - fHalfW, position.y - fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[0].texCoord = float2(0, 0);

    v[1].position = mul(float4(float3(position.x - fHalfW, position.y + fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[1].texCoord = float2(0, 1);

    v[2].position = mul(float4(float3(position.x + fHalfW, position.y - fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[2].texCoord = float2(1, 0);

    v[3].position = mul(float4(float3(position.x + fHalfW, position.y + fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[3].texCoord = float2(1, 1);
    
	///////////////////////////////22
    v[4].position = mul(float4(float3(position.x + fHalfW, position.y - fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[4].texCoord = float2(0, 0);

    v[5].position = mul(float4(float3(position.x + fHalfW, position.y + fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[5].texCoord = float2(0, 1);

    v[6].position = mul(float4(float3(position.x + fHalfW, position.y - fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[6].texCoord = float2(1, 0);

    v[7].position = mul(float4(float3(position.x + fHalfW, position.y + fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[7].texCoord = float2(1, 1);

	///////////////////////////////33
    v[8].position = mul(float4(float3(position.x + fHalfW, position.y - fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[8].texCoord = float2(0, 0);

    v[9].position = mul(float4(float3(position.x + fHalfW, position.y + fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[9].texCoord = float2(0, 1);

    v[10].position = mul(float4(float3(position.x - fHalfW, position.y - fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[10].texCoord = float2(1, 0);

    v[11].position = mul(float4(float3(position.x - fHalfW, position.y + fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[11].texCoord = float2(1, 1);
    

	///////////////////////////////44
    v[12].position = mul(float4(float3(position.x - fHalfW, position.y - fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[12].texCoord = float2(0, 0);

    v[13].position = mul(float4(float3(position.x - fHalfW, position.y + fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[13].texCoord = float2(0, 1);

    v[14].position = mul(float4(float3(position.x - fHalfW, position.y - fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[14].texCoord = float2(1, 0);

    v[15].position = mul(float4(float3(position.x - fHalfW, position.y + fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[15].texCoord = float2(1, 1);


	///////////////////////////////55
    v[16].position = mul(float4(float3(position.x - fHalfW, position.y + fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[16].texCoord = float2(0, 0);

    v[17].position = mul(float4(float3(position.x - fHalfW, position.y + fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[17].texCoord = float2(0, 1);

    v[18].position = mul(float4(float3(position.x + fHalfW, position.y + fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[18].texCoord = float2(1, 0);

    v[19].position = mul(float4(float3(position.x + fHalfW, position.y + fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[19].texCoord = float2(1, 1);

	///////////////////////////////66
    v[20].position = mul(float4(float3(position.x - fHalfW, position.y - fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[20].texCoord = float2(0, 0);

    v[21].position = mul(float4(float3(position.x - fHalfW, position.y - fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[21].texCoord = float2(0, 1);

    v[22].position = mul(float4(float3(position.x + fHalfW, position.y - fHalfH, position.z + fHalfZ), 1.0f), gmtxViewProjection);
    v[22].texCoord = float2(1, 0);

    v[23].position = mul(float4(float3(position.x + fHalfW, position.y - fHalfH, position.z - fHalfZ), 1.0f), gmtxViewProjection);
    v[23].texCoord = float2(1, 1);

	//11
    stream.Append(v[0]);
    stream.Append(v[1]);
    stream.Append(v[2]);
    stream.Append(v[3]);

	//22
    stream.RestartStrip();
    stream.Append(v[4]);
    stream.Append(v[5]);
    stream.Append(v[6]);
    stream.Append(v[7]);

	//33
    stream.RestartStrip();
    stream.Append(v[8]);
    stream.Append(v[9]);
    stream.Append(v[10]);
    stream.Append(v[11]);

	//44
    stream.RestartStrip();
    stream.Append(v[12]);
    stream.Append(v[13]);
    stream.Append(v[14]);
    stream.Append(v[15]);

	//위
	//stream.RestartStrip();
    stream.Append(v[16]);
    stream.Append(v[17]);
    stream.Append(v[18]);
    stream.Append(v[19]);

	//아래
    stream.RestartStrip();
    stream.Append(v[20]);
    stream.Append(v[21]);
    stream.Append(v[22]);
    stream.Append(v[23]);
}

float4 PSParticleDraw(GS_PARTICLE_OUT input) : SV_TARGET
{
    float4 cColor = gtxtParticleTextureArray.Sample(gParticleSamplerState, float3(input.texCoord, 0));
    return (cColor * input.color);
}


/*
fxc /E PSParticleDraw /T ps_5_0 /Od /Zi /Fo CompiledPs.fxo Particle.fx    
*/