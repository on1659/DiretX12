#pragma once

#include "GeoMetryMesh.h"

class CGeoSphereMesh : public CGeoMetryMesh
{
public:
	CGeoSphereMesh(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CGeoSphereMesh();
};

