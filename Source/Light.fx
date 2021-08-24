//--------------------------------------------------------------------------------------

#define MAX_LIGHTS			56
#define MAX_DYNAMIC_LIGHTS	13


#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f
#define SHAODW_LIGHT		4.0f

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES
//#define _WITH_REFLECT

#define LINEAR_FOG			1.0f
#define EXP_FOG				2.0f
#define EXP2_FOG			3.0f
#define E					2.71828182846

struct MATERIAL
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular; //a = power
	float4				m_cEmissive;
};

struct LIGHT
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular;
	float3				m_vPosition;
	float				m_fRange;
	float3				m_vDirection;
	float				m_nType;
	float3				m_vAttenuation;
	float 				m_fFalloff;
	float 				m_fTheta; //cos(m_fTheta)
	float				m_fPhi; //cos(m_fPhi)
	float				m_bEnable;
	float				padding;
};

cbuffer cbStaticLights : register(b1)
{
	LIGHT	gLights[MAX_LIGHTS];
	float4	gcLightGlobalAmbient;
};

cbuffer cbDynamicLights : register(b2)
{
	LIGHT	gDynmaicLights[MAX_DYNAMIC_LIGHTS];
	float4	gvCameraPosition;
};

cbuffer cbMaterial : register(b3)
{
	MATERIAL			gMaterial;
};

//뽀그
#define CULL_CAMERA_DISTANCE 28
static const float4 gcFogColor =     { 1.0f, 0.9f, 1.0f, 1.0f };
static const float4 gvFogParameter = { LINEAR_FOG, 2.0f, CULL_CAMERA_DISTANCE, 0.9f }; //(mode, Start, End, Desity);


struct LIGHTEDCOLOR
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular;
};

struct DEFFERRED_MATERIAL
{
	float4 m_cAmbient;
	float4 m_cSpecular;
};

// 방향성 조명의 효과를 계산하는 함수이다. 
// 방향성 조명은 조명까지의 거리에 따라 조명의 양이 변하지 않는다.
LIGHTEDCOLOR DirectionalLight(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	/// directional light 의 ToLight 벡터는 빛의 방향의 반대 방향이다. (저 하늘 멀리 광원이 있다고 가정하므로)
	float3 vToLight = normalize(-gLights[i].m_vDirection);
	float fDiffuseFactor = dot(vToLight, vNormal);
	if (fDiffuseFactor > 0.0f)
	{

		// 스팩큘러 라이트 계산
		if (gMaterial.m_cSpecular.a != 0.0f)
		{
			float3 vReflect = reflect(-vToLight, vNormal);	// -vToLight : 입사각, vNormal : 반사평면
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
			output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
		}

		// 디퓨즈 라이트 계산
		output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
	}

	// 앰비넌트 라이트 계산.
	output.m_cAmbient = gMaterial.m_cAmbient * gLights[i].m_cAmbient;
	return(output);
}

//점 조명의 효과를 계산하는 함수이다.
LIGHTEDCOLOR PointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;


	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);

				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));


		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;

	}
	return(output);
}

LIGHTEDCOLOR SpotLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);

				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
#ifdef _WITH_THETA_PHI_CONES
		float fAlpha = max(dot(-vToLight, gLights[i].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gLights[i].m_fPhi) / (gLights[i].m_fTheta - gLights[i].m_fPhi)), 0.0f), gLights[i].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), gLights[i].m_fFalloff);
#endif
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor * fSpotFactor);
		output.m_cDiffuse *= fAttenuationFactor * fSpotFactor;
		output.m_cSpecular *= fAttenuationFactor * fSpotFactor;
	}
	return(output);
}

float4 Lighting(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
		float3 vToCamera = normalize(gvCameraPosition.xyz - vPosition);
		LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (i = 0; i < MAX_LIGHTS; i++)
		{
			if (gLights[i].m_bEnable == 1.0f)
			{
				if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
				{
					LightedColor = DirectionalLight(i, vNormal, vToCamera);
					cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
				}
				if (gLights[i].m_nType == POINT_LIGHT)
				{
					LightedColor = PointLight(i, vPosition, vNormal, vToCamera);
					cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
				}
				if (gLights[i].m_nType == SPOT_LIGHT)
				{
					LightedColor = SpotLight(i, vPosition, vNormal, vToCamera);
					cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
				}
			
			}
		}
	cColor += (gcLightGlobalAmbient * gMaterial.m_cAmbient);

	// 일반적으로 재질의 Diffused 알파값을 조명의 알파값으로 설정한다.
	cColor.a = 1.0f;// gMaterial.m_cDiffuse.a;

	return (cColor);
}


