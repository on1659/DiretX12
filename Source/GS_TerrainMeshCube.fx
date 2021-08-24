
#include "GSShader.fx"

//-------------------------------------GeoInstanceMesh----------------------------------------
struct GS_INSTANCED_CUBE_VS_INPUT
{
	float3 size : SIZE;					//사이즈
	float4x4 mtxTransform : INSTANCEPOS;//인스턴싱을 위한행렬
};

struct GS_INSTANCED_CUBE_VS_OUTPUT
{
	float3 size : SIZE;					//큐브크기
	float3 positionW : POSITION;		//큐브 사이즈
	float2 texCoord : TEXCOORD0;		//텍스쳐
	float4x4 mtxTransform : INSTANCEPOS;//인스턴싱을 위한행렬
};

struct GS_INSTANCED_CUBE_GS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;	//굳이 안넘겨도 되는 데이터
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};
//-------------------------------------GeoInstanceMesh----------------------------------------


//------------------------------------------------------------------------------- GS_INSTANCE_CUBE
GS_INSTANCED_CUBE_VS_OUTPUT VS_GS_INSTANCE_Cube(GS_INSTANCED_CUBE_VS_INPUT input)
{
	GS_INSTANCED_CUBE_VS_OUTPUT output = (GS_INSTANCED_CUBE_VS_OUTPUT)0;
	output.size = input.size;
	output.positionW = float3(input.mtxTransform._41, input.mtxTransform._42, input.mtxTransform._43);
	//output.positionW = float3(input.mtxTransform._41, input.mtxTransform._42, input.mtxTransform._43);// mul(float4(0.0f, 0.0f, 0.0f, 1.0f), input.mtxTransform).xyz;
	return output;
}


