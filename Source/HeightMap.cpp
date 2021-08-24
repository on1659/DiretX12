#include "stdafx.h"
#include "HeightMap.h"

#define _WITH_APPROXIMATE_OPPOSITE_CORNER

CHeightMap::CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, XMVECTOR d3dxvScale)
{
	m_nWidth = nWidth;
	m_nLength = nLength;
	XMStoreFloat3(&m_d3dxvScale, d3dxvScale);

	BYTE *pHeightMapImage = new BYTE[m_nWidth * m_nLength];

	HANDLE hFile = ::CreateFile(pFileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, nullptr);
	DWORD dwBytesRead;
	::ReadFile(hFile, pHeightMapImage, (m_nWidth * m_nLength), &dwBytesRead, nullptr);
	::CloseHandle(hFile);

	m_pHeightMapImage = new BYTE[m_nWidth * m_nLength];
	for (int y = 0; y < m_nLength; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_pHeightMapImage[x + ((m_nLength - 1 - y)*m_nWidth)] = pHeightMapImage[x + (y*m_nWidth)];
		}
	}

	if (pHeightMapImage) delete[] pHeightMapImage;
}

CHeightMap::~CHeightMap()
{
	if (m_pHeightMapImage) delete[] m_pHeightMapImage;
	m_pHeightMapImage = nullptr;
}

XMFLOAT3& CHeightMap::GetHeightMapNormalXMFLOAT3(int x, int z)
{
	if ((x < 0.0f) || (z < 0.0f) || (x >= m_nWidth) || (z >= m_nLength))
	{
		m_xmFloat3 = XMFLOAT3(0.0f, 1.0f, 0.0f);
		return (m_xmFloat3);
	}
	int nHeightMapIndex = x + (z * m_nWidth);
	int xHeightMapAdd, zHeightMapAdd;
	xHeightMapAdd = (x < (m_nWidth - 1)) ? 1 : -1;
	zHeightMapAdd = (z < (m_nLength - 1)) ? m_nWidth : -m_nWidth;
	float y1 = (float)m_pHeightMapImage[nHeightMapIndex] * m_d3dxvScale.y;
	float y2 = (float)m_pHeightMapImage[nHeightMapIndex + xHeightMapAdd] * m_d3dxvScale.y;
	float y3 = (float)m_pHeightMapImage[nHeightMapIndex + zHeightMapAdd] * m_d3dxvScale.y;
	XMVECTOR vEdge1 = XMVectorSet(0.0f, y3 - y1, m_d3dxvScale.z, 0.0f);
	XMVECTOR vEdge2 = XMVectorSet(m_d3dxvScale.x, y2 - y1, 0.0f, 0.0f);
	XMVECTOR vNormal;
	vNormal = XMVector3Cross(vEdge1, vEdge2);
	vNormal = XMVector3Normalize(vNormal);
	XMStoreFloat3(&m_xmFloat3, vNormal);
	return(m_xmFloat3);
}

XMVECTOR& CHeightMap::GetHeightMapNormal(int x, int z)
{
	if ((x < 0.0f) || (z < 0.0f) || (x >= m_nWidth) || (z >= m_nLength)) return(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	int nHeightMapIndex = x + (z * m_nWidth);
	int xHeightMapAdd, zHeightMapAdd;
	xHeightMapAdd = (x < (m_nWidth - 1)) ? 1 : -1;
	zHeightMapAdd = (z < (m_nLength - 1)) ? m_nWidth : -m_nWidth;
	float y1 = (float)m_pHeightMapImage[nHeightMapIndex] * m_d3dxvScale.y;
	float y2 = (float)m_pHeightMapImage[nHeightMapIndex + xHeightMapAdd] * m_d3dxvScale.y;
	float y3 = (float)m_pHeightMapImage[nHeightMapIndex + zHeightMapAdd] * m_d3dxvScale.y;
	XMVECTOR vEdge1 = XMVectorSet(0.0f, y3 - y1, m_d3dxvScale.z, 0.0f);
	XMVECTOR vEdge2 = XMVectorSet(m_d3dxvScale.x, y2 - y1, 0.0f, 0.0f);
	XMVECTOR vNormal;
	vNormal = XMVector3Cross(vEdge1, vEdge2);
	vNormal = XMVector3Normalize(vNormal);
	return(vNormal);
}

float CHeightMap::GetHeight(float fx, float fz, bool bReverseQuad)
{
	fx = fx / m_d3dxvScale.x;
	fz = fz / m_d3dxvScale.z;
	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= m_nWidth) || (fz >= m_nLength)) return(0.0f);

	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;

	float fBottomLeft = (float)m_pHeightMapImage[x + (z*m_nWidth)];
	float fBottomRight = (float)m_pHeightMapImage[(x + 1) + (z*m_nWidth)];
	float fTopLeft = (float)m_pHeightMapImage[x + ((z + 1)*m_nWidth)];
	float fTopRight = (float)m_pHeightMapImage[(x + 1) + ((z + 1)*m_nWidth)];
	#ifdef _WITH_APPROXIMATE_OPPOSITE_CORNER
		if (bReverseQuad)
		{
			if (fzPercent >= fxPercent)
				fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
			else
				fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
		}
		else
		{
			if (fzPercent < (1.0f - fxPercent))
				fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
			else
				fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
		}
	#endif
	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;

	return(fHeight);
}
