#pragma once

class Mesh;

//--------------------------------------------------------------------------------------------------------------------------------------------
class CGeoMetryMesh : public CMeshTexturedIlluminated
{
protected:
	D3DXVECTOR3						*m_pd3dxvSize;
	ID3D11Buffer					*m_pd3dSizeBuffer;

public:
	CGeoMetryMesh(ID3D11Device *pd3dDevice);

	virtual ~CGeoMetryMesh();
};