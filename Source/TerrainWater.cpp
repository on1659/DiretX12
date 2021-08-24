#include "stdafx.h"
#include "TerrainWater.h"

CTerrainWater::CTerrainWater(ID3D11Device *pd3dDevice, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMVECTOR d3dxvScale)
{
	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	XMStoreFloat3(&m_d3dxvScale, d3dxvScale);

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	m_vsMeshes.reserve(cxBlocks * czBlocks);

	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//	오류 위험
			SetMesh(std::make_unique<CWaterGridMesh>(pd3dDevice, xStart, zStart,
				nBlockWidth, nBlockLength, d3dxvScale, nullptr, D3D11_USAGE_DYNAMIC), x + (z*cxBlocks));
		}
	}
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());


	//KYT '16.02.08
	//-------------------------------------------------블렌딩
	Object::SetBlendState("TerrainWater");
	
}

CTerrainWater::~CTerrainWater()
{
}

void CTerrainWater::Animate(float fTimeElapsed)
{
	static XMFLOAT3 d3dxOffset(0.0f, 0.0f, 0.0f);
	d3dxOffset.y += 0.005f * 0.001f;
	d3dxOffset.x = 0.0025f * sinf(4.0f * d3dxOffset.y);
	m_d3dxmtxTexture._41 += d3dxOffset.x;
	m_d3dxmtxTexture._42 += d3dxOffset.y;
}

void CTerrainWater::Render(ID3D11DeviceContext *pd3dDeviceContext,std::shared_ptr<CCamera> pCamera)
{
	CTexture::UpdateConstBuffer(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));

	CGameObject::Render(pd3dDeviceContext, pCamera);
}
