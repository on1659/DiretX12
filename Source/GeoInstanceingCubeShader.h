#pragma once

#include "GeoMetryInstanceShader.h"

//KYT '16.01.22 plus 
/*
	GeomMetoryShader ¡¶¿€
*/
// ex) Player


class CGeoInstanceingCubeShader : public CGeoMetryInstanceShader
{
public:

	CGeoInstanceingCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CGeoInstanceingCubeShader(std::unique_ptr<CMesh> pMesh);

	CGeoInstanceingCubeShader(int nObjects = 1);

	virtual ~CGeoInstanceingCubeShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};
