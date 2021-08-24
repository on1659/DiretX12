#pragma once
#include "WARP3DMesh.h"

class CWarp3D_PlayerMesh : public CMeshTexturedIlluminated
{
public:
	CWarp3D_PlayerMesh(ID3D11Device *pd3dDevice, std::string path);
	~CWarp3D_PlayerMesh();
};

