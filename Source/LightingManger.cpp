#include "stdafx.h"
#include "Timer.h"
#include "LightingManger.h"
#include "HeightMapTerrain.h"
#include "TerrainPlayer.h"

ID3D11Buffer *CLightingManger::m_pd3dcbStaticLights = nullptr;
ID3D11Buffer *CLightingManger::m_pd3dcbDynamicLights = nullptr;

CLightingManger::CLightingManger()
{
	m_pStaticLights = nullptr;
	m_pDynamicLights = nullptr;
	m_load = false;
}

CLightingManger::~CLightingManger()
{
}

void CLightingManger::ReleseInstance()
{
	if (m_pStaticLights) delete m_pStaticLights;
	m_pStaticLights = nullptr;

	if (m_load && m_pd3dcbStaticLights) m_pd3dcbStaticLights->Release();

	if (m_pDynamicLights) delete m_pDynamicLights;
	m_pDynamicLights = nullptr;

	if (m_pd3dcbDynamicLights) m_pd3dcbDynamicLights->Release();

	CSingleTonBase::ReleseInstance();

}


void CLightingManger::Load(ID3D11Device* pd3dDevice)
{

	int width, depth;
	width = 250;
	depth = 160;
	m_load = true;

#ifdef _MULIT_RENDER_TARGET_
#pragma region[StaticLight]
	m_pStaticLights = new LIGHTS;
	::ZeroMemory(m_pStaticLights, sizeof(LIGHTS));
	m_pStaticLights->m_d3dxcGlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);


	//StaticLight00: 전역조명
	m_pStaticLights->m_pStaticLights[0].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[0].m_nType = DIRECTIONAL_LIGHT;
	//RSH '16.08.13
	/*
		조명 작업 중 조명을 명확히 보기 위해 전역 광원이 오브젝트들을 비추지 않도록 Ambient를 바꾼다. 1.0f->0.0f
	*/
	//m_pStaticLights->m_pStaticLights[0].m_d3dxcAmbient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxcAmbient = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxcDiffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxcSpecular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);

	//StaticLight01: 레드팀 시작 지점
	m_pStaticLights->m_pStaticLights[1].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[1].m_nType = POINT_LIGHT;
	m_pStaticLights->m_pStaticLights[1].m_fRange = 5.0f;
	m_pStaticLights->m_pStaticLights[1].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[1].m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[1].m_d3dxcSpecular = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[1].m_d3dxvPosition = XMFLOAT3(13, 5, 64);
	m_pStaticLights->m_pStaticLights[1].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[1].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);

	//StaticLight02: 블루팀 시작 지점
	m_pStaticLights->m_pStaticLights[2].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[2].m_nType = POINT_LIGHT;
	m_pStaticLights->m_pStaticLights[2].m_fRange = 5.0f;
	m_pStaticLights->m_pStaticLights[2].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[2].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[2].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[2].m_d3dxvPosition = XMFLOAT3(238, 5, 64);
	m_pStaticLights->m_pStaticLights[2].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[2].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);

	//StaticLight03: 리스폰 지점 (1시)
	m_pStaticLights->m_pStaticLights[3].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[3].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[3].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[3].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[3].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[3].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[3].m_d3dxvPosition = XMFLOAT3(161, 20.f, 145);
	m_pStaticLights->m_pStaticLights[3].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[3].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[3].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[3].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[3].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//StaticLight04: 리스폰 지점 (2시)
	m_pStaticLights->m_pStaticLights[4].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[4].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[4].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[4].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[4].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[4].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[4].m_d3dxvPosition = XMFLOAT3(239, 20.f, 132);
	m_pStaticLights->m_pStaticLights[4].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[4].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[4].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[4].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[4].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//StaticLight05: 리스폰 지점 (4시)
	m_pStaticLights->m_pStaticLights[5].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[5].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[5].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[5].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[5].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[5].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[5].m_d3dxvPosition = XMFLOAT3(238, 20.f, 17);
	m_pStaticLights->m_pStaticLights[5].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[5].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[5].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[5].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[5].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//StaticLight06: 리스폰 지점 (5시)
	m_pStaticLights->m_pStaticLights[6].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[6].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[6].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[6].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[6].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[6].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[6].m_d3dxvPosition = XMFLOAT3(159, 20.f, 9);
	m_pStaticLights->m_pStaticLights[6].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[6].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[6].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[6].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[6].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//StaticLight07: 리스폰 지점 (7시)
	m_pStaticLights->m_pStaticLights[7].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[7].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[7].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[7].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[7].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[7].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[7].m_d3dxvPosition = XMFLOAT3(92, 20.f, 9);
	m_pStaticLights->m_pStaticLights[7].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[7].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[7].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[7].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[7].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//StaticLight08: 리스폰 지점 (8시)
	m_pStaticLights->m_pStaticLights[8].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[8].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[8].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[8].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[8].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[8].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[8].m_d3dxvPosition = XMFLOAT3(15, 20.f, 17);
	m_pStaticLights->m_pStaticLights[8].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[8].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[8].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[8].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[8].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//StaticLight09: 리스폰 지점 (10시)
	m_pStaticLights->m_pStaticLights[9].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[9].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[9].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[9].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[9].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[9].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[9].m_d3dxvPosition = XMFLOAT3(11, 20.f, 132);
	m_pStaticLights->m_pStaticLights[9].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[9].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[9].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[9].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[9].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//StaticLight10: 리스폰 지점 (11시)
	m_pStaticLights->m_pStaticLights[10].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[10].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[10].m_fRange = 60.0f;
	m_pStaticLights->m_pStaticLights[10].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[10].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[10].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[10].m_d3dxvPosition = XMFLOAT3(90, 20.f, 145);
	m_pStaticLights->m_pStaticLights[10].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[10].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pStaticLights->m_pStaticLights[10].m_fFalloff = 20.0f;
	m_pStaticLights->m_pStaticLights[10].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[10].m_fTheta = (float)cos(D3DXToRadian(10.0f));


	//절벽 스포라이트A
	m_pStaticLights->m_pStaticLights[11].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[11].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[11].m_fRange = 100.0f;
	m_pStaticLights->m_pStaticLights[11].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[11].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pStaticLights->m_pStaticLights[11].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[11].m_d3dxvPosition = XMFLOAT3(100, 5, 137);
	m_pStaticLights->m_pStaticLights[11].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[11].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[11].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[11].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pStaticLights->m_pStaticLights[11].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//절벽 스포라이트B
	m_pStaticLights->m_pStaticLights[12].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[12].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[12].m_fRange = 100.0f;
	m_pStaticLights->m_pStaticLights[12].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[12].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pStaticLights->m_pStaticLights[12].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[12].m_d3dxvPosition = XMFLOAT3(110, 3, 137);
	m_pStaticLights->m_pStaticLights[12].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[12].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[12].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[12].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pStaticLights->m_pStaticLights[12].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//절벽 스포라이트C
	m_pStaticLights->m_pStaticLights[13].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[13].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[13].m_fRange = 100.0f;
	m_pStaticLights->m_pStaticLights[13].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[13].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pStaticLights->m_pStaticLights[13].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[13].m_d3dxvPosition = XMFLOAT3(152, 5, 140);
	m_pStaticLights->m_pStaticLights[13].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[13].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[13].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[13].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pStaticLights->m_pStaticLights[13].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//절벽 스포라이트D
	m_pStaticLights->m_pStaticLights[14].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[14].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[14].m_fRange = 100.0f;
	m_pStaticLights->m_pStaticLights[14].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[14].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pStaticLights->m_pStaticLights[14].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[14].m_d3dxvPosition = XMFLOAT3(138, 5, 135);
	m_pStaticLights->m_pStaticLights[14].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[14].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[14].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[14].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pStaticLights->m_pStaticLights[14].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//절벽 스포라이트E
	m_pStaticLights->m_pStaticLights[15].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[15].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[15].m_fRange = 100.0f;
	m_pStaticLights->m_pStaticLights[15].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[15].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pStaticLights->m_pStaticLights[15].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[15].m_d3dxvPosition = XMFLOAT3(135, 7, 130);
	m_pStaticLights->m_pStaticLights[15].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[15].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[15].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[15].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pStaticLights->m_pStaticLights[15].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//절벽 스포라이트F
	m_pStaticLights->m_pStaticLights[16].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[16].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[16].m_fRange = 100.0f;
	m_pStaticLights->m_pStaticLights[16].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[16].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pStaticLights->m_pStaticLights[16].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[16].m_d3dxvPosition = XMFLOAT3(124, 7, 132);
	m_pStaticLights->m_pStaticLights[16].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[16].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[16].m_fFalloff = 12.0f;
	m_pStaticLights->m_pStaticLights[16].m_fPhi = (float)cos(D3DXToRadian(50.0f));
	m_pStaticLights->m_pStaticLights[16].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//다리장식: 좌측로고
	m_pStaticLights->m_pStaticLights[17].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[17].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[17].m_fRange = 8.0f;
	m_pStaticLights->m_pStaticLights[17].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[17].m_d3dxcDiffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pStaticLights->m_pStaticLights[17].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[17].m_d3dxvPosition = XMFLOAT3(85.5, 2.5, 79);
	m_pStaticLights->m_pStaticLights[17].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[17].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[17].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[17].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pStaticLights->m_pStaticLights[17].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//다리장식: 우측로고
	m_pStaticLights->m_pStaticLights[18].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[18].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[18].m_fRange = 10.0f;
	m_pStaticLights->m_pStaticLights[18].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[18].m_d3dxcDiffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pStaticLights->m_pStaticLights[18].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[18].m_d3dxvPosition = XMFLOAT3(164, 2.5, 79);
	m_pStaticLights->m_pStaticLights[18].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[18].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[18].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[18].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pStaticLights->m_pStaticLights[18].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	//다리장식: 좌측초코칩
	m_pStaticLights->m_pStaticLights[19].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[19].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[19].m_fRange = 30.0f;
	m_pStaticLights->m_pStaticLights[19].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[19].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pStaticLights->m_pStaticLights[19].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[19].m_d3dxvPosition = XMFLOAT3(105, 3.0, 97.5);
	m_pStaticLights->m_pStaticLights[19].m_d3dxvDirection = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[19].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[19].m_fFalloff = 8.0f;
	m_pStaticLights->m_pStaticLights[19].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[19].m_fTheta = (float)cos(D3DXToRadian(10.0f));
	
	//다리장식: 우측붕어빵
	m_pStaticLights->m_pStaticLights[20].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[20].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[20].m_fRange = 30.0f;
	m_pStaticLights->m_pStaticLights[20].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[20].m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[20].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[20].m_d3dxvPosition = XMFLOAT3(145, 3.0, 96.5);
	m_pStaticLights->m_pStaticLights[20].m_d3dxvDirection = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[20].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[20].m_fFalloff = 2.0f;
	m_pStaticLights->m_pStaticLights[20].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[20].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//다리장식: 우측막대사탕
	m_pStaticLights->m_pStaticLights[21].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[21].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[21].m_fRange = 30.0f;
	m_pStaticLights->m_pStaticLights[21].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[21].m_d3dxcDiffuse = XMFLOAT4(0.8f, 0.4f, 0.4f, 1.0f);
	m_pStaticLights->m_pStaticLights[21].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[21].m_d3dxvPosition = XMFLOAT3(146, 2.5, 27);
	m_pStaticLights->m_pStaticLights[21].m_d3dxvDirection = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[21].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[21].m_fFalloff = 2.0f;
	m_pStaticLights->m_pStaticLights[21].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	m_pStaticLights->m_pStaticLights[21].m_fTheta = (float)cos(D3DXToRadian(10.0f));

	//중앙로고받침대
	m_pStaticLights->m_pStaticLights[22].m_bEnable = 0.0f;
	m_pStaticLights->m_pStaticLights[22].m_nType = POINT_LIGHT;
	m_pStaticLights->m_pStaticLights[22].m_fRange = 8.0f;
	m_pStaticLights->m_pStaticLights[22].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[22].m_d3dxcDiffuse = XMFLOAT4(0.2f, 0.2f, 0.8f, 1.0f);
	m_pStaticLights->m_pStaticLights[22].m_d3dxcSpecular = XMFLOAT4(0.2f, 0.2f, 0.8f, 1.0f);
	m_pStaticLights->m_pStaticLights[22].m_d3dxvPosition = XMFLOAT3(126, 4, 24);
	m_pStaticLights->m_pStaticLights[22].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);

	//중앙로고받침대
	m_pStaticLights->m_pStaticLights[23].m_bEnable = 0.0f;
	m_pStaticLights->m_pStaticLights[23].m_nType = POINT_LIGHT;
	m_pStaticLights->m_pStaticLights[23].m_fRange = 8.0f;
	m_pStaticLights->m_pStaticLights[23].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[23].m_d3dxcDiffuse = XMFLOAT4(0.8f, 0.2f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[23].m_d3dxcSpecular = XMFLOAT4(0.8f, 0.2f, 0.2f, 1.0f);
	m_pStaticLights->m_pStaticLights[23].m_d3dxvPosition = XMFLOAT3(126, 4, 73);
	m_pStaticLights->m_pStaticLights[23].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);

	//중앙로고
	m_pStaticLights->m_pStaticLights[24].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[24].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[24].m_fRange = 30.0f;
	m_pStaticLights->m_pStaticLights[24].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[24].m_d3dxcDiffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pStaticLights->m_pStaticLights[24].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[24].m_d3dxvPosition = XMFLOAT3(126, 5, 40);
	m_pStaticLights->m_pStaticLights[24].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_pStaticLights->m_pStaticLights[24].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[24].m_fFalloff = 10.0f;
	m_pStaticLights->m_pStaticLights[24].m_fPhi = (float)cos(D3DXToRadian(20.0f));
	m_pStaticLights->m_pStaticLights[24].m_fTheta = (float)cos(D3DXToRadian(10.0f));


	//중앙로고
	m_pStaticLights->m_pStaticLights[25].m_bEnable = 1.0f;
	m_pStaticLights->m_pStaticLights[25].m_nType = SPOT_LIGHT;
	m_pStaticLights->m_pStaticLights[25].m_fRange = 30.0f;
	m_pStaticLights->m_pStaticLights[25].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[25].m_d3dxcDiffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pStaticLights->m_pStaticLights[25].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pStaticLights->m_pStaticLights[25].m_d3dxvPosition = XMFLOAT3(126, 5, 65);
	m_pStaticLights->m_pStaticLights[25].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[25].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pStaticLights->m_pStaticLights[25].m_fFalloff = 10.0f;
	m_pStaticLights->m_pStaticLights[25].m_fPhi = (float)cos(D3DXToRadian(20.0f));
	m_pStaticLights->m_pStaticLights[25].m_fTheta = (float)cos(D3DXToRadian(10.0f));


	UINT staticLightCount = 26;

	//42,135
	//

	for (int k = 1; k < 16; k++)
	{
		m_pStaticLights->m_pStaticLights[staticLightCount].m_bEnable = 1.0f;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_nType = SPOT_LIGHT;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fRange = 100.0f;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxcDiffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxvPosition = XMFLOAT3(15, 2.5+(k%2), k*10);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxvDirection = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fFalloff = 8.0f;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fPhi = (float)cos(D3DXToRadian(30.0f));
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fTheta = (float)cos(D3DXToRadian(10.0f));

		staticLightCount++;
	}

	for (int k = 1; k < 16; k++)
	{
		m_pStaticLights->m_pStaticLights[staticLightCount].m_bEnable = 1.0f;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_nType = SPOT_LIGHT;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fRange = 100.0f;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxcDiffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxvPosition = XMFLOAT3(236, 2.5 + (k % 2), k * 10);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxvDirection = XMFLOAT3(1.0f, 0.0f, 0.0f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fFalloff = 8.0f;
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fPhi = (float)cos(D3DXToRadian(30.0f));
		m_pStaticLights->m_pStaticLights[staticLightCount].m_fTheta = (float)cos(D3DXToRadian(10.0f));

		staticLightCount++;
	}

	////좌측 벽 스프라이트
	//m_pStaticLights->m_pStaticLights[23].m_bEnable = 1.0f;
	//m_pStaticLights->m_pStaticLights[23].m_nType = SPOT_LIGHT;
	//m_pStaticLights->m_pStaticLights[23].m_fRange = 100.0f;
	//m_pStaticLights->m_pStaticLights[23].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pStaticLights->m_pStaticLights[23].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pStaticLights->m_pStaticLights[23].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	//m_pStaticLights->m_pStaticLights[23].m_d3dxvPosition = XMFLOAT3(100, 5, 137);
	//m_pStaticLights->m_pStaticLights[23].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//m_pStaticLights->m_pStaticLights[23].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	//m_pStaticLights->m_pStaticLights[23].m_fFalloff = 8.0f;
	//m_pStaticLights->m_pStaticLights[23].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	//m_pStaticLights->m_pStaticLights[23].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	////절벽 스포라이트
	//m_pStaticLights->m_pStaticLights[1].m_bEnable = 1.0f;
	//m_pStaticLights->m_pStaticLights[1].m_nType = SPOT_LIGHT;
	//m_pStaticLights->m_pStaticLights[1].m_fRange = 100.0f;
	//m_pStaticLights->m_pStaticLights[1].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pStaticLights->m_pStaticLights[1].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_pStaticLights->m_pStaticLights[1].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	//m_pStaticLights->m_pStaticLights[1].m_d3dxvPosition = XMFLOAT3(width / 2, 1, depth / 2);
	//m_pStaticLights->m_pStaticLights[1].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//m_pStaticLights->m_pStaticLights[1].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	//m_pStaticLights->m_pStaticLights[1].m_fFalloff = 8.0f;
	//m_pStaticLights->m_pStaticLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	//m_pStaticLights->m_pStaticLights[1].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	
	
	////WarplogoLeft
	//m_pStaticLights->m_pStaticLights[3].m_bEnable = 1.0f;
	//m_pStaticLights->m_pStaticLights[3].m_nType = SPOT_LIGHT;
	//m_pStaticLights->m_pStaticLights[3].m_fRange = 20.0f;
	//m_pStaticLights->m_pStaticLights[3].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pStaticLights->m_pStaticLights[3].m_d3dxcDiffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	//m_pStaticLights->m_pStaticLights[3].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_pStaticLights->m_pStaticLights[3].m_d3dxvPosition = XMFLOAT3(85.5, 2.0f, 82.5f);
	//m_pStaticLights->m_pStaticLights[3].m_d3dxvDirection = XMFLOAT3(0.0f, 1.0f, 0.1f);
	//m_pStaticLights->m_pStaticLights[3].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	//m_pStaticLights->m_pStaticLights[3].m_fFalloff = 20.0f;
	//m_pStaticLights->m_pStaticLights[3].m_fPhi = (float)cos(D3DXToRadian(140.0f));
	//m_pStaticLights->m_pStaticLights[3].m_fTheta = (float)cos(D3DXToRadian(15.0f));
	//
	////WarplogoRight
	//m_pStaticLights->m_pStaticLights[4].m_bEnable = 1.0f;
	//m_pStaticLights->m_pStaticLights[4].m_nType = SPOT_LIGHT;
	//m_pStaticLights->m_pStaticLights[4].m_fRange = 20.0f;
	//m_pStaticLights->m_pStaticLights[4].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pStaticLights->m_pStaticLights[4].m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
	//m_pStaticLights->m_pStaticLights[4].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_pStaticLights->m_pStaticLights[4].m_d3dxvPosition = XMFLOAT3(164.f, 2.0f, 82.5f);
	//m_pStaticLights->m_pStaticLights[4].m_d3dxvDirection = XMFLOAT3(0.0f, 1.0f, 0.1f);
	//m_pStaticLights->m_pStaticLights[4].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	//m_pStaticLights->m_pStaticLights[4].m_fFalloff = 20.0f;
	//m_pStaticLights->m_pStaticLights[4].m_fPhi = (float)cos(D3DXToRadian(140.0f));
	//m_pStaticLights->m_pStaticLights[4].m_fTheta = (float)cos(D3DXToRadian(15.0f));
	//
	//m_pStaticLights->m_pStaticLights[5].m_bEnable = 1.0f;
	//m_pStaticLights->m_pStaticLights[5].m_nType = SPOT_LIGHT;
	//m_pStaticLights->m_pStaticLights[5].m_fRange = 10.0f;
	//m_pStaticLights->m_pStaticLights[5].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pStaticLights->m_pStaticLights[5].m_d3dxcDiffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	//m_pStaticLights->m_pStaticLights[5].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	//m_pStaticLights->m_pStaticLights[5].m_d3dxvPosition = XMFLOAT3(width / 2, 10, depth / 2);
	//m_pStaticLights->m_pStaticLights[5].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	//m_pStaticLights->m_pStaticLights[5].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	//m_pStaticLights->m_pStaticLights[5].m_fFalloff = 20.0f;
	//m_pStaticLights->m_pStaticLights[5].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	//m_pStaticLights->m_pStaticLights[5].m_fTheta = (float)cos(D3DXToRadian(15.0f));

	//int lightCnt = 5;
	//
	//float xPos;
	//float yPos;
	//float zPos;
	//XMFLOAT4 tmpColor;
	//float phiAngle;
	//RSH '16.05.15
	
	///*
	//정적조명 배치
	//*/
	//for (int x = 0; x < 10; ++x)
	//{
	//	xPos = ((x*20.0f) + 26.5f);
	//	for (int z = 0; z < 8; ++z)
	//	{
	//		yPos = (cos(x*z*2.25f))*10.0f + 30.0f;
	//		zPos = ((z*15.0f) + 9.5f);
	//
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_bEnable = 1.0f;
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_nType = SPOT_LIGHT;
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_fRange = 60.0f;
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//
	//		if (x % 2 && z % 2)
	//		{
	//			tmpColor = XMFLOAT4(0.3f, 0.0f, 0.2f, 1.0f);
	//		}
	//		else if (x % 2)
	//		{
	//			tmpColor = XMFLOAT4(0.0f, 0.3f, 0.2f, 1.0f);
	//		}
	//		else if (z % 2)
	//		{
	//			tmpColor = XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
	//		}
	//		else
	//		{
	//			tmpColor = XMFLOAT4(0.6f, 0.2f, 0.8f, 1.0f);
	//		}
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_d3dxcDiffuse = tmpColor;
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_d3dxcSpecular = tmpColor;
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_d3dxvPosition = XMFLOAT3(xPos, 20.f, zPos);
	//
	//
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	//
	//
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_fFalloff = 20.0f;
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_fPhi = (float)cos(D3DXToRadian(30.0f));
	//		m_pStaticLights->m_pStaticLights[lightCnt].m_fTheta = (float)cos(D3DXToRadian(10.0f));
	//		lightCnt++;
	//	}
	//}

#pragma endregion

	#pragma region [Dynamic Lights]

		m_pDynamicLights = new LIGHTS_Dynamic;
		::ZeroMemory(m_pDynamicLights, sizeof(LIGHTS_Dynamic));

		m_pDynamicLights->m_pDynamicLights[0].m_bEnable = 1.0f;
		m_pDynamicLights->m_pDynamicLights[0].m_nType = SHADOW_LIGHT;
		m_pDynamicLights->m_pDynamicLights[0].m_fRange = 0.2f;
		m_pDynamicLights->m_pDynamicLights[0].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pDynamicLights->m_pDynamicLights[0].m_d3dxcDiffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		m_pDynamicLights->m_pDynamicLights[0].m_d3dxcSpecular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		m_pDynamicLights->m_pDynamicLights[0].m_d3dxvPosition = XMFLOAT3(45, 1.0f, 26);
		m_pDynamicLights->m_pDynamicLights[0].m_d3dxvDirection = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_pDynamicLights->m_pDynamicLights[0].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		m_pDynamicLights->m_pDynamicLights[0].m_fFalloff = 20.0f;
		m_pDynamicLights->m_pDynamicLights[0].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pDynamicLights->m_pDynamicLights[0].m_fTheta = (float)cos(D3DXToRadian(15.0f));

		//중앙그라운드 회전A
		m_pDynamicLights->m_pDynamicLights[1].m_bEnable = 1.0f;
		m_pDynamicLights->m_pDynamicLights[1].m_nType = SPOT_LIGHT;
		m_pDynamicLights->m_pDynamicLights[1].m_fRange = 10.0f;
		m_pDynamicLights->m_pDynamicLights[1].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pDynamicLights->m_pDynamicLights[1].m_d3dxcDiffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		m_pDynamicLights->m_pDynamicLights[1].m_d3dxcSpecular = XMFLOAT4(0.7f, 0.7f, 0.7f, 0.0f);
		m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition = XMFLOAT3(width / 2, 10.0f, depth / 2);
		m_pDynamicLights->m_pDynamicLights[1].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		m_pDynamicLights->m_pDynamicLights[1].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		m_pDynamicLights->m_pDynamicLights[1].m_fFalloff = 20.0f;
		m_pDynamicLights->m_pDynamicLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pDynamicLights->m_pDynamicLights[1].m_fTheta = (float)cos(D3DXToRadian(12.0f));

		//중앙그라운드 회전B
		m_pDynamicLights->m_pDynamicLights[2].m_bEnable = 1.0f;
		m_pDynamicLights->m_pDynamicLights[2].m_nType = SPOT_LIGHT;
		m_pDynamicLights->m_pDynamicLights[2].m_fRange = 10.0f;
		m_pDynamicLights->m_pDynamicLights[2].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pDynamicLights->m_pDynamicLights[2].m_d3dxcDiffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		m_pDynamicLights->m_pDynamicLights[2].m_d3dxcSpecular = XMFLOAT4(0.7f, 0.7f, 0.7f, 0.0f);
		m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition = XMFLOAT3(width / 2, 10.0f, depth / 2);
		m_pDynamicLights->m_pDynamicLights[2].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		m_pDynamicLights->m_pDynamicLights[2].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		m_pDynamicLights->m_pDynamicLights[2].m_fFalloff = 20.0f;
		m_pDynamicLights->m_pDynamicLights[2].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pDynamicLights->m_pDynamicLights[2].m_fTheta = (float)cos(D3DXToRadian(12.0f));
	

		for (int i = 3; i < MAX_DYNAMIC_LIGHTS; i++)
		{
			m_pDynamicLights->m_pDynamicLights[i].m_bEnable = 1.0f;
			m_pDynamicLights->m_pDynamicLights[i].m_nType = SPOT_LIGHT;
			m_pDynamicLights->m_pDynamicLights[i].m_fRange = 10.0f;
			m_pDynamicLights->m_pDynamicLights[i].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
			m_pDynamicLights->m_pDynamicLights[i].m_d3dxcDiffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			m_pDynamicLights->m_pDynamicLights[i].m_d3dxcSpecular = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			//m_pDynamicLights->m_pDynamicLights[i].m_d3dxvPosition;
			m_pDynamicLights->m_pDynamicLights[i].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
			m_pDynamicLights->m_pDynamicLights[i].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
			m_pDynamicLights->m_pDynamicLights[i].m_fFalloff = 30.0f;
			m_pDynamicLights->m_pDynamicLights[i].m_fPhi = (float)cos(D3DXToRadian(50.0f));
			m_pDynamicLights->m_pDynamicLights[i].m_fTheta = (float)cos(D3DXToRadian(15.0f));
		}

		////좌측그라운드 하단 회전A
		//m_pDynamicLights->m_pDynamicLights[3].m_bEnable = 1.0f;
		//m_pDynamicLights->m_pDynamicLights[3].m_nType = SPOT_LIGHT;
		//m_pDynamicLights->m_pDynamicLights[3].m_fRange = 10.0f;
		//m_pDynamicLights->m_pDynamicLights[3].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[3].m_d3dxcDiffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[3].m_d3dxcSpecular = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
		////m_pDynamicLights->m_pDynamicLights[3].m_d3dxvPosition;
		//m_pDynamicLights->m_pDynamicLights[3].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		//m_pDynamicLights->m_pDynamicLights[3].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		//m_pDynamicLights->m_pDynamicLights[3].m_fFalloff = 30.0f;
		//m_pDynamicLights->m_pDynamicLights[3].m_fPhi = (float)cos(D3DXToRadian(50.0f));
		//m_pDynamicLights->m_pDynamicLights[3].m_fTheta = (float)cos(D3DXToRadian(15.0f));
		//
		////좌측그라운드 하단 회전B
		//m_pDynamicLights->m_pDynamicLights[4].m_bEnable = 1.0f;
		//m_pDynamicLights->m_pDynamicLights[4].m_nType = SPOT_LIGHT;
		//m_pDynamicLights->m_pDynamicLights[4].m_fRange = 10.0f;
		//m_pDynamicLights->m_pDynamicLights[4].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[4].m_d3dxcDiffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[4].m_d3dxcSpecular = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
		////m_pDynamicLights->m_pDynamicLights[4].m_d3dxvPosition;
		//m_pDynamicLights->m_pDynamicLights[4].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		//m_pDynamicLights->m_pDynamicLights[4].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		//m_pDynamicLights->m_pDynamicLights[4].m_fFalloff = 30.0f;
		//m_pDynamicLights->m_pDynamicLights[4].m_fPhi = (float)cos(D3DXToRadian(50.0f));
		//m_pDynamicLights->m_pDynamicLights[4].m_fTheta = (float)cos(D3DXToRadian(15.0f));
		//
		////좌측그라운드 상단 회전A
		//m_pDynamicLights->m_pDynamicLights[5].m_bEnable = 1.0f;
		//m_pDynamicLights->m_pDynamicLights[5].m_nType = SPOT_LIGHT;
		//m_pDynamicLights->m_pDynamicLights[5].m_fRange = 10.0f;
		//m_pDynamicLights->m_pDynamicLights[5].m_d3dxcAmbient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[5].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[5].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		////m_pDynamicLights->m_pDynamicLights[5].m_d3dxvPosition;
		//m_pDynamicLights->m_pDynamicLights[5].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		//m_pDynamicLights->m_pDynamicLights[5].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		//m_pDynamicLights->m_pDynamicLights[5].m_fFalloff = 20.0f;
		//m_pDynamicLights->m_pDynamicLights[5].m_fPhi = (float)cos(D3DXToRadian(50.0f));
		//m_pDynamicLights->m_pDynamicLights[5].m_fTheta = (float)cos(D3DXToRadian(15.0f));
		//
		////좌측그라운드 상단 회전B
		//m_pDynamicLights->m_pDynamicLights[6].m_bEnable = 1.0f;
		//m_pDynamicLights->m_pDynamicLights[6].m_nType = SPOT_LIGHT;
		//m_pDynamicLights->m_pDynamicLights[6].m_fRange = 10.0f;
		//m_pDynamicLights->m_pDynamicLights[6].m_d3dxcAmbient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[6].m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		//m_pDynamicLights->m_pDynamicLights[6].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		////m_pDynamicLights->m_pDynamicLights[6].m_d3dxvPosition;
		//m_pDynamicLights->m_pDynamicLights[6].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		//m_pDynamicLights->m_pDynamicLights[6].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		//m_pDynamicLights->m_pDynamicLights[6].m_fFalloff = 20.0f;
		//m_pDynamicLights->m_pDynamicLights[6].m_fPhi = (float)cos(D3DXToRadian(50.0f));
		//m_pDynamicLights->m_pDynamicLights[6].m_fTheta = (float)cos(D3DXToRadian(15.0f));
	
		//int MaxCount = 0;
		//for (int y = 0; y < MaxCount; ++y)
		//{
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_bEnable = 1.0f;
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_nType = (rand() % 2) ? POINT_LIGHT : SPOT_LIGHT;
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_fRange = 15.0f;
		//
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_d3dxvPosition =
		//			XMFLOAT3(y * 10, 0, y * 8);
		//
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_d3dxcDiffuse = RANDOM_COLOR;
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_d3dxcSpecular = RANDOM_COLOR;
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_fFalloff = 20.0f;
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		//		m_pDynamicLights->m_pDynamicLights[cnt].m_fTheta = (float)cos(D3DXToRadian(15.0f));
		//}
	#pragma endregion


	#else
		m_load = true;
		m_pStaticLights = new LIGHTS;
		::ZeroMemory(m_pStaticLights, sizeof(LIGHTS));
		m_pStaticLights->m_d3dxcGlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

		m_pStaticLights->m_pStaticLights[0].m_bEnable = 1.0f;
		m_pStaticLights->m_pStaticLights[0].m_nType = POINT_LIGHT;
		m_pStaticLights->m_pStaticLights[0].m_fRange = 300.0f;
		m_pStaticLights->m_pStaticLights[0].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
		m_pStaticLights->m_pStaticLights[0].m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		m_pStaticLights->m_pStaticLights[0].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
		m_pStaticLights->m_pStaticLights[0].m_d3dxvPosition = XMFLOAT3(300.0f, 300.0f, 300.0f);
		m_pStaticLights->m_pStaticLights[0].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_pStaticLights->m_pStaticLights[0].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);

		m_pStaticLights->m_pStaticLights[1].m_bEnable = 1.0f;
		m_pStaticLights->m_pStaticLights[1].m_nType = SPOT_LIGHT;
		m_pStaticLights->m_pStaticLights[1].m_fRange = 100.0f;
		m_pStaticLights->m_pStaticLights[1].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pStaticLights->m_pStaticLights[1].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		m_pStaticLights->m_pStaticLights[1].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
		m_pStaticLights->m_pStaticLights[1].m_d3dxvPosition = XMFLOAT3(500.0f, 300.0f, 500.0f);
		m_pStaticLights->m_pStaticLights[1].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
		m_pStaticLights->m_pStaticLights[1].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
		m_pStaticLights->m_pStaticLights[1].m_fFalloff = 8.0f;
		m_pStaticLights->m_pStaticLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pStaticLights->m_pStaticLights[1].m_fTheta = (float)cos(D3DXToRadian(20.0f));

		m_pStaticLights->m_pStaticLights[2].m_bEnable = 1.0f;
		m_pStaticLights->m_pStaticLights[2].m_nType = DIRECTIONAL_LIGHT;
		m_pStaticLights->m_pStaticLights[2].m_d3dxcAmbient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pStaticLights->m_pStaticLights[2].m_d3dxcDiffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		m_pStaticLights->m_pStaticLights[2].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		m_pStaticLights->m_pStaticLights[2].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);

		m_pStaticLights->m_pStaticLights[3].m_bEnable = 1.0f;
		m_pStaticLights->m_pStaticLights[3].m_nType = SPOT_LIGHT;
		m_pStaticLights->m_pStaticLights[3].m_fRange = 60.0f;
		m_pStaticLights->m_pStaticLights[3].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pStaticLights->m_pStaticLights[3].m_d3dxcDiffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		m_pStaticLights->m_pStaticLights[3].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		m_pStaticLights->m_pStaticLights[3].m_d3dxvPosition = XMFLOAT3(500.0f, 300.0f, 500.0f);
		m_pStaticLights->m_pStaticLights[3].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		m_pStaticLights->m_pStaticLights[3].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
		m_pStaticLights->m_pStaticLights[3].m_fFalloff = 20.0f;
		m_pStaticLights->m_pStaticLights[3].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pStaticLights->m_pStaticLights[3].m_fTheta = (float)cos(D3DXToRadian(15.0f));
	#endif


	m_nShadowPosHeight = Helper::RuntimeDataModify(float(), "ShaodwHeight");

	CLightingManger::CreateConstBuffers(pd3dDevice);
}

