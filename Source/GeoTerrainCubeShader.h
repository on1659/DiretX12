#pragma once

#include "GeoMetryInstanceShader.h"

class CGeoTerrainCubeShader : public CGeoMetryInstanceShader
{
public:

	CGeoTerrainCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects);

	CGeoTerrainCubeShader(std::unique_ptr<CMesh> pMesh);

	CGeoTerrainCubeShader(int nObjects = 1);

	virtual ~CGeoTerrainCubeShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};
