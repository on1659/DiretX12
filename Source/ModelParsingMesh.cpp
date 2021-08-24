#include "stdafx.h"
#include "ModelParsingMesh.h"



CModelParsingMesh::CModelParsingMesh(ID3D11Device *pd3dDevice, float size) : CMeshTexturedIlluminated(pd3dDevice)
{

}
CModelParsingMesh::~CModelParsingMesh()
{

}

/////////////////////////////////////////////////모델 읽기1
void CModelParsingMesh::ReadFile_Binaray(const wchar_t * _Filename, const wchar_t* mode){
	int nReadBytes;
	FILE *pFile = nullptr;

	::_wfopen_s(&pFile, _Filename, mode);
	
	//정점의 개수
	nReadBytes = ::fread(&m_nVertices, sizeof(int), 1, pFile);
	
	//정점의 위치 벡터
	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	nReadBytes = ::fread(m_pd3dxvPositions, sizeof(XMFLOAT3), m_nVertices, pFile);

	//법선 벡터
	m_pd3dxvNormals = new XMFLOAT3[m_nVertices];//정점 수 만큼 노말벡터 동적할당
	nReadBytes = ::fread(m_pd3dxvNormals, sizeof(XMFLOAT3), m_nVertices, pFile);//저장할 곳, 데이타 타입, 데이타 수, 


	//텍스쳐 좌표
	m_pd3dxTexCoords = new XMFLOAT2[m_nVertices];
	nReadBytes = ::fread(m_pd3dxTexCoords, sizeof(XMFLOAT2), m_nVertices, pFile);
	//인덱스 수
	nReadBytes = ::fread(&m_nIndices, sizeof(int), 1, pFile);
	//인덱스 순서
	m_pnIndices = new UINT[m_nIndices];
	nReadBytes = ::fread(m_pnIndices, sizeof(UINT), m_nIndices, pFile);
	//이진 파일을 닫는다.
	::fclose(pFile);
}
/////////////////////////////////////////////////모델 읽기1

/////////////////////////////////////////////////모델 읽기2
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
/////////////////////////////////////////////////모델 읽기2/////////////////////////////////////////////////모델 읽기1


/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////모델 읽기1
CBinaryFileMeshIlluminated::CBinaryFileMeshIlluminated(ID3D11Device *pd3dDevice, float size) : CModelParsingMesh(pd3dDevice)
{
	//바이너리 데이터
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

	//정점은 두 개의 요소를 가지고 각 요소마다 하나의 버퍼를 생성한다.
	/*정점의 첫 번째 요소는 위치 벡터이고 위치 벡터 한 개는 12바이트이다. 정점의 첫 번째 요소를 나타내는 버퍼를 생성한다.*/
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
	//위치 버퍼 생성
	//바운딩 박스 구하고
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
	//노말벡터 버퍼 생성

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT2) * m_nVertices;
	d3dBufferData.pSysMem = m_pd3dxTexCoords;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dTexCoordBuffer);
	//텍스쳐 좌표 버퍼 생성

	//정점은 위치 벡터, 법선 벡터, 텍스쳐 좌표를 갖는다.
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

/////////////////////////////////////////////////모델 읽기1

/////////////////////////////////////////////////모델 읽기2
CTextFileMeshIlluminated::CTextFileMeshIlluminated(ID3D11Device *pd3dDevice, float size) : CModelParsingMesh(pd3dDevice)
{
	//텍스트 데이터
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

	//정점은 두 개의 요소를 가지고 각 요소마다 하나의 버퍼를 생성한다.
	/*정점의 첫 번째 요소는 위치 벡터이고 위치 벡터 한 개는 12바이트이다. 정점의 첫 번째 요소를 나타내는 버퍼를 생성한다.*/
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
	//위치 버퍼 생성
	//바운딩 박스 구하고
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
	//노말벡터 버퍼 생성

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT2) * m_nVertices;
	d3dBufferData.pSysMem = m_pd3dxTexCoords;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dTexCoordBuffer);
	//텍스쳐 좌표 버퍼 생성

	//정점은 위치 벡터, 법선 벡터, 텍스쳐 좌표를 갖는다.
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
/////////////////////////////////////////////////모델 읽기2
