#pragma once

#include "InstancedObjectsShader.h"

class CMonsterHeadCubeShader : public CInstancedObjectsShader
{
private:
	std::vector<XMMATRIX>		m_fRenderObjectWorldMatrix;
	float fHeight;

public:

	CMonsterHeadCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CMonsterHeadCubeShader(std::unique_ptr<CMesh> pMesh);

	CMonsterHeadCubeShader(int nObjects = 1);

	virtual ~CMonsterHeadCubeShader();

	void AddPosition(const XMFLOAT3& xmPosition);

	void AddPosition(const float& x, const float& y, const float& z);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
	
};

