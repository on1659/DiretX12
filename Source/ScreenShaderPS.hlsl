
#include "Light.fx"
#include "GBuffer.fx"


/*
	fxc /E VSTextureToScreen /T vs_5_0 /Od /Zi /Fo CompiledGs.fxo ScreenShader.fx
*/
/*
	Gbuffer.

    Diffuse.a == 15 -> 캐릭터 외곽선

	positionW.xyz = pos;
	psoitionW.w = Lighting할지 말지 말지결정

	normal.xyz = normal;
	normal.z = Fog 할지 말지 결정 (임시)
*/
//-----------------------------------------------------------------Defeerred Lighting -------------------------------------------------/
SamplerState        gssSamplerState	 : register(s0);

Texture2D           gtxtDiffuse		 : register(t0);
Texture2D           gtxtPositionW	 : register(t1);
Texture2D           gtxtNormal	 	 : register(t2);
Texture2D           gtxtShadowMap	 : register(t5);

cbuffer cbBulrOption : register(b5)
{
    float4 gBlurOption;
};

cbuffer cbOUTLINE_DATA : register(b13)
{
    float  gOutLineWidth;
    float3 gOutLineColor;
}

//-----------------------------------------------------------------Defeerred Lighting -------------------------------------------------/

//----------------------------------------------------------------Mini Map-------------------------------------------------/
#define FRAME_WIDTH  1280.0f
#define FRAME_HEIGHT 720.0f
static const float gMinmapLeft =  (float)(0.5f - 126.0f / FRAME_WIDTH) + (float)(460 / FRAME_WIDTH);
static const float gMinmapTop  =  (float)(0.5f - 80.0f / FRAME_HEIGHT) + (float)(175 / FRAME_HEIGHT);
static const float DPI_X = 1.0f / 256.0f;
static const float DPI_Y = 1.0f / 160.0f;
//----------------------------------------------------------------Mini Map-------------------------------------------------/

static const float gBlurMode =  5.0f;
static const float gRespawn  = 20.0f;

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};
 

/*
  0.0625 = 1 / 16;
  0.125  = 1 / 8;
  0.25   = 1 / 4
*/

float4 GBuffer_WeightedAveragefilterBlurring(float2 textCoord)
{
	float2 tex = textCoord;
	//
	if( 0.4 < tex.x && tex.x < 0.6 && 0.5 < tex.y)
		return (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x, tex.y)));

	float2 minus = float2(0.99, 0.99);
	float2 plus = float2(1.01, 1.01);


	float4 color = 0.0f;

	color += 0.0625 * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * minus.x, tex.y * minus.y)));
	color += 0.125  * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x,		   tex.y * minus.y)));
	color += 0.0625 * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * plus.x,  tex.y * minus.y)));

	color += 0.125  * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * minus.x, tex.y)));
	color += 0.25   * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x,		   tex.y)));
	color += 0.125  * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * plus.x,  tex.y)));

	color += 0.0625 * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * minus.x, tex.y * plus.y)));
	color += 0.125  * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x,		   tex.y * plus.y)));
	color += 0.0625 * (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * plus.x,  tex.y * plus.y)));

	return color;

}

bool MinimapPoint(float2 textCoord)
{
	float x = textCoord.x;
	float y = textCoord.y;

	float playerX = gvCameraPosition.x / FRAME_WIDTH;
	float playerZ = (180 - gvCameraPosition.z) / FRAME_HEIGHT;


	if (gMinmapLeft + playerX <= x & x <= gMinmapLeft + playerX + DPI_X)
	{
		if (gMinmapTop + playerZ <= y & y <= gMinmapTop + playerZ + DPI_Y)
		{
			return true;
		}
	}
	return false;
}

