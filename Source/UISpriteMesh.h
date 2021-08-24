#pragma once

class CUISpriteMesh : public CMeshTexturedIlluminated
{
private:
	XMFLOAT3						m_dot[3];
	XMFLOAT3						m_f3BeginPosition;


public:

	CUISpriteMesh(ID3D11Device *pd3dDevice, float fWidth = 30.0f, float fheight = 30.0f);

	virtual ~CUISpriteMesh();

	void GetDot(XMFLOAT3 &a, XMFLOAT3 &b, XMFLOAT3 &c);

	virtual void OffSetSize(ID3D11DeviceContext* pd3dDeviceContext, float x, float y, float z)
	{
		m_fSize.x += x;		m_fSize.y += y;
		m_pd3dxvPositions[1] = XMFLOAT3(+m_fSize.x, +m_fSize.y, 0.0f); //우상;
		m_pd3dxvPositions[2] = XMFLOAT3(+m_fSize.x, -m_fSize.y, 0.0f); //우하 2
		m_pd3dxvPositions[4] = XMFLOAT3(+m_fSize.x, -m_fSize.y, 0.0f); //우하 4
		pd3dDeviceContext->UpdateSubresource(m_pd3dPositionBuffer, 0, NULL, m_pd3dxvPositions, 0, 0);
	}

	virtual void SetSize(ID3D11DeviceContext* pd3dDeviceContext, float x, float y, float z)
	{
		m_fSize.x = x;  m_fSize.y = y; 	m_fSize.z = z;

		int i = 0;
		m_pd3dxvPositions[i++] = XMFLOAT3(-m_fSize.x, +m_fSize.y, -m_fSize.z); //좌상 0
		m_pd3dxvPositions[i++] = XMFLOAT3(+m_fSize.x, +m_fSize.y, -m_fSize.z); //우상 1
		m_pd3dxvPositions[i++] = XMFLOAT3(+m_fSize.x, -m_fSize.y, -m_fSize.z); //우하 2

		m_pd3dxvPositions[i++] = XMFLOAT3(-m_fSize.x, +m_fSize.y, -m_fSize.z); //좌상 3
		m_pd3dxvPositions[i++] = XMFLOAT3(+m_fSize.x, -m_fSize.y, -m_fSize.z); //우하 4
		m_pd3dxvPositions[i++] = XMFLOAT3(-m_fSize.x, -m_fSize.y, -m_fSize.z); //좌하 5

		pd3dDeviceContext->UpdateSubresource(m_pd3dPositionBuffer, 0, NULL, m_pd3dxvPositions, 0, 0);

	}

};
