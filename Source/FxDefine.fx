
//#define _WITH_TERRAIN_TEXTURE_ARRAY
//-------------------------------------------------------------------------------------------------------------------------------
// Constant Buffer Variables
//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------VS Use-------------------------------//
cbuffer cbViewProjectionMatrix  : register(b0)
{
	matrix gmtxViewProjection : packoffset(c0);
};
//cbuffer cbViewOrthoMatrix		: register(b1)			{	matrix		gmtxOrthoViewProjection : packoffset(c0);};
cbuffer cbWorldMatrix			: register(b2)
{
	matrix gmtxWorld : packoffset(c0);
};
cbuffer cbTextureMatrix			: register(b3)
{
	matrix gmtxTexture : packoffset(c0);
};
cbuffer cbTerrain				: register(b3)
{
	int4 gvTerrainTextureIndex : packoffset(c0);
};
cbuffer cbSkyBox				: register(b4)
{
	int4 gvSkyBoxTextureIndex : packoffset(c0);
};
//-------------------------------------------------------VS Use-------------------------------//

//-----------------------------------------------------------------Use PS SamplerState-------------------------------------------------
SamplerState gssDefault			: register(s0);
SamplerState gssDefaultDetail   : register(s1);
SamplerState gssTerrain			: register(s2);
SamplerState gssTerrainDetail	: register(s3);
SamplerState gssSkyBox		    : register(s4);
//-----------------------------------------------------------------Use PS SamplerState-------------------------------------------------

//-------------------------------------------------------PS Use-------------------------------//
Texture2D gtxtDefault			: register(t0);
Texture2D gtxtDefaultDetail		: register(t1);
Texture2D gtxtTerrain			: register(t10);
Texture2D gtxtTerrainNormal		: register(t11);
Texture2D gtxtTerrainDetail		: register(t12);


TextureCube gtxtSkyBox			: register(t13);
//-------------------------------------------------------PS Use-------------------------------//




