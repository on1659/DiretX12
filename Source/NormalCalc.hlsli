
Texture2D gtxtNormal : register(t1);

SamplerState LinearSS
{
    Filter = MIN_MAG_MIP_LINEAR;

    AddressU = WRAP;
    AddressV = WRAP;
};

float3 CalcNormal(float3 vNormal, float3 vTangent, float2 vTexcoord)
{
    vNormal = normalize(vNormal);
    vTangent = normalize(vTangent - dot(vTangent, vNormal) * vNormal);
    float3 vBinormal = normalize(cross(vNormal, vTangent));

    float3x3 TBN = float3x3(vTangent, vBinormal, vNormal);
	
    float3 vNormalW = gtxtNormal.Sample(LinearSS, vTexcoord).rgb;
    vNormalW = 2.0f * vNormalW - 1.0f;
	
    return mul(vNormalW, TBN);
}

float3 CalcNormal(float3 vNormal, float3 vTangent)
{
    vNormal = normalize(vNormal);
    vTangent = normalize(vTangent - dot(vTangent, vNormal) * vNormal);
    float3 vBinormal = normalize(cross(vNormal, vTangent));

    float3x3 TBN = float3x3(vTangent, vBinormal, vNormal);
	
    float3 vNormalW = vNormal;
    vNormalW = 2.0f * vNormalW - 1.0f;
	
    return mul(vNormalW, TBN);
}