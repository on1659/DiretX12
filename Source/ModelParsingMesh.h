#pragma once

//XMMath ÇÔ¼ö Àû¿ë X

class CModelParsingMesh : public CMeshTexturedIlluminated
{
public:
	CModelParsingMesh(ID3D11Device *pd3dDevice, float size = 50.0f);
	virtual ~CModelParsingMesh(); 

public:
	/////////////////////////////////////////////////¸ðµ¨ ÀÐ±â1
	XMFLOAT3 *m_pd3dxvNormals;
	XMFLOAT2 *m_pd3dxTexCoords;

	/////////////////////////////////////////////////¸ðµ¨ ÀÐ±â1
	//////////////////////////////////////////////////¸ðµ¨ ÀÐ±â1
	void ReadFile_Binaray(const wchar_t * _Filename, const wchar_t* mode);

	void ReadFile_Text(char * _Filename);

	/////////////////////////////////////////////////¸ðµ¨ ÀÐ±â1
};

/////////////////////////////////////////////////¸ðµ¨ ÀÐ±â1
class CBinaryFileMeshIlluminated : public CModelParsingMesh
{
public:
	CBinaryFileMeshIlluminated(ID3D11Device *pd3dDevice, float size = 50.0f);
	virtual ~CBinaryFileMeshIlluminated();
};
/////////////////////////////////////////////////¸ðµ¨ ÀÐ±â1

/////////////////////////////////////////////////¸ðµ¨ ÀÐ±â2
class CTextFileMeshIlluminated : public CModelParsingMesh
{
public:
	CTextFileMeshIlluminated(ID3D11Device *pd3dDevice, float size = 50.0f);
	virtual ~CTextFileMeshIlluminated();
};
/////////////////////////////////////////////////¸ðµ¨ ÀÐ±â2
