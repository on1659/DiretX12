#include "stdafx.h"
#include "GeoCubeMesh.h"

CGeoCubeMesh::CGeoCubeMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth) : CGeoMetryMesh(pd3dDevice)
{
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	XMFLOAT3 *pd3dxvSize = new XMFLOAT3[m_nVertices];//�� �Ѱ� ����
	int i = 0;
	pd3dxvSize[i] = XMFLOAT3(fWidth, fHeight, fDepth);

	//GS�� ���ϱ� ���� size������ ������ �����̴�. ���� 1��!$
	ID3D11Buffer *pd3dSizeBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR3), m_nVertices, pd3dxvSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//GS�� ���ϱ� ���� color������ ������ �����̴�. ���� 1��!
	//���� ������ �����ϰ� ���ص� �ȴ�.

	//size, color������ ���� set
	ID3D11Buffer *ppd3dBuffers[1] = { pd3dSizeBuffer };
	UINT pnBufferStrides[1] = { sizeof(D3DXVECTOR3) };
	UINT pnBufferOffsets[1] = { 0 };
	AssembleToVertexBuffer(1, ppd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);
	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fx, fy, fz };
	m_fSize = XMFLOAT3(fx, fy, fz);
}

CGeoCubeMesh::~CGeoCubeMesh()
{
}
