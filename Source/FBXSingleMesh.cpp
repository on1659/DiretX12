#include "stdafx.h"
#include "FBXSingleMesh.h"
#include "Mesh.h"
#include "FBXLoader.h"
 
CFBXSingleMeshTexturedIlluminated::CFBXSingleMeshTexturedIlluminated(ID3D11Device *pd3dDevice, std::string path, float modelscale, bool isVoxel)
	: CMeshTexturedIlluminated(pd3dDevice)
{

#ifndef _USE_TAN
	m_pfbxModel = new FBXModelLoader(path, modelscale, isVoxel);
	m_pfbxModel->ProcessMeshInfo();

	m_nVertices = m_pfbxModel->GetIndiecesCount();
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

	XMFLOAT2* pd3dxvTexCoords;
	pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	XMFLOAT3* pd3dxvNormals;
	pd3dxvNormals = new XMFLOAT3[m_nVertices];

	int *m_fbxIndices = m_pfbxModel->GetIndices();

	int j = 0;
	int n = m_pfbxModel->GetPolygonCount();

	for (int i = 0; i < n; i++)
	{
		//정점은 인덱스 순서대로 정보를 넣습니다. UV랑 노멀은 그냥 순서대로!
		if (m_pfbxModel->IsVoxel())
		{
			m_pd3dxvPositions[j] = m_pfbxModel->GetVertices(m_fbxIndices[j + 1]);
			m_pd3dxvPositions[j + 1] = m_pfbxModel->GetVertices(m_fbxIndices[j]);
			m_pd3dxvPositions[j + 2] = m_pfbxModel->GetVertices(m_fbxIndices[j + 2]);

			XMFLOAT2 tempUV0 = m_pfbxModel->GetTextureCoords(j);
			XMFLOAT2 tempUV1 = m_pfbxModel->GetTextureCoords(j + 1);
			XMFLOAT2 tempUV2 = m_pfbxModel->GetTextureCoords(j + 2);

			pd3dxvTexCoords[j] = tempUV1;
			pd3dxvTexCoords[j + 1] = tempUV0;
			pd3dxvTexCoords[j + 2] = tempUV2;

			pd3dxvNormals[j] = m_pfbxModel->GetNormals(j + 1);
			pd3dxvNormals[j + 1] = m_pfbxModel->GetNormals(j);
			pd3dxvNormals[j + 2] = m_pfbxModel->GetNormals(j + 2);
		}
		else
		{
			m_pd3dxvPositions[j] = m_pfbxModel->GetVertices(m_fbxIndices[j]);
			m_pd3dxvPositions[j + 1] = m_pfbxModel->GetVertices(m_fbxIndices[j + 1]);
			m_pd3dxvPositions[j + 2] = m_pfbxModel->GetVertices(m_fbxIndices[j + 2]);

			XMFLOAT2 tempUV0 = m_pfbxModel->GetTextureCoords(j);
			XMFLOAT2 tempUV1 = m_pfbxModel->GetTextureCoords(j + 1);
			XMFLOAT2 tempUV2 = m_pfbxModel->GetTextureCoords(j + 2);

			pd3dxvTexCoords[j] = tempUV0;
			pd3dxvTexCoords[j + 1] = tempUV1;
			pd3dxvTexCoords[j + 2] = tempUV2;

			pd3dxvNormals[j] = m_pfbxModel->GetNormals(j);
			pd3dxvNormals[j + 1] = m_pfbxModel->GetNormals(j + 1);
			pd3dxvNormals[j + 2] = m_pfbxModel->GetNormals(j + 2);
		}

		
		j += 3;
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	delete[] pd3dxvNormals;
	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);


	float fx = (m_pfbxModel->GetHighestX() - m_pfbxModel->GetLowestX()) * 0.5f;
	float fy = (m_pfbxModel->GetHighestY() - m_pfbxModel->GetLowestY());
	float fz = (m_pfbxModel->GetHighestZ() - m_pfbxModel->GetLowestZ()) * 0.5f;

	m_bcBoundingbox.Center = { 0,  0,  0 };
	m_bcBoundingbox.Extents = { fx, fy, fz };
	m_fSize = XMFLOAT3(fx, fy, fz);
#else
	m_pfbxModel = new FBXModelLoader(path, modelscale, isVoxel);
	m_pfbxModel->ProcessMeshInfo();

	m_nVertices = m_pfbxModel->GetIndiecesCount();
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

	//XMFLOAT2* m_pTexCoords;
	//m_pTexCoords = new XMFLOAT2[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];

	XMFLOAT3* pd3dxvNormals;
	pd3dxvNormals = new XMFLOAT3[m_nVertices];

	int *m_fbxIndices = m_pfbxModel->GetIndices();

	int j = 0;
	int n = m_pfbxModel->GetPolygonCount();

	for (int i = 0; i < n; i++)
	{
		//정점은 인덱스 순서대로 정보를 넣습니다. UV랑 노멀은 그냥 순서대로!
		if (m_pfbxModel->IsVoxel())
		{
			m_pd3dxvPositions[j] = m_pfbxModel->GetVertices(m_fbxIndices[j + 1]);
			m_pd3dxvPositions[j + 1] = m_pfbxModel->GetVertices(m_fbxIndices[j]);
			m_pd3dxvPositions[j + 2] = m_pfbxModel->GetVertices(m_fbxIndices[j + 2]);

			XMFLOAT2 tempUV0 = m_pfbxModel->GetTextureCoords(j);
			XMFLOAT2 tempUV1 = m_pfbxModel->GetTextureCoords(j + 1);
			XMFLOAT2 tempUV2 = m_pfbxModel->GetTextureCoords(j + 2);

			m_pTexCoords[j] = tempUV1;
			m_pTexCoords[j + 1] = tempUV0;
			m_pTexCoords[j + 2] = tempUV2;

			pd3dxvNormals[j] = m_pfbxModel->GetNormals(j + 1);
			pd3dxvNormals[j + 1] = m_pfbxModel->GetNormals(j);
			pd3dxvNormals[j + 2] = m_pfbxModel->GetNormals(j + 2);
		}
		else
		{
			m_pd3dxvPositions[j] = m_pfbxModel->GetVertices(m_fbxIndices[j]);
			m_pd3dxvPositions[j + 1] = m_pfbxModel->GetVertices(m_fbxIndices[j + 1]);
			m_pd3dxvPositions[j + 2] = m_pfbxModel->GetVertices(m_fbxIndices[j + 2]);

			XMFLOAT2 tempUV0 = m_pfbxModel->GetTextureCoords(j);
			XMFLOAT2 tempUV1 = m_pfbxModel->GetTextureCoords(j + 1);
			XMFLOAT2 tempUV2 = m_pfbxModel->GetTextureCoords(j + 2);

			m_pTexCoords[j] = tempUV0;
			m_pTexCoords[j + 1] = tempUV1;
			m_pTexCoords[j + 2] = tempUV2;

			pd3dxvNormals[j] = m_pfbxModel->GetNormals(j);
			pd3dxvNormals[j + 1] = m_pfbxModel->GetNormals(j + 1);
			pd3dxvNormals[j + 2] = m_pfbxModel->GetNormals(j + 2);
		}


		j += 3;
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


	delete[] pd3dxvNormals;
	delete[] m_pTexCoords;
	if (m_pTangents) delete[] m_pTangents;

	ID3D11Buffer *pd3dBuffers[4] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dTangentBuffer };
	UINT	  pnBufferStrides[4] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT3) };
	UINT	  pnBufferOffsets[4] = { 0, 0, 0 };
	AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);


	float fx = (m_pfbxModel->GetHighestX() - m_pfbxModel->GetLowestX()) * 0.5f;
	float fy = (m_pfbxModel->GetHighestY() - m_pfbxModel->GetLowestY());
	float fz = (m_pfbxModel->GetHighestZ() - m_pfbxModel->GetLowestZ()) * 0.5f;

	m_bcBoundingbox.Center = { 0,  0,  0 };
	m_bcBoundingbox.Extents = { fx, fy, fz };
	m_fSize = XMFLOAT3(fx, fy, fz);
#endif
}

CFBXSingleMeshTexturedIlluminated::~CFBXSingleMeshTexturedIlluminated()
{
}