[maxvertexcount(20)]
void GS_GS_INSTANCE_Cube_Texutre(point GS_INSTANCED_CUBE_VS_OUTPUT input[1], uint nPrimID: SV_PrimitiveID, inout TriangleStream<GS_INSTANCED_CUBE_GS_OUTPUT> stream){

	GS_INSTANCED_CUBE_GS_OUTPUT v[24];

	//월드 좌표//v[index].position
	float3 position = input[0].positionW;
	float fx = input[0].size.x * 0.5f;
	float fy = input[0].size.y * 0.5f;
	float fz = input[0].size.z * 0.5f;

	float index = 0;
	float3 fNormalVector;// = float3(0.0f, 0.0f, 0.0f);
	//matrix gmtxViewProjection = mul(gmtxView, gmtxProjection);

	//0
	v[index].positionW = float3(position.x - fx, position.y + fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 0.0);
	index++;
	//1
	v[index].positionW = float3(position.x - fx, position.y - fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 1.0);
	index++;
	//2
	v[index].positionW = float3(position.x + fx, position.y + fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 0.0);
	index++;
	//3
	v[index].positionW = float3(position.x + fx, position.y - fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 1.0);
	index++;
	fNormalVector = GetNormal(v[index - 4].positionW, v[index - 3].positionW, v[index - 2].positionW);
	//fNormalVector = normalize(fNormalVector);
	v[index - 4].normal = fNormalVector;
	v[index - 3].normal = fNormalVector;
	v[index - 2].normal = fNormalVector;
	v[index - 1].normal = fNormalVector;


	//4
	v[index].positionW = float3(position.x + fx, position.y + fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 0.0);
	index++;
	//5
	v[index].positionW = float3(position.x + fx, position.y - fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 1.0);
	index++;
	//6
	v[index].positionW = float3(position.x + fx, position.y + fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 0.0);
	index++;
	//7
	v[index].positionW = float3(position.x + fx, position.y - fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 1.0);
	index++;
	fNormalVector = GetNormal(v[index - 4].positionW, v[index - 3].positionW, v[index - 2].positionW);
	//fNormalVector = normalize(fNormalVector);
	v[index - 4].normal = fNormalVector;
	v[index - 3].normal = fNormalVector;
	v[index - 2].normal = fNormalVector;
	v[index - 1].normal = fNormalVector;



	//8
	v[index].positionW = float3(position.x + fx, position.y + fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 0.0);
	index++;
	//9
	v[index].positionW = float3(position.x + fx, position.y - fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 1.0);
	index++;
	//10
	v[index].positionW = float3(position.x - fx, position.y + fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 0.0);
	index++;
	//11
	v[index].positionW = float3(position.x - fx, position.y - fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 1.0);
	index++;
	fNormalVector = GetNormal(v[index - 4].positionW, v[index - 3].positionW, v[index - 2].positionW);
	//fNormalVector = normalize(fNormalVector);
	v[index - 4].normal = fNormalVector;
	v[index - 3].normal = fNormalVector;
	v[index - 2].normal = fNormalVector;
	v[index - 1].normal = fNormalVector;



	//12
	v[index].positionW = float3(position.x - fx, position.y + fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 0.0);
	index++;
	//13
	v[index].positionW = float3(position.x - fx, position.y - fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 1.0);
	index++;
	//14
	v[index].positionW = float3(position.x - fx, position.y + fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 0.0);
	index++;
	//15
	v[index].positionW = float3(position.x - fx, position.y - fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 1.0);
	index++;
	fNormalVector = GetNormal(v[index - 4].positionW, v[index - 3].positionW, v[index - 2].positionW);
	//fNormalVector = normalize(fNormalVector);
	v[index - 4].normal = fNormalVector;
	v[index - 3].normal = fNormalVector;
	v[index - 2].normal = fNormalVector;
	v[index - 1].normal = fNormalVector;



	//16
	v[index].positionW = float3(position.x - fx, position.y + fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 0.0);
	index++;
	//17
	v[index].positionW = float3(position.x - fx, position.y + fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(0.0, 1.0);
	index++;
	//18
	v[index].positionW = float3(position.x + fx, position.y + fy, position.z + fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 0.0);
	index++;
	//19
	v[index].positionW = float3(position.x + fx, position.y + fy, position.z - fz);
	v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	v[index].texCoord = float2(1.0, 1.0);
	index++;
	fNormalVector = GetNormal(v[index - 4].positionW, v[index - 3].positionW, v[index - 2].positionW);
	//fNormalVector = normalize(fNormalVector);
	v[index - 4].normal = fNormalVector;
	v[index - 3].normal = fNormalVector;
	v[index - 2].normal = fNormalVector;
	v[index - 1].normal = fNormalVector;

	////20
	//v[index].positionW = float3(position.x + fx, position.y, position.z - fz);
	//v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	//v[index].texCoord = float2(0.0, 0.0);
	//index++;
	////21
	//v[index].positionW = float3(position.x, position.y, position.z - fz);
	//v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	//v[index].texCoord = float2(0.0, 1.0);
	//index++;
	////22
	//v[index].positionW = float3(position.x + fx, position.y, position.z + fz);
	//v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	//v[index].texCoord = float2(1.0, 0.0);
	//index++;
	////23
	//v[index].positionW = float3(position.x, position.y, position.z + fz);
	//v[index].position = mul(float4(v[index].positionW, 1.0f), gmtxViewProjection);
	//v[index].texCoord = float2(1.0, 1.0);
	//index++;
	//
	//fNormalVector = GetNormal(v[index - 4].positionW, v[index - 3].positionW, v[index - 2].positionW);
	////fNormalVector = normalize(fNormalVector);
	//v[index - 4].normal = fNormalVector;
	//v[index - 3].normal = fNormalVector;
	//v[index - 2].normal = fNormalVector;
	//v[index - 1].normal = fNormalVector;




	int i = 0;
	for (int count = 0; count < 5; count++)
	{
		stream.Append(v[i + 0]);
		stream.Append(v[i + 2]);
		stream.Append(v[i + 1]);
		stream.Append(v[i + 3]);
		i += 4;
		stream.RestartStrip();
	}
}

//쉐이더 파일("Effect.fx")에 다음과 같이 픽셀 - 쉐이더 함수를 추가한다.
float4 PS_GS_INSTANCE_Cube(GS_INSTANCED_CUBE_GS_OUTPUT input) : SV_Target
{
	float4 cIllumination = Lighting(input.positionW, input.normal);
	float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord) * cIllumination;
	cColor = Fog(cColor, input.positionW);
	return cColor;
	//입력되는 정점의 색상을 그대로 출력한다. 
}

//쉐이더 파일("Effect.fx")에 다음과 같이 픽셀 - 쉐이더 함수를 추가한다.
GBUFFER PS_GS_INSTANCE_Cube_GBUFFER(GS_INSTANCED_CUBE_GS_OUTPUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	output.positionW = input.position;// float4(input.positionW, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);
	return output;
}
/*
fxc /E PS_GS_INSTANCE_Cube_GBUFFER /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo GS_TerrainMeshCube.fx
*/