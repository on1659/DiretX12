#pragma once
#include "GeoMetryMesh.h"

class CGeoAABBMesh : public CGeoMetryMesh
{
public:
	CGeoAABBMesh(ID3D11Device *pd3dDevice);
	virtual ~CGeoAABBMesh();
};

