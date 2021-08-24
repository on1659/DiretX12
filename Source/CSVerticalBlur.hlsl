cbuffer DownScaleConstants : register(b0) {
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

Texture2D<float4> Input : register(t0);
RWTexture2D<float4> Output : register(u0);

static const float SampleWeights[13] = {
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216
};

#define kernelhalf 6
#define groupthreads 128

groupshared float4 SharedInput[groupthreads];

[numthreads(groupthreads, 1, 1)]
void main(uint3 Gid : SV_GroupID, uint GI : SV_GroupIndex) {
	int2 coord = int2(Gid.x, GI - kernelhalf + (groupthreads - kernelhalf * 2) * Gid.y);
	coord = clamp(coord, int2(0, 0), int2(Res.x - 1, Res.y - 1));
	SharedInput[GI] = Input.Load(int3(coord, 0));

	GroupMemoryBarrierWithGroupSync();

	//수직방향 블러
	if (GI >= kernelhalf && GI < (groupthreads - kernelhalf) && ((GI - kernelhalf + (groupthreads - kernelhalf * 2) * Gid.y) < Res.y)) {
		float4 vOut = 0;

		[unroll]
		for (int i = -kernelhalf; i <= kernelhalf; ++i) {
			vOut += SharedInput[GI + i] * SampleWeights[i + kernelhalf];
		}
		Output[coord] = float4(vOut.rgb, 1.0f);
	}
}