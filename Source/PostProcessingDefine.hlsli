
//#include "PostProcessingDefine.hlsli"

cbuffer DownScaleConstants : register(b0)
{
	//�ٿ� ������ �ػ� ��� x �ʺ�, y ����
    uint2 Res : packoffset(c0);
	//�ٿ���� �̹����� �� �ȼ� ��
    uint Domain : packoffset(c0.z);
	//ù��° pass�� ����� �׷� ��
    uint GroupSize : packoffset(c0.w);
	//������
    float Adaptation : packoffset(c1);
	//bloom �Ӱ谪 ����
    float fBloomThreshold : packoffset(c1.y);
}

#define MAX_GROUPS 64
groupshared float SharedAvgFinal[MAX_GROUPS];

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

//Use CS

RWStructuredBuffer<float> AverageLum    : register(u0); //��� group���� ��ŭ�� AverageLum
RWTexture2D<float4> HDRDownScale        : register(u1);

StructuredBuffer<float> AverageValues1D : register(t0);
StructuredBuffer<float> PrevAvgLum      : register(t1); //���� ��� �ֵ� ��

Texture2D HDRTex                        : register(t0); //only firstPass
groupshared float SharedPositions[1024];


//Use PS
Texture2D<float4> HDRDownScaleTex   : register(t0);
StructuredBuffer<float> AvgLum      : register(t1);
Texture2D<float4> BloomTexture      : register(t2);

RWTexture2D<float4> Bloom           : register(u0);