// 방향성 조명의 효과를 계산하는 함수이다. 
// 방향성 조명은 조명까지의 거리에 따라 조명의 양이 변하지 않는다.
LIGHTEDCOLOR DynmaicDirectionalLight(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	/// directional light 의 ToLight 벡터는 빛의 방향의 반대 방향이다. (저 하늘 멀리 광원이 있다고 가정하므로)
	float3 vToLight = normalize(-gDynmaicLights[i].m_vDirection);
	float fDiffuseFactor = dot(vToLight, vNormal);
	if (fDiffuseFactor > 0.0f)
	{

		// 스팩큘러 라이트 계산
		if (gMaterial.m_cSpecular.a != 0.0f)
		{
			float3 vReflect = reflect(-vToLight, vNormal);	// -vToLight : 입사각, vNormal : 반사평면
			float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
			output.m_cSpecular = gMaterial.m_cSpecular * (gDynmaicLights[i].m_cSpecular * fSpecularFactor);
		}

		// 디퓨즈 라이트 계산
		output.m_cDiffuse = gMaterial.m_cDiffuse * (gDynmaicLights[i].m_cDiffuse * fDiffuseFactor);
	}

	// 앰비넌트 라이트 계산.
	output.m_cAmbient = gMaterial.m_cAmbient * gDynmaicLights[i].m_cAmbient;
	return(output);
}

//점 조명의 효과를 계산하는 함수이다.
LIGHTEDCOLOR DynmaicPointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;


	float3 vToLight = gDynmaicLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gDynmaicLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
				float3 vReflect = reflect(-vToLight, vNormal);
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);

				output.m_cSpecular = gMaterial.m_cSpecular * (gDynmaicLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gDynmaicLights[i].m_cDiffuse * fDiffuseFactor);
		}
		float fAttenuationFactor = 1.0f / dot(gDynmaicLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));


		output.m_cAmbient = gMaterial.m_cAmbient * (gDynmaicLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;

	}
	return(output);
}

LIGHTEDCOLOR DynmaicSpotLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gDynmaicLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gDynmaicLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
				float3 vReflect = reflect(-vToLight, vNormal);
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);

				output.m_cSpecular = gMaterial.m_cSpecular * (gDynmaicLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gDynmaicLights[i].m_cDiffuse * fDiffuseFactor);
		}
#ifdef _WITH_THETA_PHI_CONES
		float fAlpha = max(dot(-vToLight, gDynmaicLights[i].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gDynmaicLights[i].m_fPhi) / (gDynmaicLights[i].m_fTheta - gDynmaicLights[i].m_fPhi)), 0.0f), gDynmaicLights[i].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gDynmaicLights[i].m_vDirection), 0.0f), gDynmaicLights[i].m_fFalloff);
#endif
		float fAttenuationFactor = 1.0f / dot(gDynmaicLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gDynmaicLights[i].m_cAmbient * fAttenuationFactor * fSpotFactor);
		output.m_cDiffuse *= fAttenuationFactor * fSpotFactor;
		output.m_cSpecular *= fAttenuationFactor * fSpotFactor;
	}
	return(output);
}

