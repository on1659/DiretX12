#include "stdafx.h"
#include "GeoAABBMesh.h"

CGeoAABBMesh::CGeoAABBMesh(ID3D11Device *pd3dDevice) : CGeoMetryMesh(pd3dDevice)
{
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	
	XMFLOAT3 *pd3dxvMinSize = new XMFLOAT3[m_nVertices];//점 한개 제작
	XMFLOAT3 *pd3dxvMaxSize = new XMFLOAT3[m_nVertices];//점 한개 제작
	pd3dxvMinSize[0] = XMFLOAT3(0.0f,0.0f,0.0f);
	pd3dxvMaxSize[0] = XMFLOAT3(0.0f,0.0f,0.0f);

	//GS에 쎗하기 위한 size정보를 가지는 버퍼이다. 정점 1개!$
	ID3D11Buffer *pd3dMinSizeBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvMinSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	ID3D11Buffer *pd3dMaxSizeBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvMaxSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//GS에 쎗하기 위한 color정보를 가지는 버퍼이다. 정점 1개!
	//색은 재질이 있으니가 안해도 된다.

	//size, color순으로 버퍼 set
	ID3D11Buffer *ppd3dBuffers[2] = { pd3dMinSizeBuffer, pd3dMaxSizeBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, ppd3dBuffers, pnBufferStrides, pnBufferOffsets);
}

CGeoAABBMesh::~CGeoAABBMesh()
{
}