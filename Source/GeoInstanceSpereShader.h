#pragma once

#include "GeoMetryInstanceShader.h"

//KYT '16.02.04 plus 
/*
GeomMetoryShader ¡¶¿€
*/
// ex) Player


class CGeoInstanceSphereShader : public CGeoMetryInstanceShader
{
public:

	CGeoInstanceSphereShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CGeoInstanceSphereShader(std::unique_ptr<CMesh> pMesh);

	CGeoInstanceSphereShader(int nObjects = 1);

	virtual ~CGeoInstanceSphereShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};
