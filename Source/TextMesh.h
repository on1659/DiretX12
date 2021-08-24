#pragma once


class CTextMesh : public CMeshTexturedIlluminated
{
public:

	CTextMesh(ID3D11Device *pd3dDevice, float fWidth = 30.0f, float fheight = 30.0f);

	virtual ~CTextMesh();

};
