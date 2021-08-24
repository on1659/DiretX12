#pragma once

class CUIMesh : public CMeshTexturedIlluminated
{
public:

	CUIMesh(ID3D11Device *pd3dDevice, float fWidth = 30.0f, float fheight = 30.0f);
	
	virtual ~CUIMesh();

};
