#pragma once

#include "GeoMetryMesh.h"

//--------------------------------------------------------------------------------------------------------------------------------------------
class CGeoCubeMesh : public CGeoMetryMesh
{
public:
	CGeoCubeMesh(ID3D11Device *pd3dDevice, float fWidth = 50.0f, float fHeight = 50.0f, float fDepth = 50.0f);

	virtual ~CGeoCubeMesh();
};