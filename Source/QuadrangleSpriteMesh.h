#pragma once

#include "QuadrangleMesh.h"

class CQuadrangleSpriteMesh : public CQuadrangleMesh
{

public:

	CQuadrangleSpriteMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fDepth = 2.0f);

	virtual ~CQuadrangleSpriteMesh();

};

