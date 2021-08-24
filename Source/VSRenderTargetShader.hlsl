
struct VS_TEXTURED_OUTPUT 
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
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
    return (output);
}


   