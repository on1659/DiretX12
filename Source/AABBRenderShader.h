#pragma once

#include "GeoMetryInstanceShader.h"

class AABB;

struct cbAABB
{
	XMFLOAT3 m_fSize;
	float padding;
};

class CAABBRenderShader : public CGeoMetryInstanceShader
{
private:

	bool							 bInit;
	//VS
	ID3D11Buffer					*m_pd3dVSBuffer;
	cbAABB							 m_pd3dcbAABB;
	

public:

	CAABBRenderShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CAABBRenderShader(std::unique_ptr<CMesh> pMesh);

	CAABBRenderShader(int nObjects = 1);

	virtual ~CAABBRenderShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

	void UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext, XMFLOAT3 fSize);

	void SetRenderObjectContainer(std::vector<std::shared_ptr<CGameObject>> vObject){ m_vRenderObject = vObject; }

	void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, AABB aabb, std::vector<std::shared_ptr<CGameObject>> vObject);
	
	void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, BoundingBox boundingbox, std::vector<std::shared_ptr<CGameObject>> vObject);
	
	void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, BoundingBox boundingbox, std::shared_ptr<CGameObject> pObject);

	void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, std::vector<BoundingBox> vBoundingbox, std::shared_ptr<CGameObject> pObject);

	void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, std::vector<std::shared_ptr<CGameObject>> vObject);

};
