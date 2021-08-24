#pragma once
#include "Mesh.h"

class CLineMesh : public CMeshDiffused
{
public:
	CLineMesh(ID3D11Device *pd3dDevice, XMFLOAT3 start, XMFLOAT3 end, XMVECTOR color);
	virtual ~CLineMesh();
};

