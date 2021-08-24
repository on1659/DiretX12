
//#include "PostProcessingDefine.hlsli"

cbuffer DownScaleConstants : register(b0)
{
	//다운 스케일 해상도 계산 x 너비, y 높이
    uint2 Res : packoffset(c0);
	//다운스케일 이미지의 총 픽셀 수
    uint Domain : packoffset(c0.z);
	//첫번째 pass에 적용된 그룹 수
    uint GroupSize : packoffset(c0.w);
	//적응값
    float Adaptation : packoffset(c1);
	//bloom 임계값 비율
    float fBloomThreshold : packoffset(c1.y);
}

#define MAX_GROUPS 64
groupshared float SharedAvgFinal[MAX_GROUPS];

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

//Use CS

RWStructuredBuffer<float> AverageLum    : register(u0); //결과 group개수 만큼의 AverageLum
RWTexture2D<float4> HDRDownScale        : register(u1);

StructuredBuffer<float> AverageValues1D : register(t0);
StructuredBuffer<float> PrevAvgLum      : register(t1); //이전 평균 휘도 값

Texture2D HDRTex                        : register(t0); //only firstPass
groupshared float SharedPositions[1024];


//Use PS
Texture2D<float4> HDRDownScaleTex   : register(t0);
StructuredBuffer<float> AvgLum      : register(t1);
Texture2D<float4> BloomTexture      : register(t2);

RWTexture2D<float4> Bloom           : register(u0);

