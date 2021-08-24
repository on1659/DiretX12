#include "stdafx.h"
#include "GeoAABBMesh.h"

CGeoAABBMesh::CGeoAABBMesh(ID3D11Device *pd3dDevice) : CGeoMetryMesh(pd3dDevice)
{
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	
	XMFLOAT3 *pd3dxvMinSize = new XMFLOAT3[m_nVertices];//�� �Ѱ� ����
	XMFLOAT3 *pd3dxvMaxSize = new XMFLOAT3[m_nVertices];//�� �Ѱ� ����
	pd3dxvMinSize[0] = XMFLOAT3(0.0f,0.0f,0.0f);
	pd3dxvMaxSize[0] = XMFLOAT3(0.0f,0.0f,0.0f);

	//GS�� ���ϱ� ���� size������ ������ �����̴�. ���� 1��!$
	ID3D11Buffer *pd3dMinSizeBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvMinSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	ID3D11Buffer *pd3dMaxSizeBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvMaxSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//GS�� ���ϱ� ���� color������ ������ �����̴�. ���� 1��!
	//���� ������ �����ϰ� ���ص� �ȴ�.

	//size, color������ ���� set
	ID3D11Buffer *ppd3dBuffers[2] = { pd3dMinSizeBuffer, pd3dMaxSizeBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, ppd3dBuffers, pnBufferStrides, pnBufferOffsets);
}

CGeoAABBMesh::~CGeoAABBMesh()
{
}