void CLightingManger::CreateConstBuffers(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pStaticLights;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbStaticLights);

	//D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS_Dynamic);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pDynamicLights;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbDynamicLights);

}

void CLightingManger::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	#ifdef _MULIT_RENDER_TARGET_

		//KYT '16.04.12
		/*
		Dynamic Lights
		*/
		{
			XMVECTOR position = PLAYER->GetCamera()->GetPosition();
			XMFLOAT3 charPos = PLAYER->GetPlayer()->GetPositionXMFLOAT3();

			charPos.y = m_nShadowPosHeight;

			//if (INPUT->KeyDown(YT_KEY::YK_F4))
			//	m_nShadowPosHeight = Helper::RuntimeDataModify("ShaodwHeight");

			m_pDynamicLights->m_pDynamicLights[0].m_d3dxvPosition = charPos;

			static D3DXVECTOR3 d3dxvGroundRotated_01_ = D3DXVECTOR3(WORLD_DEPTH*0.10f, 0.0f, 0.0f);
			static D3DXVECTOR3 d3dxvGroundRotated_02_ = D3DXVECTOR3(-WORLD_DEPTH*0.10f, 0.0f, 0.0f);

			D3DXMATRIX d3dxmtxRotate;
			XMFLOAT3 d3dxvRotateCenter;
			XMFLOAT3 dirRotated;

			//1: 중심을 돌고 있음.
			D3DXMatrixRotationYawPitchRoll(&d3dxmtxRotate, (float)(30.0f*(TIMEMGR->GetTimeElapsed()))* (D3DX_PI / 180.0), 0.0f, 0.0f);
			D3DXVec3TransformCoord(&d3dxvGroundRotated_01_, &d3dxvGroundRotated_01_, &d3dxmtxRotate);
			d3dxvRotateCenter = XMFLOAT3(WORLD_WIDTH / 2, 10.0f, WORLD_DEPTH / 2);
			dirRotated = XMFLOAT3(d3dxvGroundRotated_01_.x, d3dxvGroundRotated_01_.y, d3dxvGroundRotated_01_.z);
			
			m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition
				= XMFLOAT3(d3dxvRotateCenter.x + dirRotated.x,
					d3dxvRotateCenter.y + dirRotated.y,
					d3dxvRotateCenter.z + dirRotated.z);

			//2: 중심을 돌고 있음.
			//D3DXMatrixRotationYawPitchRoll(&d3dxmtxRotate, (float)(30.0f*(TIMEMGR->GetTimeElapsed()))* (D3DX_PI / 180.0), 0.0f, 0.0f);
			D3DXVec3TransformCoord(&d3dxvGroundRotated_02_, &d3dxvGroundRotated_02_, &d3dxmtxRotate);
			dirRotated = XMFLOAT3(d3dxvGroundRotated_02_.x, d3dxvGroundRotated_02_.y, d3dxvGroundRotated_02_.z);

			m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition
				= XMFLOAT3(d3dxvRotateCenter.x + dirRotated.x,
					d3dxvRotateCenter.y + dirRotated.y,
					d3dxvRotateCenter.z + dirRotated.z);

			//좌측 하단 그라운드 3,4
			m_pDynamicLights->m_pDynamicLights[3].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.x - 65.0f,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.z - 55.0f);

			m_pDynamicLights->m_pDynamicLights[4].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.x - 65.0f,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.z - 55.0f);

			//좌측 상단 그라운드 5,6
			m_pDynamicLights->m_pDynamicLights[5].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.x-64,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.z-4);
		
			m_pDynamicLights->m_pDynamicLights[6].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.x-64,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.z-4);

			//우측 하단 그라운드 7,8 
			m_pDynamicLights->m_pDynamicLights[7].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.x + 72,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.z - 55);

			m_pDynamicLights->m_pDynamicLights[8].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.x + 72,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.z - 55);

			//우측 상단 그라운드 9,10
			m_pDynamicLights->m_pDynamicLights[9].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.x + 71,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.z - 4);

			m_pDynamicLights->m_pDynamicLights[10].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.x + 71,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.z - 4);

			//우측 상단 그라운드 11,12
			m_pDynamicLights->m_pDynamicLights[11].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.x,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[1].m_d3dxvPosition.z-54);

			m_pDynamicLights->m_pDynamicLights[12].m_d3dxvPosition
				= XMFLOAT3(m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.x,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.y,
					m_pDynamicLights->m_pDynamicLights[2].m_d3dxvPosition.z-54);
		
			D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
			pd3dDeviceContext->Map(m_pd3dcbDynamicLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
			LIGHTS_Dynamic *pcbLight = (LIGHTS_Dynamic*)d3dMappedResource.pData;
			m_pDynamicLights->m_d3dxvCameraPosition = position;
			memcpy(pcbLight, m_pDynamicLights, sizeof(LIGHTS_Dynamic));
			pd3dDeviceContext->Unmap(m_pd3dcbStaticLights, 0);
			pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_DYNAMIC_LIGHT, 1, &m_pd3dcbDynamicLights);
			pd3dDeviceContext->GSSetConstantBuffers(PS_CB_SLOT_DYNAMIC_LIGHT, 1, &m_pd3dcbDynamicLights);
			pd3dDeviceContext->CSSetConstantBuffers(PS_CB_SLOT_DYNAMIC_LIGHT, 1, &m_pd3dcbDynamicLights);
		}
		//RSH '16.05.15
		/*
		월드 전체를 돌고 있는 조명배치
		*/
	#endif
	if (m_load == false)return;
	//KYT '16.02.25
	/*
	뭔가 쓸때없는 연산을 하고있어서 맘에 안들어
	*/
	{
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		pd3dDeviceContext->Map(m_pd3dcbStaticLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
		memcpy(pcbLight, m_pStaticLights, sizeof(LIGHTS));
		pd3dDeviceContext->Unmap(m_pd3dcbStaticLights, 0);
		pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_STATIC_LIGHT, 1, &m_pd3dcbStaticLights);
		pd3dDeviceContext->CSSetConstantBuffers(PS_CB_SLOT_STATIC_LIGHT, 1, &m_pd3dcbStaticLights);
	}
	m_load = false;
}

void CLightingManger::DebugLight(ID3D11DeviceContext * pd3dDeviceContext)
{
	//width = 250;
	//depth = 160;

	m_pStaticLights->m_d3dxcGlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pStaticLights->m_pStaticLights[0].m_bEnable = 0.0f;
	m_pStaticLights->m_pStaticLights[0].m_nType = POINT_LIGHT;
	m_pStaticLights->m_pStaticLights[0].m_fRange = 10.0f;
	m_pStaticLights->m_pStaticLights[0].m_d3dxcAmbient = XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	//m_pStaticLights->m_pStaticLights[0].m_d3dxvPosition = XMFLOAT3(300.0f, 300.0f, 300.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxvPosition = XMFLOAT3(250 / 2, 10, 160 / 2);
	m_pStaticLights->m_pStaticLights[0].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pStaticLights->m_pStaticLights[0].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
}

void CLightingManger::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS* pLights)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbStaticLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbStaticLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_STATIC_LIGHT, 1, &m_pd3dcbStaticLights);
}

void CLightingManger::Update(float fTimeElapsed, std::shared_ptr<CCamera> pCamera)
{
	return;
}
