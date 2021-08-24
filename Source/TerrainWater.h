#pragma once
class CTerrainWater : public CGameObject
{
public:
	CTerrainWater(ID3D11Device *pd3dDevice, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMVECTOR d3dxvScale);
	virtual ~CTerrainWater();

private:
	int							m_nWidth;
	int							m_nLength;

	XMFLOAT3					m_d3dxvScale;

public:
	XMFLOAT4X4					m_d3dxmtxTexture;

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext,std::shared_ptr<CCamera> pCamera);
};