bool IsOutLine(float2 tex)
{
    float fWidth = gOutLineWidth;
    if (fWidth <= 0.0f)
        return false;
    float2 minus = float2(1.0f, 1.0f);
    minus -= fWidth;
    float2 plus = float2(1.0f, 1.0f);;
    plus += fWidth;

    float color = 0.0f;
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * minus.x, tex.y * minus.y))).a;
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x, tex.y * minus.y))).a;
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * plus.x, tex.y * minus.y))).a;
    
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * minus.x, tex.y))).a;
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x, tex.y))).a;
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * plus.x, tex.y))).a;
    
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * minus.x, tex.y * plus.y))).a;
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x, tex.y * plus.y))).a;
    color += (gtxtDiffuse.Sample(gssSamplerState, float2(tex.x * plus.x, tex.y * plus.y))).a;
    
    if (color >= 15.0f)
        return true;
    return false;
}

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{

	//------------------- 조명 계산을 위한 정보 얻기 ------------------- 
	    float4 normal    = gtxtNormal.Sample(gssSamplerState, input.texCoord);
	    float4 positionW = gtxtPositionW.Sample(gssSamplerState, input.texCoord);
	    float4 shadowMap = gtxtShadowMap.Sample(gssSamplerState, input.texCoord);
	//------------------- 조명 계산을 위한 정보 얻기 ------------------- 

    //KYT '16.04.22
	/*
	  ㆍ블러링을 위해 PS 수정
	  ㆍ블러링을 위해서 앞에(pos, normal) 데이터를 먼저 받아온다.
	*/
    float4 diffuse = gtxtDiffuse.Sample(gssSamplerState, input.texCoord);

    if (diffuse.a != 15.0f)
    {
        if (IsOutLine(input.texCoord))
            return float4(gOutLineColor, 0);
    }


	//KYT '16.04.28
	/*
		Run Mode On  
		자세한건 stdafx.h 참조
	*/
    if (gBlurOption.y == gBlurMode)
	{
		if (normal.w == 10) // 10 블러링 상태가 아니라면
		{
			diffuse = gtxtDiffuse.Sample(gssSamplerState, input.texCoord);
		}
		else                // 블러링 상태라면
		{
			diffuse = GBuffer_WeightedAveragefilterBlurring(input.texCoord);
		}
	}
	else        // 안눌림
	{
		diffuse = gtxtDiffuse.Sample(gssSamplerState, input.texCoord);
	}

    if (gBlurOption.w == gRespawn)
    {
        if (normal.w != 20)
        {
            diffuse *= 0.3f;
           // normal.w = 0;
        }
    }
    
    //-------------------------------------CS에서 안해도됌------------------------------------
		//if (MinimapPoint(input.texCoord) == true) 
		//return float4(1, 0, 0, 1);
    //-------------------------------------CS에서 안해도됌------------------------------------


	//조명 계산에 필요한 구조체 정의
	DEFFERRED_MATERIAL dMaterial = (DEFFERRED_MATERIAL)0;
	dMaterial.m_cAmbient = gMaterial.m_cAmbient;
	dMaterial.m_cSpecular = gMaterial.m_cSpecular;
	//조명 계산에 필요한 구조체 정의

	//조명 계산 및 출력 색상 정하기
	float4 cColor = diffuse;
	float4 defferred_cIllumination;
     
	if (positionW.w == 1 )
	{
		defferred_cIllumination  = Defferred_Lighting(positionW.xyz, normal.xyz, dMaterial);
		defferred_cIllumination += Defferred_Lighting_Dynamic(positionW.xyz, normal.xyz);
		cColor = cColor * defferred_cIllumination;
	}

	if (normal.w == 1)
	{
		cColor = Fog(cColor, positionW.xyz);
    }
    else if (normal.w == 2)
    {
        cColor = FogAlpahRemove(cColor, positionW.xyz);
    }
	else if (normal.w == 4)
	{
        cColor = FogDensity(cColor, 2.0f, positionW.xyz);
    }
    else if (normal.w == 3)
    {
        cColor = FogSpace(cColor, 0.9);
    }
    else if (normal.w == 5)
    {
       cColor = FogWaterFall(cColor, 0.7f, positionW.xyz);
    }

    if(gBlurOption.z > 1.0f)
    {
        cColor.r *= gBlurOption.z;
        cColor.g *= gBlurOption.z;
        cColor.b *= gBlurOption.z;
    }

    cColor.a = 0.1f;
	return(cColor);
}

/*
fxc /E PSTextureToScreen /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo ScreenShader.fx
*/

