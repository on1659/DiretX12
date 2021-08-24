#include "stdafx.h"
#include "TerrainCubeMesh.h"
#include "Mesh.h"

//------------------------------------------------------------------------------------------------
CTerrainCubeMesh::CTerrainCubeMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshTexturedIlluminated(pd3dDevice)
{
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth, fy = fHeight, fz = fDepth;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	int i = 0;

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, 0.0f);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, 0.0f);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, 0.0f, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, 0.0f, 0.0f);

	XMFLOAT2 pd3dxvTexCoords[36];
	i = 0;
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	XMFLOAT3 pd3dxvNormals[36];
	XMVECTOR pxm[36];
	//	for (int i = 0; i < 36; i++) pd3dxvNormals[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 36; i++) pxm[i] = XMLoadFloat3(&pd3dxvNormals[i]);
	CalculateVertexNormal(pxm);
	for (int i = 0; i < 36; i++) XMStoreFloat3(&pd3dxvNormals[i], pxm[i]);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);


	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(0, 0, 0);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(fx, fy, fz);

	m_bcBoundingbox.Center = { 0.0f, 0.0f, 0.0f };
	m_bcBoundingbox.Extents = { fx * 0.5f, fy * 0.5f, fz  * 0.5f };

	m_fSize = XMFLOAT3(fx * 0.5f, fy * 0.5f, fz  * 0.5f);

}

CTerrainCubeMesh::~CTerrainCubeMesh()
{
}
