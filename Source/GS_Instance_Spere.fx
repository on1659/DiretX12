
#include "GSShader.fx"

VS_INSTANCED_SPHERE_OUT VS_GS_Instance_Sphere(VS_INSTANCED_SPHERE_IN input)
{

	float4 position = float4(0, 0, 0, 1.0f);

	VS_INSTANCED_SPHERE_OUT output = (VS_INSTANCED_SPHERE_OUT)0;
	output.size = input.size;
	output.mtxTransform = input.mtxTransform;

	return output;
}

[maxvertexcount(64)]
void GS_GS_Instance_Sphere(point VS_INSTANCED_SPHERE_OUT input[1], uint nPrimID: SV_PrimitiveID, inout TriangleStream<GS_INSTANCED_SPHERE_OUT> stream)
{
	GS_INSTANCED_SPHERE_OUT v[4];

	//월드 좌표
	float fRadius = input[0].size.x;
	float nSlices = input[0].size.y;
	float nStacks = input[0].size.z;
	float PI = 3.141592f;

	//matrix gmtxViewProjection = mul(gmtxView, gmtxProjection);

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	int k = 0;

	phi_j = float(PI / nStacks) * nPrimID;
	phi_jj = float(PI / nStacks) * (nPrimID + 1);

	fRadius_j = fRadius * sin(phi_j);
	fRadius_jj = fRadius * sin(phi_jj);
	y_j = fRadius*cos(phi_j);
	y_jj = fRadius*cos(phi_jj);
	for (float i = 0; i < nSlices; i++)
	{
		theta_i = float(2 * PI / nSlices) * i;
		theta_ii = float(2 * PI / nSlices) * (i + 1);

		v[0].normal = mul(float4(fRadius_j*cos(theta_i), y_j, fRadius_j*sin(theta_i), 1.0f), input[0].mtxTransform).xyz;
		v[0].position = mul(float4(v[0].normal, 1.0f), gmtxViewProjection);
		v[0].positionW = v[0].normal;
		v[0].texCoord = float2(0, 0);

		v[1].normal = mul(float4(fRadius_jj*cos(theta_i), y_jj, fRadius_jj*sin(theta_i), 1.0f), input[0].mtxTransform).xyz;
		v[1].position = mul(float4(v[1].normal, 1.0f), gmtxViewProjection);
		v[1].positionW = v[1].normal;
		v[1].texCoord = float2(1, 0);

		v[2].normal = mul(float4(fRadius_j*cos(theta_ii), y_j, fRadius_j*sin(theta_ii), 1.0f), input[0].mtxTransform).xyz;
		v[2].position = mul(float4(v[2].normal, 1.0f), gmtxViewProjection);
		v[2].positionW = v[2].normal;
		v[2].texCoord = float2(0, 1);

		v[3].normal = mul(float4(fRadius_jj*cos(theta_ii), y_jj, fRadius_jj*sin(theta_ii), 1.0f), input[0].mtxTransform).xyz;
		v[3].position = mul(float4(v[3].normal, 1.0f), gmtxViewProjection);
		v[3].positionW = v[3].normal;
		v[3].texCoord = float2(1, 1);

		stream.Append(v[0]);
		stream.Append(v[2]);
		stream.Append(v[1]);
		stream.Append(v[3]);
		stream.RestartStrip();
	}
}


float4 PS_GS_Instance_Sphere(GS_INSTANCED_SPHERE_OUT input) : SV_Target
{
	input.normal = normalize(input.normal);
	float4 cIllumination = Lighting(input.positionW, input.normal);
	float4 cColor = gtxtDefault.Sample(gssDefault, input.texCoord) * cIllumination;
	cColor = Fog(cColor, input.positionW);
	return cColor;
}

//쉐이더 파일("Effect.fx")에 다음과 같이 픽셀 - 쉐이더 함수를 추가한다.
GBUFFER PS_GS_Instance_Sphere_GBUFFER(GS_INSTANCED_SPHERE_OUT input) : SV_Target
{
	GBUFFER output = (GBUFFER)0;
	output.positionW = float4(input.positionW, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.diffuse = gtxtDefault.Sample(gssDefault, input.texCoord);
	return output;
}

// fxc /E PS_GS_Instance_Sphere /T gs_5_0 /Od /Zi /Fo CompiledGs.fxo Effecttest.fx



