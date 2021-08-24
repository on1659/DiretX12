#pragma once

//XMMath �Լ� ���� X

class CModelParsingMesh : public CMeshTexturedIlluminated
{
public:
	CModelParsingMesh(ID3D11Device *pd3dDevice, float size = 50.0f);
	virtual ~CModelParsingMesh(); 

public:
	/////////////////////////////////////////////////�� �б�1
	XMFLOAT3 *m_pd3dxvNormals;
	XMFLOAT2 *m_pd3dxTexCoords;

	/////////////////////////////////////////////////�� �б�1
	//////////////////////////////////////////////////�� �б�1
	void ReadFile_Binaray(const wchar_t * _Filename, const wchar_t* mode);

	void ReadFile_Text(char * _Filename);

	/////////////////////////////////////////////////�� �б�1
};

/////////////////////////////////////////////////�� �б�1
class CBinaryFileMeshIlluminated : public CModelParsingMesh
{
public:
	CBinaryFileMeshIlluminated(ID3D11Device *pd3dDevice, float size = 50.0f);
	virtual ~CBinaryFileMeshIlluminated();
};
/////////////////////////////////////////////////�� �б�1

/////////////////////////////////////////////////�� �б�2
class CTextFileMeshIlluminated : public CModelParsingMesh
{
public:
	CTextFileMeshIlluminated(ID3D11Device *pd3dDevice, float size = 50.0f);
	virtual ~CTextFileMeshIlluminated();
};
/////////////////////////////////////////////////�� �б�2
