

static const float4 DirectionalLightPosition = { 80.0f, 100.0f, 80.0f , 1.0f};


cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxViewProjection : packoffset(c0);
};

cbuffer cbShaodwViewProjectionMatrix : register(b7)
{
    matrix gmtxShadowViewProjection : packoffset(c0);
};

struct VS_TEXTURED_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 cam_lightPos : POSITION;
    float3 cam_Pos : POSITION1;
};

static const float2 arrUV[4] =
{
    float2(0.0, 0.0),
	float2(1.0, 0.0),
	float2(0.0, 1.0),
	float2(1.0, 1.0),
};

//¥Ÿ¡ﬂ ∑ª¥ı≈∏∞Ÿ
VS_TEXTURED_OUTPUT main(float4 position : POSITION, float2 texCoord : TEXCOORD)
{
    VS_TEXTURED_OUTPUT output = (VS_TEXTURED_OUTPUT) 0;
    output.position = position;
    output.texCoord = texCoord;
    output.cam_lightPos = mul(DirectionalLightPosition, gmtxShadowViewProjection);
    output.cam_Pos = mul(DirectionalLightPosition, gmtxViewProjection);
    return (output);
}


   