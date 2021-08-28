#pragma once

#include "InstancedObjectsShader.h"

class CBlendShader : public CInstancedObjectsShader
{
public:

	CBlendShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CBlendShader(std::unique_ptr<CMesh> pMesh);

	CBlendShader(int nObjects = 1);

	virtual ~CBlendShader();


	//KYT '16.05.12
	/*
	��� ��¿�� ���� Foward Render�� �ؾ� �Ѵ�.
	*/
	void CreateShader(ID3D11Device* pd3dDevice);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

};