float4 DynmaicLighting(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
	float3 vToCamera = normalize(gvCameraPosition.xyz - vPosition);
	LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (i = 0; i < MAX_LIGHTS; i++)
	{
		if (gDynmaicLights[i].m_bEnable == 1.0f)
		{
			if (gDynmaicLights[i].m_nType == DIRECTIONAL_LIGHT)
			{
				LightedColor = DirectionalLight(i, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gDynmaicLights[i].m_nType == POINT_LIGHT)
			{
				LightedColor = PointLight(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gDynmaicLights[i].m_nType == SPOT_LIGHT)
			{
				LightedColor = SpotLight(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}

		}
	}
	cColor += (gcLightGlobalAmbient * gMaterial.m_cAmbient);

	// 일반적으로 재질의 Diffused 알파값을 조명의 알파값으로 설정한다.
	cColor.a = 1.0f;// gMaterial.m_cDiffuse.a;

	return (cColor);
}

////파티클용 (라이팅을 적용하지 않는다) 직교투영
float4 NoLighting(float3 vPosition, float3 vNormal)
{
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	cColor += gMaterial.m_cAmbient;
	cColor.a = gMaterial.m_cDiffuse.a;
	return(cColor);
}



//-----------------------------------------------------------------------Deferred Lighting-----------------------------------------------/


//-----------------------------------------------------------------------Static Lighting-----------------------------------------------/

LIGHTEDCOLOR Defferred_DirectionalLight(int i, float3 vNormal, float3 vToCamera, DEFFERRED_MATERIAL dMaterial)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = -gLights[i].m_vDirection;
	float fDiffuseFactor = dot(vToLight, vNormal);
	if (fDiffuseFactor > 0.0f)
	{
		if (dMaterial.m_cSpecular.a != 0.0f)
		{
#ifdef _WITH_REFLECT
			float3 vReflect = reflect(-vToLight, vNormal);
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), dMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
			float3 vHalf = normalize(vToCamera + vToLight);
#else
			float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
				float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), dMaterial.m_cSpecular.a);
#endif
			output.m_cSpecular = dMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
		}
		output.m_cDiffuse = /*gMaterial.m_cDiffuse * */(gLights[i].m_cDiffuse * fDiffuseFactor);
	}
	output.m_cAmbient = dMaterial.m_cAmbient * gLights[i].m_cAmbient;
	return(output);
}

LIGHTEDCOLOR Defferred_PointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera, DEFFERRED_MATERIAL dMaterial)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (dMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), dMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
					float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), dMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = dMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = /*gMaterial.m_cDiffuse * */(gLights[i].m_cDiffuse * fDiffuseFactor);
		}
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = dMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;
	}
	return(output);
}

LIGHTEDCOLOR Defferred_SpotLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera, DEFFERRED_MATERIAL dMaterial)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		if (fDiffuseFactor > 0.0f)
		{
			if (dMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), dMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
					float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), dMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = dMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = /*gMaterial.m_cDiffuse * */(gLights[i].m_cDiffuse * fDiffuseFactor);
		}
#ifdef _WITH_THETA_PHI_CONES
		float fAlpha = max(dot(-vToLight, gLights[i].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gLights[i].m_fPhi) / (gLights[i].m_fTheta - gLights[i].m_fPhi)), 0.0f), gLights[i].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), gLights[i].m_fFalloff);
#endif
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = dMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor * fSpotFactor);
		output.m_cDiffuse *= fAttenuationFactor * fSpotFactor;
		output.m_cSpecular *= fAttenuationFactor * fSpotFactor;
	}
	return(output);
}


//라이팅 함수 변경이 필요함 
float4 Defferred_Lighting(float3 vPosition, float3 vNormal, DEFFERRED_MATERIAL dMaterial)
{
    float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
    float3 vToCamera = normalize(vCameraPosition - vPosition);
    LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR) 0;
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    int i = 0;
	[unroll(121)]for (i = 0; i < MAX_LIGHTS; i++)
	{
		if (gLights[i].m_bEnable == 1.0f)
		{
			if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
			{
				LightedColor = Defferred_DirectionalLight(i, vNormal, vToCamera, dMaterial);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gLights[i].m_nType == POINT_LIGHT)
			{
				LightedColor = Defferred_PointLight(i, vPosition, vNormal, vToCamera, dMaterial);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gLights[i].m_nType == SPOT_LIGHT)
			{
				LightedColor = Defferred_SpotLight(i, vPosition, vNormal, vToCamera, dMaterial);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
		}
	}
	cColor += (gcLightGlobalAmbient * dMaterial.m_cAmbient);
	//cColor.a = gMaterial.m_cDiffuse.a;
	cColor.a = 1.0f;
	return(cColor);
}

//-----------------------------------------------------------------------Static Lighting-----------------------------------------------/



//-----------------------------------------------------------------------Dynamic Lighting-----------------------------------------------/

LIGHTEDCOLOR Defferred_DirectionalLight_Dynamic(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = -gDynmaicLights[i].m_vDirection;
	float fDiffuseFactor = dot(vToLight, vNormal);
	output.m_cAmbient = gDynmaicLights[i].m_cAmbient;
	output.m_cDiffuse = gDynmaicLights[i].m_cDiffuse * fDiffuseFactor;
	return(output);
}

