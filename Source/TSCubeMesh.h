#pragma once

#include "Mesh.h"

class CTSCubeMesh : public CMeshTextured
{
public:
	CTSCubeMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fDepth = 2.0f, float fLength = 2.0f);

	virtual ~CTSCubeMesh();

};

