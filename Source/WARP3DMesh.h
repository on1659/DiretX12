#pragma once

class Mesh;
class CWarp3DModel;

class CWARP3DMesh : public CMeshTexturedIlluminated
{
public:

	CWARP3DMesh(ID3D11Device *pd3dDevice, std::string path);
	
	~CWARP3DMesh();
	
	
};