LIGHTEDCOLOR Defferred_PointLight_Dynamic(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = gDynmaicLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gDynmaicLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		float fAttenuationFactor = 1.0f / dot(gDynmaicLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = (gDynmaicLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;
	}
	return(output);
}

LIGHTEDCOLOR Defferred_SpotLight_Dynamic(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gDynmaicLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gDynmaicLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		float fAlpha = max(dot(-vToLight, gDynmaicLights[i].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gDynmaicLights[i].m_fPhi) / (gDynmaicLights[i].m_fTheta - gDynmaicLights[i].m_fPhi)), 0.0f), gDynmaicLights[i].m_fFalloff);
		float fAttenuationFactor = 1.0f / dot(gDynmaicLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = (gDynmaicLights[i].m_cAmbient * fAttenuationFactor * fSpotFactor);
		output.m_cDiffuse *= fAttenuationFactor * fSpotFactor;
		output.m_cSpecular *= fAttenuationFactor * fSpotFactor;
	}
	return(output);
}

bool IsShadow(int i, float3 vPosition)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gDynmaicLights[i].m_vPosition - vPosition;
	float fDistance = length(vToLight);
	if (fDistance <= gDynmaicLights[i].m_fRange)
	{
		return true;
	}
	return false;
}

//라이팅 함수 변경이 필요함 
float4 Defferred_Lighting_Dynamic(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
	float3 vToCamera = normalize(vCameraPosition - vPosition);
	LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (i = 0; i < MAX_DYNAMIC_LIGHTS; i++)
	{
		if (gDynmaicLights[i].m_bEnable == 1.0f)
		{
			if (gDynmaicLights[i].m_nType == DIRECTIONAL_LIGHT)
			{
				LightedColor = Defferred_DirectionalLight_Dynamic(i, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gDynmaicLights[i].m_nType == POINT_LIGHT)
			{
				LightedColor = Defferred_PointLight_Dynamic(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gDynmaicLights[i].m_nType == SPOT_LIGHT)
			{
				LightedColor = Defferred_SpotLight_Dynamic(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
		}
	}
	cColor.a = 1.0f;
	return(cColor);
}

//-----------------------------------------------------------------------Dynamic Lighting-----------------------------------------------/



//-----------------------------------------------------------------------Deferred Lighting-----------------------------------------------/




//뽀그
float4 Fog(float4 cColor, float3 vPosition)
{
	float3 vCameraPosition = gvCameraPosition.xyz;
	float3 vPositionToCamera = vCameraPosition - vPosition;
	float fDistanceToCamera = length(vPositionToCamera);
	float fFogFactor = 0.0f;
	if (gvFogParameter.x == LINEAR_FOG)
	{
		float fFogRange = gvFogParameter.z - gvFogParameter.y;
		fFogFactor = saturate((fDistanceToCamera - gvFogParameter.y) / fFogRange) * gvFogParameter.w;
	}
	return (lerp(cColor, gcFogColor, fFogFactor));
}


//뽀그
float4 FogDensity(float4 cColor, float fRange, float3 vPosition)
{
    float3 vCameraPosition = gvCameraPosition.xyz;
    float3 vPositionToCamera = vCameraPosition - vPosition;
    float fDistanceToCamera = length(vPositionToCamera);
    float fFogFactor = 0.0f;
    if (gvFogParameter.x == LINEAR_FOG)
    {
        float fFogRange = gvFogParameter.z - gvFogParameter.y;
        fFogRange *= fRange;
        fFogFactor = saturate((fDistanceToCamera - gvFogParameter.y) / fFogRange) * (gvFogParameter.w);
    }
    return (lerp(cColor, gcFogColor, fFogFactor));
} 
//뽀그
float4 FogWaterFall(float4 cColor, float fRange, float3 vPosition)
{
    float3 vCameraPosition = gvCameraPosition.xyz;
    float3 vPositionToCamera = vCameraPosition - vPosition;
    float fDistanceToCamera = length(vPositionToCamera);
    float fFogFactor = 0.0f;
    if (gvFogParameter.x == LINEAR_FOG)
    {
        float fFogRange = gvFogParameter.z - gvFogParameter.y;
        fFogFactor = saturate((fDistanceToCamera - gvFogParameter.y) / fFogRange) * (fRange * gvFogParameter.w);
    }
    return (lerp(cColor, gcFogColor, fFogFactor));
}


//뽀그
float4 FogAlpahRemove(float4 cColor, float3 vPosition)
{
	float alpha = cColor.a;
	float3 vCameraPosition = gvCameraPosition.xyz;
	float3 vPositionToCamera = vCameraPosition - vPosition;
	float fDistanceToCamera = length(vPositionToCamera);
	float fFogFactor = 0.0f;
	if (gvFogParameter.x == LINEAR_FOG)
	{
		float fFogRange = gvFogParameter.z - gvFogParameter.y;
		fFogFactor = saturate((fDistanceToCamera - gvFogParameter.y) / fFogRange) * gvFogParameter.w;
	}
	float4 cColorByFog = lerp(cColor, gcFogColor, fFogFactor);
	cColorByFog.a = alpha;
	return (cColorByFog);
}

float4 FogSpace(float4 cColor, float fDensity)
{
	float alpha = cColor.a;
	float4 cColorByFog = lerp(cColor, gcFogColor, fDensity);
	cColorByFog.a = alpha;
	return (cColorByFog);
}


/*
fxc /E Defferred_Lighting /T ps_5_0 /Od /Zi /Fo CompiledGs.fxo Light.fx		
*/


//float4 Blurring(float2 textCoord)
//{
//	float2  tex = textCoord;
//		float4 cColor1 = gtxtDefault.Sample(gssDefault, float2(tex.x * 0.99, tex.y));
//		float4 cColor2 = gtxtDefault.Sample(gssDefault, float2(tex.x, tex.y * 1.01));
//		float4 cColor3 = gtxtDefault.Sample(gssDefault, float2(tex.x * 1.01, tex.y));
//		float4 cColor4 = gtxtDefault.Sample(gssDefault, float2(tex.x, tex.y * 0.99));
//		return  (cColor1 + cColor2 + cColor3 + cColor4) / 4;
//
//}
//
//float4 SobelBlurring(float2 textCoord)
//{
//	float2  tex = textCoord;
//
//		float2 minus = float2(0.99, 0.099);
//		float2 plus = float2(1.01, 1.01);
//
//
//		if (tex.x * plus.x > 1)plus.x = 1;
//	if (tex.y * plus.y > 1)plus.y = 1;
//
//	if (tex.x * minus.x < 0)minus.x = 1;
//	if (tex.x * minus.x < 0)minus.y = 1;
//
//
//	float4 cColor1 = -1 * (gtxtDefault.Sample(gssDefault, float2(tex.x * minus.x, tex.y * minus.y)));
//		float4 cColor2 = -2 * (gtxtDefault.Sample(gssDefault, float2(tex.x * minus.x, tex.y)));
//		float4 cColor3 = -1 * (gtxtDefault.Sample(gssDefault, float2(tex.x * minus.x, tex.y * plus.y)));
//
//		float4 cColor4 = 1 * (gtxtDefault.Sample(gssDefault, float2(tex.x * plus.y, tex.y * minus.y)));
//		float4 cColor5 = 2 * (gtxtDefault.Sample(gssDefault, float2(tex.x * plus.y, tex.y)));
//		float4 cColor6 = 1 * (gtxtDefault.Sample(gssDefault, float2(tex.x * plus.y, tex.y * plus.y)));
//
//		return  (cColor1 + cColor2 + cColor3 + cColor4 + cColor5 + cColor6) / 6;
//
//}
//
//float4 GaussianBlurring(float2 textCoord)
//{
//	float2  tex = textCoord;
//	float2 minus = float2(0.9, 0.9);
//	float2 plus = float2(1.1, 1.1);
//	if (tex.x * plus.x > 1)plus.x = 1;
//	if (tex.y * plus.y > 1)plus.y = 1;
//	if (tex.x * minus.x < 0)minus.x = 1;
//	if (tex.x * minus.x < 0)minus.y = 1;
//	float4 cColor1 = 1 * (gtxtDefault.Sample(gssDefault, float2(tex.x, tex.y *minus.y)));
//	float4 cColor2 = 1 * (gtxtDefault.Sample(gssDefault, float2(tex.x * minus.x, tex.y)));
//	float4 cColor3 = 1 * (gtxtDefault.Sample(gssDefault, float2(tex.x*plus.x, tex.y)));
//	float4 cColor4 = 1 * (gtxtDefault.Sample(gssDefault, float2(tex.x, tex.y *plus.y)));
//	float4 cColor5 = 1 * (gtxtDefault.Sample(gssDefault, float2(tex.x, tex.y)));
//	return  (cColor1 + cColor2 + cColor3 + cColor4 + cColor5) / 5;
//
//}