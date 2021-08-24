#pragma once

class CMeshTexturedIlluminated;

class CTerrainCubeMesh : public CMeshTexturedIlluminated
{
public:
	CTerrainCubeMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CTerrainCubeMesh();
};
