#pragma once

#include "stdafx.h"
#include "InstancedObjectsShader.h"

class CWaterFallMeshShader : public CInstancedObjectsShader
{
public:

	CWaterFallMeshShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CWaterFallMeshShader(std::unique_ptr<CMesh> pMesh);

	CWaterFallMeshShader(int nObjects = 1);

	virtual ~CWaterFallMeshShader();


	//KYT '16.05.12
	/*
	��� ��¿�� ���� Foward Render�� �ؾ� �Ѵ�.
	*/
	virtual void CreateShader(ID3D11Device *pd3dDevice);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);
};

