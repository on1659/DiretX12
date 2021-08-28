#include "stdafx.h"
#include "ModelParsingMesh.h"



CModelParsingMesh::CModelParsingMesh(ID3D11Device* pd3dDevice, float size) : CMeshTexturedIlluminated(pd3dDevice)
{

}
CModelParsingMesh::~CModelParsingMesh()
{

}

/////////////////////////////////////////////////�� �б�1
void CModelParsingMesh::ReadFile_Binaray(const wchar_t * _Filename, const wchar_t* mode){
	int nReadBytes;
	FILE *pFile = nullptr;

	::_wfopen_s(&pFile, _Filename, mode);
	
	//������ ����
	nReadBytes = ::fread(&m_nVertices, sizeof(int), 1, pFile);
	
	//������ ��ġ ����
	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	nReadBytes = ::fread(m_pd3dxvPositions, sizeof(XMFLOAT3), m_nVertices, pFile);

	//���� ����
	m_pd3dxvNormals = new XMFLOAT3[m_nVertices];//���� �� ��ŭ �븻���� �����Ҵ�
	nReadBytes = ::fread(m_pd3dxvNormals, sizeof(XMFLOAT3), m_nVertices, pFile);//������ ��, ����Ÿ Ÿ��, ����Ÿ ��, 


	//�ؽ��� ��ǥ
	m_pd3dxTexCoords = new XMFLOAT2[m_nVertices];
	nReadBytes = ::fread(m_pd3dxTexCoords, sizeof(XMFLOAT2), m_nVertices, pFile);
	//�ε��� ��
	nReadBytes = ::fread(&m_nIndices, sizeof(int), 1, pFile);
	//�ε��� ����
	m_pnIndices = new UINT[m_nIndices];
	nReadBytes = ::fread(m_pnIndices, sizeof(UINT), m_nIndices, pFile);
	//���� ������ �ݴ´�.
	::fclose(pFile);
}
/////////////////////////////////////////////////�� �б�1

