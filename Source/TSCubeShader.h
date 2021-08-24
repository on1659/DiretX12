#pragma once

//KYT '16.02.28
/*
  Normal, BumpMap Cube
*/

#include "InstancedObjectsShader.h"


struct fSize
{
	XMFLOAT4 size;
};

class CTSCubeMesh;

class CTSCubeShader : public CInstancedObjectsShader
{
private:
	bool							bCubeSizeSetting;
	ID3D11Buffer					*m_pd3dcbSize;

public:
	XMFLOAT4X4						m_d3dxmtxTexture;

	XMFLOAT3						m_f3BumpScale;

public:

	CTSCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CTSCubeShader(std::unique_ptr<CMesh> pMesh);

	CTSCubeShader(int nObjects = 1);

	virtual ~CTSCubeShader();

	void SetBumpScale(const float& x, const float& y, const float& z)
	{
		if (m_pMesh)
		{
			m_f3BumpScale.x = m_pMesh->GetSize().x * 0.1f;
			m_f3BumpScale.y = m_pMesh->GetSize().y * 0.1f;
			m_f3BumpScale.z = m_pMesh->GetSize().z * 0.1f;
			return;
		}
		m_f3BumpScale.x = x;
		m_f3BumpScale.y = y;
		m_f3BumpScale.z = z;
	}
		
	void SetBumpScale(const XMFLOAT3& fScale = XMFLOAT3{ 0.0f, 0.0f, 0.0f })
	{
		SetBumpScale(fScale.x, fScale.y, fScale.z);
	}

	void CreateShader(ID3D11Device *pd3dDevice) override;

	void CreateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

	void AnimateObjects(float fTimeElapsed);

};

