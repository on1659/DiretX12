
#include "GBuffer.fx"
#include "Light.fx"

//-------------------------------------------------------VS Use-------------------------------//
cbuffer cbViewProjectionMatrix  : register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};
cbuffer cbWorldMatrix			: register(b2)
{
	matrix gmtxWorld : packoffset(c0);
};
cbuffer cbTextureMatrix			: register(b3)
{
	matrix gmtxTexture : packoffset(c0);
};

//-------------------------------------------------------VS Use-------------------------------//

SamplerState gssDefault			: register(s0);

Texture2D gtxtDefault			: register(t0);
Texture2D gtxtNormal			: register(t1);
Texture2D gtxtDisplacement		: register(t2);

//-------------------------------------------------------VS Use-------------------------------//

//-------------------------------------------------------DS Use-------------------------------//
cbuffer cbTextureMatrixBump		: register(b1)
{
	float4 cbSize : packoffset(c0);
};
//-------------------------------------------------------DS Use-------------------------------//

