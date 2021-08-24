#pragma once
class CHeightMap : public Object
{
private:
	BYTE						*m_pHeightMapImage;
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_d3dxvScale;

public:
	CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, XMVECTOR d3dxvScale);
	virtual ~CHeightMap();

	float GetHeight(float x, float z, bool bReverseQuad = false);
	XMVECTOR& GetHeightMapNormal(int x, int z);
	XMFLOAT3& GetHeightMapNormalXMFLOAT3(int x, int z);
	XMVECTOR& GetScale() { return(XMLoadFloat3(&m_d3dxvScale)); }
	XMFLOAT3& GetScaleXMFLAOT3() { return((m_d3dxvScale)); }

	BYTE *GetHeightMapImage() { return(m_pHeightMapImage); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};









