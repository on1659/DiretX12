#include "stdafx.h"
#include "QuadrangleSpriteMesh.h"

CQuadrangleSpriteMesh::CQuadrangleSpriteMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight) : CQuadrangleMesh(pd3dDevice)
{
	m_nVertices = 6;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = 0.0f;

	m_fSize = XMFLOAT3(fx, fy, fz);

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	int i = 0;

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz); m_dot[0] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz); m_dot[1] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz); m_dot[2] = XMFLOAT3(+fx, -fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);


	XMFLOAT2 pd3dxvTexCoords[6];
	i = 0;
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };

	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

}

CQuadrangleSpriteMesh::~CQuadrangleSpriteMesh()
{
}
