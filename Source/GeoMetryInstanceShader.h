#pragma once

//KYT '16.01.22 plus 
/*
	GeomMetoryShader ¡¶¿€
*/
// ex) Player

#include "InstancedObjectsShader.h"

//----------------------------------------------------------------
class CGeoMetryInstanceShader : public CInstancedObjectsShader
{

public:

	CGeoMetryInstanceShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CGeoMetryInstanceShader(std::unique_ptr<CMesh> pMesh);

	CGeoMetryInstanceShader(int nObjects = 1);

	virtual ~CGeoMetryInstanceShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

};
