
struct VS_TEXTURED_OUTPUT 
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};

static const float2 arrUV[4] =
{
    float2(0.0, 0.0),
	float2(1.0, 0.0),
	float2(0.0, 1.0),
	float2(1.0, 1.0),
};

//¥Ÿ¡ﬂ ∑ª¥ı≈∏∞Ÿ
VS_TEXTURED_OUTPUT main(uint VertexID : SV_VertexID)
{
    VS_TEXTURED_OUTPUT output;

    output.position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
    output.texCoord = arrUV[VertexID].xy;
    
    return output;
}


