#pragma once

//KYT '16.01.22 plus 
/*
GeomMetoryShader ¡¶¿€
*/
// ex) Player

#include "InstancedObjectsShader.h"

//----------------------------------------------------------------
class CChochShader : public CInstancedObjectsShader
{

public:

	CChochShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CChochShader(std::unique_ptr<CMesh> pMesh);

	CChochShader(int nObjects = 1);

	virtual ~CChochShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

};
