#pragma once
#include "Object.h"
#include "LineMesh.h"

class CAxisObjects : public CGameObject
{
	CGameObject*	m_pOwnerObject			= nullptr;
	CGameObject*	m_pWorldAxisObject		= nullptr;

public:
	CAxisObjects(CGameObject* pOwner = nullptr);

	virtual ~CAxisObjects();

	void CreateAxis(ID3D11Device *pd3dDevice);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera) override;
};