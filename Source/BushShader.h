#pragma once
#include "InstancedObjectsShader.h"

class CBushShader : public CInstancedObjectsShader
{
public:

	CBushShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CBushShader(std::unique_ptr<CMesh> pMesh);

	CBushShader(int nObjects = 1);

	virtual ~CBushShader();


	//KYT '16.05.12
	/*
	  ��� ��¿�� ���� Foward Render�� �ؾ� �Ѵ�.
	*/
	void CreateShader(ID3D11Device* pd3dDevice);


	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera);

};

