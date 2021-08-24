#include "stdafx.h"
#include "WARP3DMesh.h"
#include "Warp3DModelManager.h"
 

CWARP3DMesh::CWARP3DMesh(ID3D11Device *pd3dDevice, std::string path)
	: CMeshTexturedIlluminated(pd3dDevice)
{
	name = path;



	#ifndef _USE_TAN	

		if (!MODELMGR->SearchModelData(path)) return;

		CWarp3DModel m_w3dModel = MODELMGR->GetModelData(path);

		if (0 == m_w3dModel.GetModelCount())
		{
			GMessageBox("Warp3D Import Error", "No Model Data!");
		}

		ModelInfo info = m_w3dModel.GetModelInfo();
		m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		m_nVertices = info.m_nIndices;

		m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

		XMFLOAT3* pd3dxvNormals;
		pd3dxvNormals = new XMFLOAT3[m_nVertices];

		//XMFLOAT2* pd3dxvTexCoords;
		m_pTexCoords = new XMFLOAT2[m_nVertices];

		std::vector<UINT> m_vIndices = info.m_lFaceIndexList;

		for (int i = 0; i < info.m_nIndices; ++i)
		{
			int index = info.GetFaceIndex(i);

			m_pd3dxvPositions[i] = info.GetPosition(index);

			pd3dxvNormals[i] = info.GetNormal(index);

			m_pTexCoords[i] = info.GetUV(i);
		}

		m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		if (m_pd3dxvPositions) delete[] m_pd3dxvPositions;
		//if(pd3dxvNormals) delete[] pd3dxvNormals;
		if (m_pTexCoords) delete[] m_pTexCoords;

		ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
		UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		UINT pnBufferOffsets[3] = { 0, 0, 0 };
		AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

		float fx = (info.m_fMaxX - info.m_fMinX) * 0.5f;
		float fy = (info.m_fMaxY - info.m_fMinY);
		float fz = (info.m_fMaxZ - info.m_fMinZ) * 0.5f;

		m_bcBoundingbox.Center = { 0,  0,  0 };
		m_bcBoundingbox.Extents = { fx, fy, fz };
		m_fSize = XMFLOAT3(fx, fy, fz);
	#else
		if (!MODELMGR->SearchModelData(path)) return;

		CWarp3DModel m_w3dModel = MODELMGR->GetModelData(path);

		if (0 == m_w3dModel.GetModelCount())
		{
			GMessageBox("Warp3D Import Error", "No Model Data!");
		}

		ModelInfo info = m_w3dModel.GetModelInfo();
		m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		m_nVertices = info.m_nIndices;

		m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

		XMFLOAT3* pd3dxvNormals;
		pd3dxvNormals = new XMFLOAT3[m_nVertices];

		m_pTexCoords = new XMFLOAT2[m_nVertices];

		std::vector<UINT> m_vIndices = info.m_lFaceIndexList;

		for (int i = 0; i < info.m_nIndices; ++i)
		{
			int index = info.GetFaceIndex(i);

			m_pd3dxvPositions[i] = info.GetPosition(index);

			pd3dxvNormals[i] = info.GetNormal(index);

			m_pTexCoords[i] = info.GetUV(i);
		}

		XMVECTOR *pTangets = new XMVECTOR[m_nVertices];
		m_pTangents = new XMFLOAT3[m_nVertices];
		CalculateVertexTangent(pTangets);

		for (auto i = 0; i < m_nVertices; ++i)
		{
			XMStoreFloat3(&m_pTangents[i], pTangets[i]);
		}

		m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTangentBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		if (m_pd3dxvPositions) delete[] m_pd3dxvPositions;
		if (pd3dxvNormals) delete[] pd3dxvNormals;
		if (m_pTexCoords) delete[] m_pTexCoords;
		if (m_pTangents) delete[] m_pTangents;

		ID3D11Buffer *pd3dBuffers[4] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dTangentBuffer };
		UINT	  pnBufferStrides[4] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT3) };
		UINT	  pnBufferOffsets[4] = { 0, 0, 0 };
		AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

		float fx = (info.m_fMaxX - info.m_fMinX) * 0.5f;
		float fy = (info.m_fMaxY - info.m_fMinY);
		float fz = (info.m_fMaxZ - info.m_fMinZ) * 0.5f;

		m_bcBoundingbox.Center = { 0,  0,  0 };
		m_bcBoundingbox.Extents = { fx, fy, fz };
		m_fSize = XMFLOAT3(fx, fy, fz);
	#endif

}

CWARP3DMesh::~CWARP3DMesh()
{

}




/*
if (!MODELMGR->SearchModelData(path)) return;

CWarp3DModel m_w3dModel = MODELMGR->GetModelData(path);

if (0 == m_w3dModel.GetModelCount())
{
GMessageBox("Warp3D Import Error", "No Model Data!");
}

ModelInfo info = m_w3dModel.GetModelInfo();
m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

m_nVertices = info.m_nIndices;

m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

XMFLOAT3* pd3dxvNormals;
pd3dxvNormals = new XMFLOAT3[m_nVertices];

XMFLOAT2* pd3dxvTexCoords;
pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

std::vector<UINT> m_vIndices = info.m_lFaceIndexList;

for (int i = 0; i < info.m_nIndices; ++i)
{
int index = info.GetFaceIndex(i);

m_pd3dxvPositions[i] = info.GetPosition(index);

pd3dxvNormals[i] = info.GetNormal(index);

pd3dxvTexCoords[i] = info.GetUV(i);
}

XMVECTOR *pTangets = new XMVECTOR[m_nVertices];
m_pTangents = new XMFLOAT3[m_nVertices];
CalculateVertexTangent(pTangets);

for (auto i = 0; i < m_nVertices; ++i)
{
XMStoreFloat3(&m_pTangents[i], pTangets[i]);
}

m_pd3dPositionBuffer	= CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
m_pd3dNormalBuffer		= CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
m_pd3dTexCoordBuffer	= CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
m_pd3dTangentBuffer		= CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

if (m_pd3dxvPositions) delete[] m_pd3dxvPositions;
//if(pd3dxvNormals) delete[] pd3dxvNormals;
if (pd3dxvTexCoords) delete[] pd3dxvTexCoords;

ID3D11Buffer *pd3dBuffers[4] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dTangentBuffer };
UINT	  pnBufferStrides[4] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT3) };
UINT	  pnBufferOffsets[4] = { 0, 0, 0 };
AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

float fx = (info.m_fMaxX - info.m_fMinX) * 0.5f;
float fy = (info.m_fMaxY - info.m_fMinY);
float fz = (info.m_fMaxZ - info.m_fMinZ) * 0.5f;

m_bcBoundingbox.Center = { 0,  0,  0 };
m_bcBoundingbox.Extents = { fx, fy, fz };
m_fSize = XMFLOAT3(fx, fy, fz);
*/