
#include "GSShader.fx"

cbuffer cbAABB : register(b10)
{
    float3 g_Size;
};

struct GS_AABB_VS_INPUT
{
    float3 size : SIZE; //사이즈
    float4x4 mtxTransform : INSTANCEPOS;
};

struct GS_AABB_VS_OUTPUT
{
    float3 size : SIZE; //큐브크기
    float3 positionW : POSITION; //위치
    float2 texCoord : TEXCOORD0; //텍스쳐
    float4x4 mtxTransform : INSTANCEPOS;
};

struct GS_AABB_GS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
};
//------------------------------------------------------------------------------- GS_INSTANCE_CUBE

GS_AABB_VS_OUTPUT VS_GS_AABB_Cube(GS_AABB_VS_INPUT input)
{
    GS_AABB_VS_OUTPUT output = (GS_AABB_VS_OUTPUT) 0;
    output.positionW = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), input.mtxTransform).xyz;
    output.mtxTransform = input.mtxTransform;
    output.size = g_Size;
    return output;
}


[maxvertexcount(16)]
void GS_GS_OOBB(point GS_AABB_VS_OUTPUT input[1], uint nPrimID : SV_PrimitiveID, inout TriangleStream<GS_AABB_GS_OUTPUT> stream)
{

    GS_AABB_GS_OUTPUT v[16];

	//월드 좌표
    float3 position = input[0].positionW;
    float fx = input[0].size.x;
    float fy = input[0].size.y;
    float fz = input[0].size.z;
    float3 posRot;


    float index = 0;

	//--------------------------------------------------------------------
	//Front
	//0
    posRot = mul(float4(-fx, +fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//1
    posRot = mul(float4(-fx, -fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//2
    posRot = mul(float4(+fx, +fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//3
    posRot = mul(float4(+fx, -fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------


	//--------------------------------------------------------------------
	//Left
	//4
    posRot = mul(float4(+fx, +fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//5
    posRot = mul(float4(+fx, -fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//6
    posRot = mul(float4(+fx, +fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//7
    posRot = mul(float4(+fx, -fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------


	//--------------------------------------------------------------------
	//Back
	//8
    posRot = mul(float4(+fx, +fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//9
    posRot = mul(float4(+fx, -fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//10
    posRot = mul(float4(-fx, +fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//11
    posRot = mul(float4(-fx, -fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------


	//--------------------------------------------------------------------
	//Right
	//12
    posRot = mul(float4(-fx, +fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//13
    posRot = mul(float4(-fx, -fy, +fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//14
    posRot = mul(float4(-fx, +fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//15
    posRot = mul(float4(-fx, -fy, -fz, 0.f), input[0].mtxTransform).xyz;
    v[index].positionW = posRot + position;
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------

    int i = 0;
    for (int count = 0; count < 4; count++)
    {
        stream.Append(v[i + 0]);
        stream.Append(v[i + 2]);
        stream.Append(v[i + 1]);
        stream.Append(v[i + 3]);
        i += 4;
        stream.RestartStrip();
    }
}

[maxvertexcount(16)]
void GS_GS_AABB(point GS_AABB_VS_OUTPUT input[1], uint nPrimID : SV_PrimitiveID, inout TriangleStream<GS_AABB_GS_OUTPUT> stream)
{

    GS_AABB_GS_OUTPUT v[16];

	//월드 좌표
    float3 position = input[0].positionW;
    float fx = input[0].size.x;
    float fy = input[0].size.y;
    float fz = input[0].size.z;
    float3 posRot;


    float index = 0;

	//--------------------------------------------------------------------
									//Front
	//0
    v[index].positionW = float3(position.x - fx, position.y + fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//1
    v[index].positionW = float3(position.x - fx, position.y - fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//2
    v[index].positionW = float3(position.x + fx, position.y + fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//3
    v[index].positionW = float3(position.x + fx, position.y - fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------


	//--------------------------------------------------------------------
									//Left
	//4
    v[index].positionW = float3(position.x + fx, position.y + fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//5
    v[index].positionW = float3(position.x + fx, position.y - fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//6
    v[index].positionW = float3(position.x + fx, position.y + fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//7
    v[index].positionW = float3(position.x + fx, position.y - fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------


	//--------------------------------------------------------------------
									//Back
	//8
    v[index].positionW = float3(position.x + fx, position.y + fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//9
    v[index].positionW = float3(position.x + fx, position.y - fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//10
    v[index].positionW = float3(position.x - fx, position.y + fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;

	//11
    v[index].positionW = float3(position.x - fx, position.y - fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------
	

	//--------------------------------------------------------------------
										//Right
	//12
    v[index].positionW = float3(position.x - fx, position.y + fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//13
    v[index].positionW = float3(position.x - fx, position.y - fy, position.z + fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//14
    v[index].positionW = float3(position.x - fx, position.y + fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//15
    v[index].positionW = float3(position.x - fx, position.y - fy, position.z - fz);
    v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
    index++;
	//--------------------------------------------------------------------

    int i = 0;
    for (int count = 0; count < 4; count++)
    {
        stream.Append(v[i + 0]);
        stream.Append(v[i + 2]);
        stream.Append(v[i + 1]);
        stream.Append(v[i + 3]);
        i += 4;
        stream.RestartStrip();
    }
}

float4 PS_GS_AABB(GS_AABB_GS_OUTPUT input) : SV_Target
{
    float4 cColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
    return cColor;
}

GBUFFER PS_GS_AABB_GBUFFER(GS_AABB_GS_OUTPUT input) : SV_Target
{
    GBUFFER output = (GBUFFER) 0;
    output.positionW = float4(input.positionW, 1.0f);
    output.diffuse = float4(0.8f, 0.7f, 0.22f, 1.0f);
    return output;
}
/*
    fxc /E GS_GS_AABB /T gs_5_0 /Od /Zi /Fo CompiledGs.fxo GS_AABB_Test.fx
*/