#include "stdafx.h"
#include "GeoSphereMesh.h"

CGeoSphereMesh::CGeoSphereMesh(ID3D11Device *pd3dDevice, float fRadius, int nSlices, int nStacks) : CGeoMetryMesh(pd3dDevice)
{
	m_nVertices = nStacks;//정점의 개수는 스텍이다. 
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	m_pd3dxvSize = new D3DXVECTOR3[m_nVertices];
	for (int i = 0; i < m_nVertices; i++) {
		m_pd3dxvSize[i] = D3DXVECTOR3(fRadius, nSlices, nStacks);
	}

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);

	}

	m_pd3dSizeBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR3), m_nVertices, m_pd3dxvSize, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[1] = { m_pd3dSizeBuffer };
	UINT pnBufferStrides[1] = { sizeof(D3DXVECTOR3) };
	UINT pnBufferOffsets[1] = { 0 };
	AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fRadius, -fRadius, -fRadius);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fRadius, +fRadius, +fRadius);

	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fRadius, fRadius, fRadius };
	m_fSize = XMFLOAT3(fRadius, fRadius, fRadius);
}

CGeoSphereMesh::~CGeoSphereMesh()
{
}