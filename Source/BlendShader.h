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
	얘는 어쩔수 없이 Foward Render을 해야 한다.
	*/
	void CreateShader(ID3D11Device *pd3dDevice);

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

};

