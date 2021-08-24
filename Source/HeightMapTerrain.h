#pragma once

#include "HeightMap.h"
 
class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMVECTOR d3dxvScale);
	virtual ~CHeightMapTerrain();

private:
	CHeightMap					*m_pHeightMap;

	int							m_nWidth;
	int							m_nLength;

	XMFLOAT3					m_d3dxvScale;

	CWaterGridMesh				**m_ppWaterMeshes;
	int							m_nWaterMeshes;

public:
	float GetHeight(float x, float z, bool bReverseQuad = false) { return(m_pHeightMap->GetHeight(x, z, bReverseQuad) * m_d3dxvScale.y); } //World
	XMVECTOR& GetNormal(float x, float z) { return(m_pHeightMap->GetHeightMapNormal(int(x / m_d3dxvScale.x), int(z / m_d3dxvScale.z))); }

	int GetHeightMapWidth() { return(m_pHeightMap->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMap->GetHeightMapLength()); }

	XMVECTOR& GetScale() { return(XMLoadFloat3(&m_d3dxvScale)); }
	XMFLOAT3& GetScaleXMFLOAT3() { return(m_d3dxvScale); }
	float GetWidth() { return(m_nWidth * m_d3dxvScale.x); }
	float GetLength() { return(m_nLength * m_d3dxvScale.z); }

#ifdef _AABB_
	float GetPeakHeight() { return(m_bcMeshBoundingCube.m_d3dxvMaximum.y); }
#else
	//XMCost 생각안함
	float GetPeakHeight() { return(m_bcMeshBoundingCube.Center.y + m_bcMeshBoundingCube.Extents.y); }

#endif
#ifdef _WITH_TERRAIN_TEXTURE_ARRAY
	ID3D11Buffer				*m_pd3dcbTextureIndex;
#endif
};