/////////////////////////////////////////////////�� �б�2
void CModelParsingMesh::ReadFile_Text(char* _Filename){

	std::ifstream fin;

	// Open the model file.
	fin.open(_Filename);

	if (!fin)
	{
		MessageBox(0, L"_Filename", 0, 0);
		return;
	}

	std::string ignore;

	fin >> ignore >> m_nVertices;
	fin >> ignore >> m_nIndices;
	fin >> ignore >> ignore >> ignore >> ignore;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	m_pd3dxTexCoords = new XMFLOAT2[m_nVertices];
	m_pd3dxvNormals = new XMFLOAT3[m_nVertices];

	float nx, ny, nz;

	for (int i = 0; i < m_nVertices; ++i)
	{
		fin >> m_pd3dxvPositions[i].x >> m_pd3dxvPositions[i].y >> m_pd3dxvPositions[i].z;
		//fin >> m_pd3dxTexCoords[i].x >> m_pd3dxTexCoords[i].y;
		//fin >> m_pd3dxvNormals[i].x >> m_pd3dxvNormals[i].y >> m_pd3dxvNormals[i].z;
		fin >> nx >> ny >> nz;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	m_pnIndices = new UINT[m_nIndices * 3];

	for (UINT i = 0; i < m_nIndices; ++i)
	{
		fin >> m_pnIndices[i * 3 + 0] >> m_pnIndices[i * 3 + 1] >> m_pnIndices[i * 3 + 2];
	}

	//fin.get(input);
	//while (input != '{')
	//{
	//	fin.get(input);
	//}
	//fin.get(input);
	//fin.get(input);
	//
	//m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	//m_pd3dxTexCoords = new XMFLOAT2[m_nVertices];
	//m_pd3dxvNormals = new XMFLOAT3[m_nVertices];
	//// Read in the vertex data.
	//for (i = 0; i<m_nVertices; i++)
	//{
	//	fin >> m_pd3dxvPositions[i].x >> m_pd3dxvPositions[i].y >> m_pd3dxvPositions[i].z;
	//	//fin >> m_pd3dxTexCoords[i].x >> m_pd3dxTexCoords[i].y;
	//	fin >> m_pd3dxvNormals[i].x >> m_pd3dxvNormals[i].y >> m_pd3dxvNormals[i].z;
	//}
	//
	//fin.get(input);
	//while (input != '{')
	//{
	//	fin.get(input);
	//}
	//fin.get(input);
	//fin.get(input);
	//
	//m_pnIndices = new UINT[m_nIndices];
	////for (i = 2; i<m_nIndices ; i++)
	////{
	////	fin >> m_pnIndices[i-2] >> m_pnIndices[i-1] >> m_pnIndices[i];
	////}
	//
	//for (int i = 0; i<m_nIndices; i++)
	//	fin >> m_pnIndices[i];

	// Close the model file.
	fin.close();

}
/////////////////////////////////////////////////�� �б�2/////////////////////////////////////////////////�� �б�1


/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////�� �б�1
CBinaryFileMeshIlluminated::CBinaryFileMeshIlluminated(ID3D11Device* pd3dDevice, float size) : CModelParsingMesh(pd3dDevice)
{
	//���̳ʸ� ������
	CModelParsingMesh::ReadFile_Binaray(L"../Assets/02_ModelData/SciFi_Fighter_AK5.txt", L"rb, ccs=UNICODE");

	XMVECTOR *pd3dxvPositions = new XMVECTOR[m_nVertices];

	for (int i = 0; i < m_nVertices; i++)
	{
		pd3dxvPositions[i] =  XMVectorScale(XMLoadFloat3(&m_pd3dxvPositions[i]), size);
	}
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), (float)D3DXToRadian(-90.0f));


	for (int i = 0; i < m_nVertices; i++)
	{
		XMStoreFloat3(&m_pd3dxvPositions[i], XMVector3TransformCoord(pd3dxvPositions[i], mtxRotate));
	}

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//������ �� ���� ��Ҹ� ������ �� ��Ҹ��� �ϳ��� ���۸� �����Ѵ�.
	/*������ ù ��° ��Ҵ� ��ġ �����̰� ��ġ ���� �� ���� 12����Ʈ�̴�. ������ ù ��° ��Ҹ� ��Ÿ���� ���۸� �����Ѵ�.*/
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT3) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pd3dxvPositions;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dPositionBuffer);
	//��ġ ���� ����
	//�ٿ�� �ڽ� ���ϰ�
	float max_x = m_pd3dxvPositions[0].x, max_y = m_pd3dxvPositions[0].y, max_z = m_pd3dxvPositions[0].z;
	for (int i = 0; i < m_nVertices; i++){
		if (max_x < m_pd3dxvPositions[i].x) max_x = m_pd3dxvPositions[i].x;
		if (max_y < m_pd3dxvPositions[i].y) max_y = m_pd3dxvPositions[i].y;
		if (max_z < m_pd3dxvPositions[i].z) max_z = m_pd3dxvPositions[i].z;
	}
	float min_x = m_pd3dxvPositions[0].x, min_y = m_pd3dxvPositions[0].y, min_z = m_pd3dxvPositions[0].z;
	for (int i = 0; i < m_nVertices; i++){
		if (min_x > m_pd3dxvPositions[i].x) min_x = m_pd3dxvPositions[i].x;
		if (min_y > m_pd3dxvPositions[i].y) min_y = m_pd3dxvPositions[i].y;
		if (min_z > m_pd3dxvPositions[i].z) min_z = m_pd3dxvPositions[i].z;
	}
	//float min_x, min_y, min_z;

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT3) * m_nVertices;
	d3dBufferData.pSysMem = m_pd3dxvNormals;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dNormalBuffer);
	//�븻���� ���� ����

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT2) * m_nVertices;
	d3dBufferData.pSysMem = m_pd3dxTexCoords;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dTexCoordBuffer);
	//�ؽ��� ��ǥ ���� ����

	//������ ��ġ ����, ���� ����, �ؽ��� ��ǥ�� ���´�.
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-max_x, -max_y, -max_z);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+max_x, +max_y, +max_z);

	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { +max_x, +max_y, +max_z };
	m_fSize = XMFLOAT3(+max_x, +max_y, +max_z);
		
	delete pd3dxvPositions;

}

