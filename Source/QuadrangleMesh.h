#pragma once

#include "Mesh.h"

class CQuadrangleMesh : public CMeshTexturedIlluminated
{

protected:
	XMFLOAT3						m_dot[3];

public:

	CQuadrangleMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fDepth = 2.0f);
	
	virtual ~CQuadrangleMesh();

	void GetDot(XMFLOAT3 &a, XMFLOAT3 &b, XMFLOAT3 &c);



};

