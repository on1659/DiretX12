
Texture2D gtxTexture                : register(t0);
SamplerState gssSamplerState		: register(s0);

struct VS_TEXTURED_OUTPUT 
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};


float4 main(VS_TEXTURED_OUTPUT input) : SV_Target
{
    return gtxTexture.Sample(gssSamplerState, input.texCoord);
}