CBinaryFileMeshIlluminated::~CBinaryFileMeshIlluminated()
{
}

/////////////////////////////////////////////////�� �б�1

/////////////////////////////////////////////////�� �б�2
CTextFileMeshIlluminated::CTextFileMeshIlluminated(ID3D11Device* pd3dDevice, float size) : CModelParsingMesh(pd3dDevice)
{
	//�ؽ�Ʈ ������
	CModelParsingMesh::ReadFile_Text("../Assets/02_ModelData/skull.txt");

	XMVECTOR *pd3dxvPositions = new XMVECTOR[m_nVertices];

	for (int i = 0; i < m_nVertices; i++)
	{
		pd3dxvPositions[i] = XMVectorScale(XMLoadFloat3(&m_pd3dxvPositions[i]), size);

	}

	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), (float)D3DXToRadian(-90.0f));


	for (int i = 0; i < m_nVertices; i++)
	{
		XMStoreFloat3(&m_pd3dxvPositions[i], XMVector3TransformCoord(pd3dxvPositions[i], mtxRotate));
	}

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//������ �� ���� ��Ҹ� ������ �� ��Ҹ��� �ϳ��� ���۸� �����Ѵ�.
	/*������ ù ��° ��Ҵ� ��ġ �����̰� ��ġ ���� �� ���� 12����Ʈ�̴�. ������ ù ��° ��Ҹ� ��Ÿ���� ���۸� �����Ѵ�.*/
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT3) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pd3dxvPositions;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dPositionBuffer);
	//��ġ ���� ����
	//�ٿ�� �ڽ� ���ϰ�
	float max_x = m_pd3dxvPositions[0].x, max_y = m_pd3dxvPositions[0].y, max_z = m_pd3dxvPositions[0].z;
	for (int i = 0; i < m_nVertices; i++){
		if (max_x < m_pd3dxvPositions[i].x) max_x = m_pd3dxvPositions[i].x;
		if (max_y < m_pd3dxvPositions[i].y) max_y = m_pd3dxvPositions[i].y;
		if (max_z < m_pd3dxvPositions[i].z) max_z = m_pd3dxvPositions[i].z;
	}
	float min_x = m_pd3dxvPositions[0].x, min_y = m_pd3dxvPositions[0].y, min_z = m_pd3dxvPositions[0].z;
	for (int i = 0; i < m_nVertices; i++){
		if (min_x > m_pd3dxvPositions[i].x) min_x = m_pd3dxvPositions[i].x;
		if (min_y > m_pd3dxvPositions[i].y) min_y = m_pd3dxvPositions[i].y;
		if (min_z > m_pd3dxvPositions[i].z) min_z = m_pd3dxvPositions[i].z;
	}
	//float min_x, min_y, min_z;

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT3) * m_nVertices;
	d3dBufferData.pSysMem = m_pd3dxvNormals;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dNormalBuffer);
	//�븻���� ���� ����

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT2) * m_nVertices;
	d3dBufferData.pSysMem = m_pd3dxTexCoords;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dTexCoordBuffer);
	//�ؽ��� ��ǥ ���� ����

	//������ ��ġ ����, ���� ����, �ؽ��� ��ǥ�� ���´�.
	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);


	#ifdef _AABB_
		m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-max_x, -max_y, -max_z);
		m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+max_x, +max_y, +max_z);
	#else
		m_bcBoundingCube.Center = { 0.f, 0.f, 0.f };
		m_bcBoundingCube.Extents = { +max_x, +max_y, +max_z };
		m_fSize = XMFLOAT3(+max_x, +max_y, +max_z);
	#endif

	delete pd3dxvPositions;
}

CTextFileMeshIlluminated::~CTextFileMeshIlluminated()
{
}
/////////////////////////////////////////////////�� �б�2
