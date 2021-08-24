#include "stdafx.h"
#include "TSCubeMesh.h"

CTSCubeMesh::CTSCubeMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fLength) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 24;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

	XMFLOAT3 *x3_Size = new  XMFLOAT3[m_nVertices];
	//m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

	int i = 0;
	float fx = fWidth * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = fLength * 0.5f;

	x3_Size[i++] = XMFLOAT3(-fx, +fy, -fz);
	x3_Size[i++] = XMFLOAT3(+fx, +fy, -fz);
	x3_Size[i++] = XMFLOAT3(-fx, -fy, -fz);
	x3_Size[i++] = XMFLOAT3(+fx, -fy, -fz);

	x3_Size[i++] = XMFLOAT3(-fx, -fy, -fz);
	x3_Size[i++] = XMFLOAT3(+fx, -fy, -fz);
	x3_Size[i++] = XMFLOAT3(-fx, -fy, +fz);
	x3_Size[i++] = XMFLOAT3(+fx, -fy, +fz);

	x3_Size[i++] = XMFLOAT3(-fx, -fy, +fz);
	x3_Size[i++] = XMFLOAT3(+fx, -fy, +fz);
	x3_Size[i++] =  XMFLOAT3(-fx, +fy, +fz);
	x3_Size[i++] = XMFLOAT3(+fx, +fy, +fz);

	x3_Size[i++] = XMFLOAT3(-fx, +fy, +fz);
	x3_Size[i++] = XMFLOAT3(+fx, +fy, +fz);
	x3_Size[i++] = XMFLOAT3(-fx, +fy, -fz);
	x3_Size[i++] = XMFLOAT3(+fx, +fy, -fz);

	x3_Size[i++] = XMFLOAT3(+fx, +fy, -fz);
	x3_Size[i++] = XMFLOAT3(+fx, +fy, +fz);
	x3_Size[i++] = XMFLOAT3(+fx, -fy, -fz);
	x3_Size[i++] = XMFLOAT3(+fx, -fy, +fz);

	x3_Size[i++] = XMFLOAT3(-fx, +fy, +fz);
	x3_Size[i++] = XMFLOAT3(-fx, +fy, -fz);
	x3_Size[i++] = XMFLOAT3(-fx, -fy, +fz);
	x3_Size[i++] = XMFLOAT3(-fx, -fy, -fz);

	float *m_pd3dxvSize = new float[1];
	m_pd3dxvSize[0] = fy * 0.5f;

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, x3_Size, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
//	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(float), 1, m_pd3dxvSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	if (x3_Size)delete[] x3_Size;
	x3_Size = nullptr;

	ID3D11Buffer *pd3dBuffers[1] = { m_pd3dPositionBuffer };
	UINT pnBufferStrides[1] = { sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[1] = { 0 };
	AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);



	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);
	m_fSize = XMFLOAT3(fx, fy, fz);

	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = m_bcBoundingCube.m_d3dxvMaximum;
	m_fSize = XMFLOAT3(fx, fy, fz);

}

CTSCubeMesh::~CTSCubeMesh()
{
}


