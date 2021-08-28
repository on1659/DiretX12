//-----------------------------------------------------------------------------
// File: CMesh.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "GameObject.h"
#include "HeightMap.h"
//------------------------------------------------------------------------------------------------
#ifdef _AABB_
void AABB::Merge(XMFLOAT3 d3dxvMinimum, XMFLOAT3 d3dxvMaximum)
	{
		if (d3dxvMinimum.x < m_d3dxvMinimum.x) m_d3dxvMinimum.x = d3dxvMinimum.x;
		if (d3dxvMinimum.y < m_d3dxvMinimum.y) m_d3dxvMinimum.y = d3dxvMinimum.y;
		if (d3dxvMinimum.z < m_d3dxvMinimum.z) m_d3dxvMinimum.z = d3dxvMinimum.z;
		if (d3dxvMaximum.x > m_d3dxvMaximum.x) m_d3dxvMaximum.x = d3dxvMaximum.x;
		if (d3dxvMaximum.y > m_d3dxvMaximum.y) m_d3dxvMaximum.y = d3dxvMaximum.y;
		if (d3dxvMaximum.z > m_d3dxvMaximum.z) m_d3dxvMaximum.z = d3dxvMaximum.z;
	}

	void AABB::Merge(AABB *pAABB)
	{
		AABB::Merge(pAABB->m_d3dxvMinimum, pAABB->m_d3dxvMaximum);
	}

	void AABB::Update(XMMATRIX& pmtxTransform)
	{
		XMVECTOR vVertices[8];
		vVertices[0] = XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z, 0.0f);
		vVertices[1] = XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z, 0.0f);
		vVertices[2] = XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z, 0.0f);
		vVertices[3] = XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z, 0.0f);
		vVertices[4] = XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z, 0.0f);
		vVertices[5] = XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z, 0.0f);
		vVertices[6] = XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z, 0.0f);
		vVertices[7] = XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z, 0.0f);

		XMStoreFloat3(&m_d3dxvMinimum, XMVectorSet(+FLT_MAX, +FLT_MAX, +FLT_MAX, 0.0f));
		XMStoreFloat3(&m_d3dxvMaximum, XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f));
		for (int i = 0; i < 8; i++)
		{
			XMFLOAT3 xf3Vector;
			XMStoreFloat3(&xf3Vector, XMVector3TransformCoord(vVertices[i], pmtxTransform));

			if (xf3Vector.x < m_d3dxvMinimum.x) m_d3dxvMinimum.x = xf3Vector.x;
			if (xf3Vector.y < m_d3dxvMinimum.y) m_d3dxvMinimum.y = xf3Vector.y;
			if (xf3Vector.z < m_d3dxvMinimum.z) m_d3dxvMinimum.z = xf3Vector.z;
			if (xf3Vector.x > m_d3dxvMaximum.x) m_d3dxvMaximum.x = xf3Vector.x;
			if (xf3Vector.y > m_d3dxvMaximum.y) m_d3dxvMaximum.y = xf3Vector.y;
			if (xf3Vector.z > m_d3dxvMaximum.z) m_d3dxvMaximum.z = xf3Vector.z;
		}

		/* '15.01.08 15:37 수정 - 이더*/
		/*
		XMFLOAT3 vVertices[8];
		XMStoreFloat3(&vVertices[0], XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z, 0.0f));
		XMStoreFloat3(&vVertices[1], XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z, 0.0f));
		XMStoreFloat3(&vVertices[2], XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z, 0.0f));
		XMStoreFloat3(&vVertices[3], XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z, 0.0f));
		XMStoreFloat3(&vVertices[4], XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z, 0.0f));
		XMStoreFloat3(&vVertices[5], XMVectorSet(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z, 0.0f));
		XMStoreFloat3(&vVertices[6], XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z, 0.0f));
		XMStoreFloat3(&vVertices[7], XMVectorSet(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z, 0.0f));
		//무의미한 연산 두번 
		XMStoreFloat3(&m_d3dxvMinimum, XMVectorSet(+FLT_MAX, +FLT_MAX, +FLT_MAX, 0.0f));
		XMStoreFloat3(&m_d3dxvMaximum, XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f));
		for (int i = 0; i < 8; i++)
		{
			XMStoreFloat3(&vVertices[i], XMVector3TransformCoord(XMLoadFloat3(&vVertices[i]), *pmtxTransform));
		
			if (vVertices[i].x < m_d3dxvMinimum.x) m_d3dxvMinimum.x = vVertices[i].x;
			if (vVertices[i].y < m_d3dxvMinimum.y) m_d3dxvMinimum.y = vVertices[i].y;
			if (vVertices[i].z < m_d3dxvMinimum.z) m_d3dxvMinimum.z = vVertices[i].z;
			if (vVertices[i].x > m_d3dxvMaximum.x) m_d3dxvMaximum.x = vVertices[i].x;
			if (vVertices[i].y > m_d3dxvMaximum.y) m_d3dxvMaximum.y = vVertices[i].y;
			if (vVertices[i].z > m_d3dxvMaximum.z) m_d3dxvMaximum.z = vVertices[i].z;
		}
		*/
		
	}
	
	BoundingBox AABB::GetBoudningBox()
	{
		BoundingBox bodingbox;
		XMVECTOR xmMin = XMLoadFloat3(&m_d3dxvMinimum);
		XMVECTOR xmMax = XMLoadFloat3(&m_d3dxvMaximum);
		XMVECTOR half = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);

		XMVECTOR xmCenter = XMVectorMultiply(XMVectorAdd(xmMax, xmMin),half);
		XMStoreFloat3(&bodingbox.Center, xmCenter);
	
		XMVECTOR xmExtern = XMVectorMultiply(XMVectorSubtract(xmMax, xmMin),half);
		XMStoreFloat3(&bodingbox.Extents, xmExtern);


		return bodingbox;
	}
#endif

//------------------------------------------------------------------------------------------------
	CMesh::CMesh(ID3D11Device* pd3dDevice) : Object()
{
	m_nType = VERTEX_POSITION_ELEMENT;

	m_nVertices = 0;
	m_nBuffers = 0;
	m_pd3dPositionBuffer = nullptr;

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_nSlot = 0;
	m_nStartVertex = 0;

	m_pd3dIndexBuffer = nullptr;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;
	m_nIndexOffset = 0;
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;

	m_pd3dxvPositions = nullptr;
	m_pnIndices = nullptr;
	
	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		
	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { +FLT_MAX, +FLT_MAX, +FLT_MAX };
}

CMesh::~CMesh()
{
	if (m_pd3dPositionBuffer) m_pd3dPositionBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();

	if (m_ppd3dVertexBuffers) delete m_ppd3dVertexBuffers;
	m_ppd3dVertexBuffers = nullptr;

	if (m_pnVertexStrides) delete m_pnVertexStrides;
	m_pnVertexStrides = nullptr;

	if (m_pnVertexOffsets) delete m_pnVertexOffsets;
	m_pnVertexOffsets = nullptr;

	//if (m_pd3dxvPositions) delete m_pd3dxvPositions;
	//m_pd3dxvPositions = nullptr;

	if (m_pnIndices) delete m_pnIndices;
	m_pnIndices = nullptr;
}

void CMesh::Release()
{
	if (m_pd3dPositionBuffer) m_pd3dPositionBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();

	if (m_ppd3dVertexBuffers) delete m_ppd3dVertexBuffers;
	m_ppd3dVertexBuffers = nullptr;

	if (m_pnVertexStrides) delete m_pnVertexStrides;
	m_pnVertexStrides = nullptr;

	if (m_pnVertexOffsets) delete m_pnVertexOffsets;
	m_pnVertexOffsets = nullptr;

	if (m_pd3dxvPositions) delete m_pd3dxvPositions;
	m_pd3dxvPositions = nullptr;

	if (m_pnIndices) delete m_pnIndices;
	m_pnIndices = nullptr;
}

ID3D11Buffer *CMesh::CreateBuffer(ID3D11Device* pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = d3dUsage;
	d3dBufferDesc.ByteWidth = nStride * nElements;
	d3dBufferDesc.BindFlags = nBindFlags;
	d3dBufferDesc.CPUAccessFlags = nCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;

	ID3D11Buffer *pd3dBuffer = nullptr;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &pd3dBuffer);
	return(pd3dBuffer);
}

void CMesh::AssembleToVertexBuffer(int nBuffers, ID3D11Buffer **ppd3dBuffers, UINT *pnBufferStrides, UINT *pnBufferOffsets)
{
	ID3D11Buffer **ppd3dNewVertexBuffers = new ID3D11Buffer*[m_nBuffers + nBuffers];
	UINT *pnNewVertexStrides = new UINT[m_nBuffers + nBuffers];
	UINT *pnNewVertexOffsets = new UINT[m_nBuffers + nBuffers];


	if (m_nBuffers > 0)
	{
		for (int i = 0; i < m_nBuffers; i++)
		{
			ppd3dNewVertexBuffers[i] = m_ppd3dVertexBuffers[i];
			pnNewVertexStrides[i] = m_pnVertexStrides[i];
			pnNewVertexOffsets[i] = m_pnVertexOffsets[i];
		}
		if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
		if (m_pnVertexStrides) delete[] m_pnVertexStrides;
		if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
	}

	for (int i = 0; i < nBuffers; i++)
	{
		ppd3dNewVertexBuffers[m_nBuffers + i] = ppd3dBuffers[i];
		pnNewVertexStrides[m_nBuffers + i] = pnBufferStrides[i];
		pnNewVertexOffsets[m_nBuffers + i] = pnBufferOffsets[i];
	}

	m_nBuffers += nBuffers;
	m_ppd3dVertexBuffers = ppd3dNewVertexBuffers;
	m_pnVertexStrides = pnNewVertexStrides;
	m_pnVertexOffsets = pnNewVertexOffsets;
}

void CMesh::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{										 //슬롯     버퍼개수		position버퍼				
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
}

void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nStartVertex);
}

void CMesh::RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances, int nStartInstance)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstances, m_nStartIndex, m_nBaseVertex, nStartInstance);
	else
		pd3dDeviceContext->DrawInstanced(m_nVertices, nInstances, m_nStartVertex, nStartInstance);
}

void CMesh::RenderInstancedWithBlending(ID3D11DeviceContext *pd3dDeviceContext, int nInstances, int nStartInstance)
{
	//OnPrepareRender(pd3dDeviceContext);
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);


	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstances, m_nStartIndex, m_nBaseVertex, nStartInstance);
	else
		pd3dDeviceContext->DrawInstanced(m_nVertices, nInstances, m_nStartVertex, nStartInstance);
}


bool RayIntersectTriangle(XMVECTOR *pd3dxvOrigin, XMVECTOR *pd3dxvDirection, XMVECTOR *pd3dxvP0, XMVECTOR *pd3dxvP1, XMVECTOR *pd3dxvP2, float *pfU, float *pfV, float *pfRayToTriangle)
{
	//KYT '16.02.07 memo
	/*XMVectorGetX 극혐 */
	XMVECTOR d3dxvEdge1 = *pd3dxvP1 - *pd3dxvP0;
	XMVECTOR d3dxvEdge2 = *pd3dxvP2 - *pd3dxvP0;
	XMVECTOR d3dxvP, d3dxvQ;
	d3dxvP = XMVector3Cross(*pd3dxvDirection, d3dxvEdge2);

	D3DXVECTOR3 v3Edge1 = XMLoadD3DXVECTOR3(d3dxvEdge1);
	D3DXVECTOR3 v3xvP = XMLoadD3DXVECTOR3(d3dxvP);
	float a = D3DXVec3Dot(&v3Edge1, &v3xvP);
	//float a = XMVectorGetX(XMVector3Dot(d3dxvEdge1, d3dxvP));
	if (::IsZero(a)) return(false);
	float f = 1.0f / a;
	XMVECTOR d3dxvP0ToOrigin = *pd3dxvOrigin - *pd3dxvP0;
	*pfU = f * XMVectorGetX(XMVector3Dot(d3dxvP0ToOrigin, d3dxvP));
	if ((*pfU < 0.0f) || (*pfU > 1.0f)) return(false);
	d3dxvQ = XMVector3Cross(d3dxvP0ToOrigin, d3dxvEdge1);
	*pfV = f * XMVectorGetX(XMVector3Dot(*pd3dxvDirection, d3dxvQ));
	if ((*pfV < 0.0f) || ((*pfU + *pfV) > 1.0f)) return(false);
	*pfRayToTriangle = f * XMVectorGetX(XMVector3Dot(d3dxvEdge2, d3dxvQ));
	return(*pfRayToTriangle >= 0.0f);
}

#define _WITH_D3DX_LIBRARY

int CMesh::CheckRayIntersection(XMVECTOR* pd3dxvRayPosition, XMVECTOR* pd3dxvRayDirection, MESHINTERSECTINFO* pd3dxIntersectInfo)
{
	int nIntersections = 0;
	BYTE *pbPositions = (BYTE *)m_pd3dxvPositions + m_pnVertexOffsets[0];

	int nOffset = (m_d3dPrimitiveTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) ? 3 : 1;
	int nPrimitives = (m_d3dPrimitiveTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) ? (m_nVertices / 3) : (m_nVertices - 2);
	if (m_nIndices > 0) nPrimitives = (m_d3dPrimitiveTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) ? (m_nIndices / 3) : (m_nIndices - 2);

	XMVECTOR v0, v1, v2;
	float fuHitBaryCentric, fvHitBaryCentric, fHitDistance, fNearHitDistance = FLT_MAX;
	for (int i = 0; i < nPrimitives; i++)
	{
		v0 = *(XMVECTOR *)(pbPositions + ((m_pnIndices) ? (m_pnIndices[(i*nOffset) + 0]) : ((i*nOffset) + 0)) * m_pnVertexStrides[0]);
		v1 = *(XMVECTOR *)(pbPositions + ((m_pnIndices) ? (m_pnIndices[(i*nOffset) + 1]) : ((i*nOffset) + 1)) * m_pnVertexStrides[0]);
		v2 = *(XMVECTOR *)(pbPositions + ((m_pnIndices) ? (m_pnIndices[(i*nOffset) + 2]) : ((i*nOffset) + 2)) * m_pnVertexStrides[0]);

#ifdef _WITH_D3DX_LIBRARY
		//Intersects(pd3dxvRayPosition, pd3dxvRayDirection, v0, v1, v2, fHitDistance);
		if (D3DXIntersectTri((D3DXVECTOR3 *)&v0, (D3DXVECTOR3 *)&v1, (D3DXVECTOR3 *)&v2, (D3DXVECTOR3 *)pd3dxvRayPosition, (D3DXVECTOR3 *)pd3dxvRayDirection, &fuHitBaryCentric, &fvHitBaryCentric, &fHitDistance))
#else
		if (::RayIntersectTriangle(pd3dxvRayPosition, pd3dxvRayDirection, &v0, &v1, &v2, &fuHitBaryCentric, &fvHitBaryCentric, &fHitDistance))
#endif
		{
			if (fHitDistance < fNearHitDistance)
			{
				fNearHitDistance = fHitDistance;
				if (pd3dxIntersectInfo)
				{
					pd3dxIntersectInfo->m_dwFaceIndex = i;
					pd3dxIntersectInfo->m_fU = fuHitBaryCentric;
					pd3dxIntersectInfo->m_fV = fvHitBaryCentric;
					pd3dxIntersectInfo->m_fDistance = fHitDistance;
				}
			}
			nIntersections++;
		}
	}
	return(nIntersections);
}
#ifdef _AABB_
void CMesh::CalculateBoundingCube()
{
	XMStoreFloat3(&m_bcBoundingCube.m_d3dxvMinimum, XMVectorSet(+FLT_MAX, +FLT_MAX, +FLT_MAX, 0.0f));
	XMStoreFloat3(&m_bcBoundingCube.m_d3dxvMaximum, XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f));
	for (int i = 0; i < m_nVertices; i++)
	{
		if (m_pd3dxvPositions[i].x < m_bcBoundingCube.m_d3dxvMinimum.x) m_bcBoundingCube.m_d3dxvMinimum.x = m_pd3dxvPositions[i].x;
		if (m_pd3dxvPositions[i].x > m_bcBoundingCube.m_d3dxvMaximum.x) m_bcBoundingCube.m_d3dxvMaximum.x = m_pd3dxvPositions[i].x;
		if (m_pd3dxvPositions[i].y < m_bcBoundingCube.m_d3dxvMinimum.y) m_bcBoundingCube.m_d3dxvMinimum.y = m_pd3dxvPositions[i].y;
		if (m_pd3dxvPositions[i].y > m_bcBoundingCube.m_d3dxvMaximum.y) m_bcBoundingCube.m_d3dxvMaximum.y = m_pd3dxvPositions[i].y;
		if (m_pd3dxvPositions[i].z < m_bcBoundingCube.m_d3dxvMinimum.z) m_bcBoundingCube.m_d3dxvMinimum.z = m_pd3dxvPositions[i].z;
		if (m_pd3dxvPositions[i].z > m_bcBoundingCube.m_d3dxvMaximum.z) m_bcBoundingCube.m_d3dxvMaximum.z = m_pd3dxvPositions[i].z;
	}
}
#else
//XMCost 변환 Cost가 더 커서 안건들임
void CMesh::CalculateBoundingCube()
{
	XMFLOAT3 xmf3Cen = m_bcBoundingCube.Center;
	XMFLOAT3 xmf3Ext = m_bcBoundingCube.Extents;

	XMFLOAT3 xmf3max = {xmf3Cen.x + xmf3Ext.x,	xmf3Cen.y + xmf3Ext.y,	xmf3Cen.z + xmf3Ext.z,	};
	XMFLOAT3 xmf3min = {xmf3Cen.x - xmf3Ext.x,	xmf3Cen.y - xmf3Ext.y,	xmf3Cen.z - xmf3Ext.z,
	};

	for (int i = 0; i < m_nVertices; i++)
	{
		if (m_pd3dxvPositions[i].x < xmf3min.x) xmf3min.x = m_pd3dxvPositions[i].x;
		if (m_pd3dxvPositions[i].x > xmf3max.x) xmf3max.x = m_pd3dxvPositions[i].x;
		if (m_pd3dxvPositions[i].y < xmf3min.y) xmf3min.y = m_pd3dxvPositions[i].y;
		if (m_pd3dxvPositions[i].y > xmf3max.y) xmf3max.y = m_pd3dxvPositions[i].y;
		if (m_pd3dxvPositions[i].z < xmf3min.z) xmf3min.z = m_pd3dxvPositions[i].z;
		if (m_pd3dxvPositions[i].z > xmf3max.z) xmf3max.z = m_pd3dxvPositions[i].z;
	}
	xmf3Cen = 
	{ (xmf3min.x + xmf3max.x) * 0.5f,	(xmf3min.y + xmf3max.y) * 0.5f,	(xmf3min.z + xmf3max.z) * 0.5f,};
	xmf3Ext = 
	{ fabs(xmf3min.x - xmf3max.x) * 0.5f, fabs(xmf3min.y - xmf3max.y) * 0.5f, fabs(xmf3min.z - xmf3max.z) * 0.5f, };
	m_bcBoundingCube.Center = xmf3Cen;
	m_bcBoundingCube.Extents = xmf3Ext;
	m_fSize = xmf3Ext;
}

#endif
//------------------------------------------------------------------------------------------------
CMeshDiffused::CMeshDiffused(ID3D11Device* pd3dDevice) : CMesh(pd3dDevice)
{
	m_nType |= VERTEX_COLOR_ELEMENT;
	m_pd3dColorBuffer = nullptr;
}

CMeshDiffused::~CMeshDiffused()
{
	if (m_pd3dColorBuffer) m_pd3dColorBuffer->Release();
}



CNormalMesh::CNormalMesh(ID3D11Device*  pd3dDevice) : CMesh(pd3dDevice)
{
}

CNormalMesh::~CNormalMesh()
{
	if (m_pd3dTangentBuffer)m_pd3dTangentBuffer->Release();
	m_pd3dTangentBuffer = nullptr;

	if (m_pTexCoords) delete[] m_pTexCoords;
	m_pTexCoords = nullptr;

	if (m_pNormals) delete[] m_pNormals;
	m_pNormals = nullptr;

	if (m_pTangents) delete[] m_pTangents;
	m_pTangents = nullptr;
}

void CNormalMesh::Release()
{
	CMesh::Release();
	if (m_pd3dTangentBuffer)m_pd3dTangentBuffer->Release();
	m_pd3dTangentBuffer = nullptr;

	if (m_pTexCoords) delete[] m_pTexCoords;
	m_pTexCoords = nullptr;

	if (m_pNormals) delete[] m_pNormals;
	m_pNormals = nullptr;

	if (m_pTangents) delete[] m_pTangents;
	m_pTangents = nullptr;
}

XMVECTOR CNormalMesh::CalculateTriAngleTangent(XMFLOAT2* pd3dxvTexCoords, UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMFLOAT3 d3dxvTangent(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vector1, vector2;
	float tU[2];
	float tV[2];

	float determinant;

	float X0 = m_pd3dxvPositions[nIndex0].x;
	float X1 = m_pd3dxvPositions[nIndex1].x;
	float X2 = m_pd3dxvPositions[nIndex2].x;

	float Y0 = m_pd3dxvPositions[nIndex0].y;
	float Y1 = m_pd3dxvPositions[nIndex1].y;
	float Y2 = m_pd3dxvPositions[nIndex2].y;

	float Z0 = m_pd3dxvPositions[nIndex0].z;
	float Z1 = m_pd3dxvPositions[nIndex1].z;
	float Z2 = m_pd3dxvPositions[nIndex2].z;

	vector1.x = X1 - X0;
	vector1.y = Y1 - Y0;
	vector1.z = Z1 - Z0;

	vector2.x = X2 - X0;
	vector2.y = Y2 - Y0;
	vector2.z = Z2 - Z0;

	float U0 = m_pTexCoords[nIndex0].x;
	float U1 = m_pTexCoords[nIndex1].x;
	float U2 = m_pTexCoords[nIndex2].x;

	float V0 = m_pTexCoords[nIndex0].y;
	float V1 = m_pTexCoords[nIndex1].y;
	float V2 = m_pTexCoords[nIndex2].y;


	tU[0] = U1 - U0;
	tU[1] = U2 - U0;

	tV[0] = V1 - V0;
	tV[1] = V2 - V0;

	//행렬식 구하기
	determinant = (tU[0] * tV[1] - tU[1] * tV[0]);

	//d3dxvTangent.x = (tuVector.x * vector2.x - tvVector.x * vector1.x) * determinant;
	//d3dxvTangent.y = (tuVector.x * vector2.y - tvVector.x * vector1.y) * determinant;
	//d3dxvTangent.z = (tuVector.x * vector2.z - tvVector.x * vector1.z) * determinant;

	d3dxvTangent.x = ((tV[1] * vector1.x) + (-tV[0] * vector2.x)) / determinant;
	d3dxvTangent.y = ((tV[1] * vector1.y) + (-tV[0] * vector2.y)) / determinant;
	d3dxvTangent.z = ((tV[1] * vector1.z) + (-tV[0] * vector2.z)) / determinant;

	m_xmVector = XMLoadFloat3(&d3dxvTangent);

	return(m_xmVector);
}
XMVECTOR CNormalMesh::CalculateTriAngleTangent(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMFLOAT3 vTangent{ 0.0f, 0.0f, 0.0f };
	XMFLOAT3 vector1, vector2;
	XMFLOAT2 tuVector, tvVector;
	float den;

	vector1.x = m_pd3dxvPositions[nIndex1].x - m_pd3dxvPositions[nIndex0].x;
	vector1.y = m_pd3dxvPositions[nIndex1].y - m_pd3dxvPositions[nIndex0].y;
	vector1.z = m_pd3dxvPositions[nIndex1].z - m_pd3dxvPositions[nIndex0].z;

	vector2.x = m_pd3dxvPositions[nIndex2].x - m_pd3dxvPositions[nIndex0].x;
	vector2.y = m_pd3dxvPositions[nIndex2].y - m_pd3dxvPositions[nIndex0].y;
	vector2.z = m_pd3dxvPositions[nIndex2].z - m_pd3dxvPositions[nIndex0].z;

	tuVector.x = m_pTexCoords[nIndex1].x - m_pTexCoords[nIndex0].x;
	tuVector.y = m_pTexCoords[nIndex1].y - m_pTexCoords[nIndex0].y;
	tvVector.x = m_pTexCoords[nIndex2].x - m_pTexCoords[nIndex0].x;
	tvVector.y = m_pTexCoords[nIndex2].y - m_pTexCoords[nIndex0].y;

	den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);

	vTangent.x = (tuVector.x * vector2.x - tvVector.x * vector1.x) * den;
	vTangent.y = (tuVector.x * vector2.y - tvVector.x * vector1.y) * den;
	vTangent.z = (tuVector.x * vector2.z - tvVector.x * vector1.z) * den;

	return(XMVector3Normalize(XMLoadFloat3(&vTangent)));
}
void CNormalMesh::SetAverageVertexTangent(XMVECTOR *pd3dxvTangents, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR d3dxvSumOfTangent = XMVectorZero();
	XMVECTOR *pd3dxvPositions = NULL;
	UINT nIndex0, nIndex1, nIndex2;

	for (int j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfTangent = XMVectorZero();
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (m_pnIndices) nIndex0 = m_pnIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (m_pnIndices) nIndex1 = m_pnIndices[nIndex1];
			nIndex2 = (m_pnIndices) ? m_pnIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfTangent += CalculateTriAngleTangent(nIndex0, nIndex1, nIndex2);
		}
		d3dxvSumOfTangent = XMVector3Normalize(d3dxvSumOfTangent);
		pd3dxvTangents[j] = d3dxvSumOfTangent;
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + j;

	}
}
void CNormalMesh::SetTriAngleListVertexTangent(XMVECTOR *pd3dxvTangents)
{
	XMVECTOR d3dxvTangent;
	XMVECTOR *pd3dxvPositions = NULL;
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvTangent = CalculateTriAngleTangent((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + (i * 3 + 0);
		pd3dxvTangents[i * 3 + 0] = d3dxvTangent;
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + (i * 3 + 1);
		pd3dxvTangents[i * 3 + 1] = d3dxvTangent;
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + (i * 3 + 2);
		pd3dxvTangents[i * 3 + 2] = d3dxvTangent;
	}
}
void CNormalMesh::CalculateVertexTangent(XMVECTOR *pd3dxvTangents)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
		if (!m_pnIndices)
			SetTriAngleListVertexTangent(pd3dxvTangents);
		else
			SetAverageVertexTangent(pd3dxvTangents, (m_nIndices / 3), 3, false);
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexTangent(pd3dxvTangents, (m_pnIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}


//------------------------------------------------------------------------------------------------
CMeshIlluminated::CMeshIlluminated(ID3D11Device* pd3dDevice) : CNormalMesh(pd3dDevice)
{
	m_nType |= VERTEX_NORMAL_ELEMENT;
	m_pd3dNormalBuffer = nullptr;
}

CMeshIlluminated::~CMeshIlluminated()
{
	if (m_pd3dNormalBuffer) m_pd3dNormalBuffer->Release();
}

XMVECTOR& CMeshIlluminated::CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	m_xmVector = XMVectorZero();
	XMVECTOR d3dxvP0 = XMLoadFloat3(&m_pd3dxvPositions[nIndex0]);
	XMVECTOR d3dxvP1 = XMLoadFloat3(&m_pd3dxvPositions[nIndex1]);
	XMVECTOR d3dxvP2 = XMLoadFloat3(&m_pd3dxvPositions[nIndex2]);
	XMVECTOR d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	XMVECTOR d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	m_xmVector = XMVector3Cross(d3dxvEdge1, d3dxvEdge2);
	m_xmVector = XMVector3Normalize(m_xmVector);
	return(m_xmVector);
}

void CMeshIlluminated::SetTriAngleListVertexNormal(XMVECTOR *pd3dxvNormals)
{
	XMVECTOR d3dxvNormal;
	XMVECTOR *pd3dxvPositions = nullptr;
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{

		d3dxvNormal = CalculateTriAngleNormal((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + (i * 3 + 0);
		pd3dxvNormals[i * 3 + 0] = d3dxvNormal;
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + (i * 3 + 1);
		pd3dxvNormals[i * 3 + 1] = d3dxvNormal;
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + (i * 3 + 2);
		pd3dxvNormals[i * 3 + 2] = d3dxvNormal;
	}
}

void CMeshIlluminated::SetAverageVertexNormal(XMVECTOR *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR d3dxvSumOfNormal = XMVectorZero();
	XMVECTOR *pd3dxvPositions = nullptr;
	UINT nIndex0, nIndex1, nIndex2;

	for (int j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = XMVectorZero();
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (m_pnIndices) nIndex0 = m_pnIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (m_pnIndices) nIndex1 = m_pnIndices[nIndex1];
			nIndex2 = (m_pnIndices) ? m_pnIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(nIndex0, nIndex1, nIndex2);
		}
		d3dxvSumOfNormal = XMVector3Normalize(d3dxvSumOfNormal);
		pd3dxvNormals[j] = d3dxvSumOfNormal;
		pd3dxvPositions = &XMLoadFloat3(m_pd3dxvPositions) + j;

	}
}

void CMeshIlluminated::CalculateVertexNormal(XMVECTOR *pd3dxvNormals)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!m_pnIndices)
			SetTriAngleListVertexNormal(pd3dxvNormals);
		else
			SetAverageVertexNormal(pd3dxvNormals, (m_nIndices / 3), 3, false);
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pd3dxvNormals, (m_pnIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------
CMeshTextured::CMeshTextured(ID3D11Device* pd3dDevice) : CMesh(pd3dDevice)
{
	m_nType |= VERTEX_TEXTURE_ELEMENT_0;
	m_pd3dTexCoordBuffer = nullptr;
}

CMeshTextured::~CMeshTextured()
{
	if (m_pd3dTexCoordBuffer) m_pd3dTexCoordBuffer->Release();
}

//------------------------------------------------------------------------------------------------
CMeshDetailTextured::CMeshDetailTextured(ID3D11Device* pd3dDevice) : CMeshTextured(pd3dDevice)
{
	m_nType |= VERTEX_TEXTURE_ELEMENT_1;
	m_pd3dDetailTexCoordBuffer = nullptr;
}

CMeshDetailTextured::~CMeshDetailTextured()
{
	if (m_pd3dDetailTexCoordBuffer) m_pd3dDetailTexCoordBuffer->Release();
}

//------------------------------------------------------------------------------------------------
CMeshTexturedIlluminated::CMeshTexturedIlluminated(ID3D11Device* pd3dDevice) : CMeshIlluminated(pd3dDevice)
{
	m_nType |= VERTEX_TEXTURE_ELEMENT_0;
	m_pd3dTexCoordBuffer = nullptr;
}

CMeshTexturedIlluminated::~CMeshTexturedIlluminated()
{
	if (m_pd3dTexCoordBuffer) m_pd3dTexCoordBuffer->Release();
}

//------------------------------------------------------------------------------------------------
CMeshDetailTexturedIlluminated::CMeshDetailTexturedIlluminated(ID3D11Device* pd3dDevice) : CMeshIlluminated(pd3dDevice)
{
	m_nType |= (VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1);
	m_pd3dTexCoordBuffer = nullptr;
	m_pd3dDetailTexCoordBuffer = nullptr;
}

CMeshDetailTexturedIlluminated::~CMeshDetailTexturedIlluminated()
{
	if (m_pd3dTexCoordBuffer) m_pd3dTexCoordBuffer->Release();
	if (m_pd3dDetailTexCoordBuffer) m_pd3dDetailTexCoordBuffer->Release();
}


//------------------------------------------------------------------------------------------------
CAirplaneMeshDiffused::CAirplaneMeshDiffused(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth, XMVECTOR d3dxColor) : CMeshDiffused(pd3dDevice)
{
	m_nVertices = 24 * 3;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

	float x1 = fx * 0.2f, z1 = fz * 0.2f, x2 = fx * 0.1f, z3 = fz * 0.3f, z2 = ((z1 - (fz - z3)) / x1) * x2 + (fz - z3);
	int i = 0;
	//Upper Plane
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, +fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, +fy, -z1);

	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, +fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -z3);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, +fy, -z1);

	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, +fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, +fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -z3);

	//Lower Plane
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, -fy, -z1);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, -fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, 0.0f);

	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, -fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, -fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -z3);

	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, -fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -z3);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, -fy, -z1);

	//Right Plane
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, +fy, +z2);

	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, +fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, -fy, +z2);

	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, +fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, -fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -z3);

	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -z3);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x2, -fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -z3);

	//Back/Right Plane
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, +fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -z3);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -z3);

	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, +fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -z3);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, -fy, -z1);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, +fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, -fy, -z1);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(+x1, -fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, 0.0f);

	//Left Plane
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, +fy, +z2);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, +(fz + z3));
	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, +fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, -fy, +z2);

	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, -fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, +fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -z3);

	m_pd3dxvPositions[i++] = XMFLOAT3(-x2, -fy, +z2);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -z3);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -z3);

	//Back/Left Plane
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, -fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, -fy, -z1);

	m_pd3dxvPositions[i++] = XMFLOAT3(0.0f, +fy, 0.0f);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, -fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, +fy, -z1);

	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, +fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, -fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -z3);

	m_pd3dxvPositions[i++] = XMFLOAT3(-x1, +fy, -z1);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -z3);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -z3);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	XMCOLOR pd3dxColors[24 * 3];
	for (int j = 0; j < m_nVertices; j++) XMStoreColor(&pd3dxColors[j], d3dxColor/* + RANDOM_COLOR*/);
	pd3dxColors[0] = XMCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pd3dxColors[3] = XMCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pd3dxColors[6] = XMCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pd3dxColors[9] = XMCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pd3dxColors[12] = XMCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pd3dxColors[15] = XMCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pd3dxColors[18] = XMCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pd3dxColors[21] = XMCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	m_pd3dColorBuffer = CreateBuffer(pd3dDevice, sizeof(XMCOLOR), m_nVertices, pd3dxColors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *ppd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMCOLOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, ppd3dBuffers, pnBufferStrides, pnBufferOffsets);

	#ifdef _AABB_
		m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
		m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);
	#else
		m_bcBoundingCube.Center = { 0.f, 0.f, 0.f };
		m_bcBoundingCube.Extents = { fx, fy, fz };
	#endif
		m_fSize = XMFLOAT3(fx, fy, fz);
}

CAirplaneMeshDiffused::~CAirplaneMeshDiffused()
{
}

//------------------------------------------------------------------------------------------------
CCubeMeshDiffused::CCubeMeshDiffused(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth, XMVECTOR d3dxColor) : CMeshDiffused(pd3dDevice)
{
	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

	m_pd3dxvPositions[0] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[1] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[2] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[3] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[4] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[5] = XMFLOAT3(+fx, -fy, -fz);
	m_pd3dxvPositions[6] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[7] = XMFLOAT3(-fx, -fy, +fz);

	XMFLOAT4 pd3dxColors[8];
	for (int i = 0; i < 8; i++) XMStoreFloat4(&pd3dxColors[i], d3dxColor/* + RANDOM_COLOR*/);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dColorBuffer = CreateBuffer(pd3dDevice, sizeof(XMCOLOR), m_nVertices, pd3dxColors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMCOLOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

#define _WITH_CORRECT_STRIP
#ifdef _WITH_CORRECT_STRIP
	m_nIndices = 18;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 5; //5,6,4 - cw
	m_pnIndices[1] = 6; //6,4,7 - ccw
	m_pnIndices[2] = 4; //4,7,0 - cw
	m_pnIndices[3] = 7; //7,0,3 - ccw
	m_pnIndices[4] = 0; //0,3,1 - cw
	m_pnIndices[5] = 3; //3,1,2 - ccw
	m_pnIndices[6] = 1; //1,2,2 - cw 
	m_pnIndices[7] = 2; //2,2,3 - ccw
	m_pnIndices[8] = 2; //2,3,3 - cw  - Degenerated Index
	m_pnIndices[9] = 3; //3,3,7 - ccw - Degenerated Index
	m_pnIndices[10] = 3;//3,7,2 - cw  - Degenerated Index
	m_pnIndices[11] = 7;//7,2,6 - ccw
	m_pnIndices[12] = 2;//2,6,1 - cw
	m_pnIndices[13] = 6;//6,1,5 - ccw
	m_pnIndices[14] = 1;//1,5,0 - cw
	m_pnIndices[15] = 5;//5,0,4 - ccw
	m_pnIndices[16] = 0;
	m_pnIndices[17] = 4;
#else
	m_nIndices = 16;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 5; //5,6,4 - cw
	m_pnIndices[1] = 6; //6,4,7 - ccw
	m_pnIndices[2] = 4; //4,7,0 - cw
	m_pnIndices[3] = 7; //7,0,3 - ccw
	m_pnIndices[4] = 0; //0,3,1 - cw
	m_pnIndices[5] = 3; //3,1,2 - ccw
	m_pnIndices[6] = 1; //1,2,3 - cw 
	m_pnIndices[7] = 2; //2,3,7 - ccw
	m_pnIndices[8] = 3; //3,7,2 - cw - Degenerated Index
	m_pnIndices[9] = 7; //7,2,6 - ccw
	m_pnIndices[10] = 2;//2,6,1 - cw
	m_pnIndices[11] = 6;//6,1,5 - ccw
	m_pnIndices[12] = 1;//1,5,0 - cw
	m_pnIndices[13] = 5;//5,0,4 - ccw
	m_pnIndices[14] = 0;
	m_pnIndices[15] = 4;
#endif

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);
	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fx, fy, fz };
	m_fSize = XMFLOAT3(fx, fy, fz);
}

CCubeMeshDiffused::~CCubeMeshDiffused()
{
}

//------------------------------------------------------------------------------------------------
CSphereMeshDiffused::CSphereMeshDiffused(ID3D11Device* pd3dDevice, float fRadius, int nSlices, int nStacks, XMVECTOR d3dxColor) : CMeshDiffused(pd3dDevice)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			m_pd3dxvPositions[k++] = XMFLOAT3(fRadius_j*cosf(theta_i), y_j, fRadius_j*sinf(theta_i));
			m_pd3dxvPositions[k++] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			m_pd3dxvPositions[k++] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			m_pd3dxvPositions[k++] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			m_pd3dxvPositions[k++] = XMFLOAT3(fRadius_jj*cosf(theta_ii), y_jj, fRadius_jj*sinf(theta_ii));
			m_pd3dxvPositions[k++] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
		}
	}

	XMCOLOR *pd3dxColors = new XMCOLOR[m_nVertices];
	for (int i = 0; i < m_nVertices; i++) XMStoreColor(&pd3dxColors[i], d3dxColor/* + RANDOM_COLOR*/);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dColorBuffer = CreateBuffer(pd3dDevice, sizeof(XMCOLOR), m_nVertices, pd3dxColors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxColors;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMCOLOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	
	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fRadius, -fRadius, -fRadius);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fRadius, +fRadius, +fRadius);
	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fRadius, fRadius, fRadius };
	m_fSize = XMFLOAT3(fRadius, fRadius, fRadius);
}

CSphereMeshDiffused::~CSphereMeshDiffused()
{
}

//------------------------------------------------------------------------------------------------
CCubeMeshIlluminated::CCubeMeshIlluminated(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshIlluminated(pd3dDevice)
{
	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];

	m_pd3dxvPositions[0] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[1] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[2] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[3] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[4] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[5] = XMFLOAT3(+fx, -fy, -fz);
	m_pd3dxvPositions[6] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[7] = XMFLOAT3(-fx, -fy, +fz);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	XMFLOAT3 pd3dxvNormals[8];
	for (int i = 0; i < 8; i++) pd3dxvNormals[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_nIndices = 36;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 3; m_pnIndices[1] = 1; m_pnIndices[2] = 0;
	m_pnIndices[3] = 2; m_pnIndices[4] = 1; m_pnIndices[5] = 3;
	m_pnIndices[6] = 0; m_pnIndices[7] = 5; m_pnIndices[8] = 4;
	m_pnIndices[9] = 1; m_pnIndices[10] = 5; m_pnIndices[11] = 0;
	m_pnIndices[12] = 3; m_pnIndices[13] = 4; m_pnIndices[14] = 7;
	m_pnIndices[15] = 0; m_pnIndices[16] = 4; m_pnIndices[17] = 3;
	m_pnIndices[18] = 1; m_pnIndices[19] = 6; m_pnIndices[20] = 5;
	m_pnIndices[21] = 2; m_pnIndices[22] = 6; m_pnIndices[23] = 1;
	m_pnIndices[24] = 2; m_pnIndices[25] = 7; m_pnIndices[26] = 6;
	m_pnIndices[27] = 3; m_pnIndices[28] = 7; m_pnIndices[29] = 2;
	m_pnIndices[30] = 6; m_pnIndices[31] = 4; m_pnIndices[32] = 5;
	m_pnIndices[33] = 7; m_pnIndices[34] = 4; m_pnIndices[35] = 6;

	CalculateVertexNormal(&XMLoadFloat3(pd3dxvNormals));

	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);
	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fx, fy, fz };
	m_fSize = XMFLOAT3(fx, fy, fz);
}

CCubeMeshIlluminated::~CCubeMeshIlluminated()
{
}

#define _WITH_INDEX_BUFFER

//------------------------------------------------------------------------------------------------
CSphereMeshIlluminated::CSphereMeshIlluminated(ID3D11Device* pd3dDevice, float fRadius, int nSlices, int nStacks) : CMeshIlluminated(pd3dDevice)
{
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fDeltaPhi = float(D3DX_PI / nStacks);
	float fDeltaTheta = float((2.0f * D3DX_PI) / nSlices);
	int k = 0;

#ifdef _WITH_INDEX_BUFFER
	m_nVertices = 2 + (nSlices * (nStacks - 1));
	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT3 *pd3dxvNormals = new XMFLOAT3[m_nVertices];

	m_pd3dxvPositions[k] = XMFLOAT3(0.0f, +fRadius, 0.0f);
	XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
	float theta_i, phi_j;
	for (int j = 1; j < nStacks; j++)
	{
		phi_j = fDeltaPhi * j;
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = fDeltaTheta * i;
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius*sinf(phi_j)*cosf(theta_i), fRadius*cosf(phi_j), fRadius*sinf(phi_j)*sinf(theta_i));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
		}
	}
	m_pd3dxvPositions[k] = XMFLOAT3(0.0f, -fRadius, 0.0f);
	XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	if (pd3dxvNormals) delete[] pd3dxvNormals;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	k = 0;
	m_nIndices = (nSlices * 3) * 2 + (nSlices * (nStacks - 2) * 3 * 2);
	m_pnIndices = new UINT[m_nIndices];
	for (int i = 0; i < nSlices; i++)
	{
		m_pnIndices[k++] = 0;
		m_pnIndices[k++] = 1 + ((i + 1) % nSlices);
		m_pnIndices[k++] = 1 + i;
	}
	for (int j = 0; j < nStacks - 2; j++)
	{
		for (int i = 0; i < nSlices; i++)
		{
			m_pnIndices[k++] = 1 + (i + (j * nSlices));
			m_pnIndices[k++] = 1 + (((i + 1) % nSlices) + (j * nSlices));
			m_pnIndices[k++] = 1 + (i + ((j + 1) * nSlices));
			m_pnIndices[k++] = 1 + (i + ((j + 1) * nSlices));
			m_pnIndices[k++] = 1 + (((i + 1) % nSlices) + (j * nSlices));
			m_pnIndices[k++] = 1 + (((i + 1) % nSlices) + ((j + 1) * nSlices));
		}
	}
	for (int i = 0; i < nSlices; i++)
	{
		m_pnIndices[k++] = (m_nVertices - 1);
		m_pnIndices[k++] = ((m_nVertices - 1) - nSlices) + i;
		m_pnIndices[k++] = ((m_nVertices - 1) - nSlices) + ((i + 1) % nSlices);
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	#else
		m_nVertices = (nSlices * 3) * 2 + (nSlices * (nStacks - 2) * 3 * 2);
		m_pd3dxvPositions = new XMVECTOR[m_nVertices];
		XMVECTOR *pd3dxvNormals = new XMVECTOR[m_nVertices];

		float theta_i, theta_ii, phi_j = 0.0f, phi_jj = fDeltaPhi;
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = fDeltaTheta * i;
			theta_ii = fDeltaTheta * (i + 1);
			m_pd3dxvPositions[k] = XMVECTOR(0.0f, +fRadius, 0.0f);
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
			m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
			m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_ii)*sinf(phi_jj));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
		}

		for (int j = 1; j < nStacks - 1; j++)
		{
			phi_j = fDeltaPhi * j;
			phi_jj = fDeltaPhi * (j + 1);
			for (int i = 0; i < nSlices; i++)
			{
				theta_i = fDeltaTheta * i;
				theta_ii = fDeltaTheta * (i + 1);
				m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_i)*sinf(phi_j));
				D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
				m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj));
				D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
				m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j));
				D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
				m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj));
				D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
				m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_ii)*sinf(phi_jj));
				D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
				m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j));
				D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
			}
		}
		phi_j = fDeltaPhi * (nStacks - 1);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = fDeltaTheta * i;
			theta_ii = fDeltaTheta * (i + 1);
			m_pd3dxvPositions[k] = XMVECTOR(0.0f, -fRadius, 0.0f);
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
			m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
			m_pd3dxvPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_i)*sinf(phi_j));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pd3dxvPositions[k]); k++;
		}

		m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMVECTOR), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMVECTOR), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		if (pd3dxvNormals) delete[] pd3dxvNormals;

		ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer };
		UINT pnBufferStrides[2] = { sizeof(XMVECTOR), sizeof(XMVECTOR) };
		UINT pnBufferOffsets[2] = { 0, 0 };
		AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	#endif


	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fRadius, -fRadius, -fRadius);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fRadius, +fRadius, +fRadius);
	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fRadius, fRadius, fRadius };
	m_fSize = XMFLOAT3(fRadius, fRadius, fRadius);
}

CSphereMeshIlluminated::~CSphereMeshIlluminated()
{
}


//------------------------------------------------------------------------------------------------
CCubeMeshTextured::CCubeMeshTextured(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	int i = 0;
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	
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

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);
	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fx, fy, fz };
	m_fSize = XMFLOAT3(fx, fy, fz);
}

CCubeMeshTextured::~CCubeMeshTextured()
{
}

//------------------------------------------------------------------------------------------------
CSphereMeshTextured::CSphereMeshTextured(ID3D11Device* pd3dDevice, float fRadius, int nSlices, int nStacks) : CMeshTextured(pd3dDevice)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_i), y_j, fRadius_j*sinf(theta_i));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i) / float(nSlices), float(j) / float(nStacks));
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_ii), y_jj, fRadius_jj*sinf(theta_ii));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i + 1) / float(nSlices), float(j + 1) / float(nStacks));
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
		}
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);


	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fRadius, -fRadius, -fRadius);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fRadius, +fRadius, +fRadius);

	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fRadius, fRadius, fRadius };
	m_fSize = XMFLOAT3(fRadius, fRadius, fRadius);
}

CSphereMeshTextured::~CSphereMeshTextured()
{
}

//------------------------------------------------------------------------------------------------
CCubeMeshTexturedIlluminated::CCubeMeshTexturedIlluminated(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshTexturedIlluminated(pd3dDevice)
{
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	int i = 0;
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pd3dxvPositions[i++] = XMFLOAT3(+fx, -fy, -fz);

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

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);

	m_bcBoundingbox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingbox.Extents = { fx, fy, fz };
	m_fSize = XMFLOAT3(fx, fy, fz);
}

CCubeMeshTexturedIlluminated::~CCubeMeshTexturedIlluminated()
{
}

//------------------------------------------------------------------------------------------------
CSphereMeshTexturedIlluminated::CSphereMeshTexturedIlluminated(ID3D11Device* pd3dDevice, float fRadius, int nSlices, int nStacks) : CMeshTexturedIlluminated(pd3dDevice)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT3 *pd3dxvNormals = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_i), y_j, fRadius_j*sinf(theta_i));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i) / float(nSlices), float(j) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_ii), y_jj, fRadius_jj*sinf(theta_ii));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i + 1) / float(nSlices), float(j + 1) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
			m_pd3dxvPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pd3dxvPositions[k]))); k++;
		}
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
	
	#ifdef _AABB_
		m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fRadius, -fRadius, -fRadius);
		m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fRadius, +fRadius, +fRadius);
	#else
		m_bcBoundingCube.Center = { 0.f, 0.f, 0.f };
		m_bcBoundingCube.Extents = { fRadius, fRadius, fRadius };
	#endif
		m_fSize = XMFLOAT3(fRadius, fRadius, fRadius);

}

CSphereMeshTexturedIlluminated::~CSphereMeshTexturedIlluminated()
{
}

//------------------------------------------------------------------------------------------------
CHeightMapGridMesh::CHeightMapGridMesh(ID3D11Device* pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale, void *pContext, D3D11_USAGE d3dUsage) : CMeshDetailTexturedIlluminated(pd3dDevice)
{
	m_nVertices = nWidth * nLength;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT3 *pd3dxvNormals = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];
	XMFLOAT2 *pd3dxvDetailTexCoords = new XMFLOAT2[m_nVertices];

	m_nWidth = nWidth;
	m_nLength = nLength;
	XMStoreFloat3(&m_d3dxvScale, d3dxvScale);

	CHeightMap *pHeightMap = (CHeightMap *)pContext;
	int cxHeightMap = (pHeightMap) ? pHeightMap->GetHeightMapWidth() : nWidth;
	int czHeightMap = (pHeightMap) ? pHeightMap->GetHeightMapLength() : nLength;
	float fHeight = 0.0f, fMinHeight = +FLT_MAX, fMaxHeight = -FLT_MAX;
	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
	{
		//KYT '16.02.07 memo
		/*XMVectorGetX 극혐*/
		for (int x = xStart; x < (xStart + nWidth); x++, i++)
		{
			fHeight = OnGetHeight(x, z, pContext);
			m_pd3dxvPositions[i] = XMFLOAT3((x*m_d3dxvScale.x), fHeight, (z*m_d3dxvScale.z));
			pd3dxvNormals[i] = (pHeightMap) ? pHeightMap->GetHeightMapNormalXMFLOAT3(x, z) : XMFLOAT3(0.0f, 1.0f, 0.0f);
			pd3dxvTexCoords[i] = XMFLOAT2(float(x) / float(cxHeightMap - 1), float(czHeightMap - 1 - z) / float(czHeightMap - 1));
			pd3dxvDetailTexCoords[i] = XMFLOAT2(float(x) / float(m_d3dxvScale.x*0.125f), float(z) / float(m_d3dxvScale.z*0.125f));
			if (fHeight < fMinHeight) fMinHeight = fHeight;
			if (fHeight > fMaxHeight) fMaxHeight = fHeight;
		}
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, d3dUsage, (d3dUsage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dDetailTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvDetailTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	if (pd3dxvNormals) delete[] pd3dxvNormals;
	if (pd3dxvTexCoords) delete[] pd3dxvTexCoords;
	if (pd3dxvDetailTexCoords) delete[] pd3dxvDetailTexCoords;

	ID3D11Buffer *pd3dBuffers[4] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dDetailTexCoordBuffer };
	UINT pnBufferStrides[4] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[4] = { 0, 0, 0, 0 };
	AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_nIndices = ((nWidth * 2)*(nLength - 1)) + ((nLength - 1) - 1);
	m_pnIndices = new UINT[m_nIndices];
	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			for (int x = 0; x < nWidth; x++)
			{
				if ((x == 0) && (z > 0)) m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			for (int x = nWidth - 1; x >= 0; x--)
			{
				if (x == (nWidth - 1)) m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

#ifdef _AABB_
	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(xStart * m_d3dxvScale.x, fMinHeight, zStart * m_d3dxvScale.z);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3((xStart + nWidth) * m_d3dxvScale.x, fMaxHeight, (zStart + nLength) *m_d3dxvScale.z);
#else
	//XMCost - 처음 로딩할때 한번이니 무시 가능
	XMFLOAT3 d3dxvMinimum = XMFLOAT3(xStart*m_d3dxvScale.x, fMinHeight, zStart*m_d3dxvScale.z);
	XMFLOAT3 d3dxvMaximum = XMFLOAT3((xStart + nWidth)*m_d3dxvScale.x, fMaxHeight, (zStart + nLength)*m_d3dxvScale.z);

	m_bcBoundingCube.Center = {
		(d3dxvMinimum.x + d3dxvMaximum.x) * 0.5f,
		(d3dxvMinimum.y + d3dxvMaximum.y) * 0.5f,
		(d3dxvMinimum.z + d3dxvMaximum.z) * 0.5f
	};
	m_bcBoundingCube.Extents = {
		fabs(d3dxvMaximum.x - d3dxvMinimum.x) * 0.5f,
		fabs(d3dxvMaximum.y - d3dxvMinimum.y) * 0.5f,
		fabs(d3dxvMaximum.z - d3dxvMinimum.z) * 0.5f
	};
#endif
}

CHeightMapGridMesh::~CHeightMapGridMesh()
{
}

float CHeightMapGridMesh::OnGetHeight(int x, int z, void *pContext)
{
	float fHeight = 0.0f;
	if (pContext)
	{
		CHeightMap *pHeightMap = (CHeightMap *)pContext;
		BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
		XMFLOAT3 d3dxvScale = pHeightMap->GetScaleXMFLAOT3();
		int nWidth = pHeightMap->GetHeightMapWidth();
		fHeight = pHeightMapImage[x + (z*nWidth)] * (d3dxvScale).y;
	}
	return(fHeight);
}

//------------------------------------------------------------------------------------------------
CSkyBoxMesh::CSkyBoxMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 24;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	int i = 0;
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	// Front Quad (remember all quads point inward)
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Back Quad																
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Left Quad																
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Right Quad																
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Top Quad																	
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Bottom Quad																
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(+fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = XMFLOAT3(-fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_nIndices = 4;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 0;
	m_pnIndices[1] = 1;
	m_pnIndices[2] = 3;
	m_pnIndices[3] = 2;

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	
	#ifdef _AABB_
		m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, -fz);
		m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, +fz);
	#else
		m_bcBoundingCube.Center = { 0.f, 0.f, 0.f };
		m_bcBoundingCube.Extents = { fx, fy, fz };
	#endif
		m_fSize = XMFLOAT3(fx, fy, fz);

}

CSkyBoxMesh::~CSkyBoxMesh()
{
}

//------------------------------------------------------------------------------------------------
CWaterGridMesh::CWaterGridMesh(ID3D11Device* pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale, void *pContext, D3D11_USAGE d3dUsage) : CHeightMapGridMesh(pd3dDevice, xStart, zStart, nWidth, nLength, d3dxvScale, pContext, d3dUsage)
{
	m_xStart = xStart;
	m_zStart = zStart;

	m_pd3dxvPreviousPositions = nullptr;
	m_pd3dxvCurrentPositions = nullptr;
	m_pd3dxvNormalVectors = nullptr;
	m_pd3dxvTempPositions = nullptr;

	OnPrepareAnimation(0.00375f, 1.0f, 1.25f, 0.00001f);
}

CWaterGridMesh::~CWaterGridMesh()
{
	if (m_pd3dxvPreviousPositions) delete[] m_pd3dxvPreviousPositions;
	if (m_pd3dxvCurrentPositions) delete[] m_pd3dxvCurrentPositions;
	if (m_pd3dxvNormalVectors) delete[] m_pd3dxvNormalVectors;
	if (m_pd3dxvTempPositions) delete[] m_pd3dxvTempPositions;
}

void CWaterGridMesh::OnPrepareAnimation(float fTimeDelta, float fSpatialDelta, float fSpeed, float fDamping)
{
	m_fTimeDelta = fTimeDelta;
	m_fSpatialDelta = fSpatialDelta;

	m_fAccumulatedTime = 0.0f;

	float d = (fDamping * m_fTimeDelta) + 2.0f;
	float e = (fSpeed * fSpeed) * (m_fTimeDelta * m_fTimeDelta) / (m_fSpatialDelta * m_fSpatialDelta);
	m_fK1 = ((fDamping * m_fTimeDelta) - 2.0f) / d;
	m_fK2 = (4.0f - (8.0f * e)) / d;
	m_fK3 = (2.0f * e) / d;

	m_pd3dxvPreviousPositions = new XMFLOAT3[m_nLength*m_nWidth];
	m_pd3dxvCurrentPositions = new XMFLOAT3[m_nLength*m_nWidth];
	m_pd3dxvTempPositions = new XMFLOAT3[m_nLength*m_nWidth];
	m_pd3dxvNormalVectors = new XMFLOAT3[m_nLength*m_nWidth];

	for (int i = 0, z = m_zStart; z < (m_zStart + m_nLength); i++, z++)
	{
		for (int j = 0, x = m_xStart; x < (m_xStart + m_nWidth); j++, x++)
		{
			m_pd3dxvPreviousPositions[(i*m_nWidth) + j] = XMFLOAT3((x*m_d3dxvScale.x), 0.0f, (z*m_d3dxvScale.z));
			m_pd3dxvCurrentPositions[(i*m_nWidth) + j] = XMFLOAT3((x*m_d3dxvScale.x), 0.0f, (z*m_d3dxvScale.z));
			m_pd3dxvNormalVectors[(i*m_nWidth) + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	for (int k = 0; k < 10; k++)
	{
		int i = 5 + (rand() % (m_nLength - 10));
		int j = 5 + (rand() % (m_nWidth - 10));

		float fMagnitude = (0.5f + ((float)(rand()) / (float)RAND_MAX) * (1.25f - 0.5f));

		SetDisturbingForce(i, j, fMagnitude);
	}
}

void CWaterGridMesh::SetDisturbingForce(int i, int j, float fMagnitude)
{
	if (((i > 1) && (i < m_nLength - 2)) && ((j > 1) && (j < m_nWidth - 2)))
	{
		m_pd3dxvCurrentPositions[(i*m_nWidth) + j].y += fMagnitude;
		m_pd3dxvCurrentPositions[(i*m_nWidth) + j + 1].y += fMagnitude * 0.5f;
		m_pd3dxvCurrentPositions[(i*m_nWidth) + j - 1].y += fMagnitude * 0.5f;
		m_pd3dxvCurrentPositions[((i + 1)*m_nWidth) + j].y += fMagnitude * 0.5f;
		m_pd3dxvCurrentPositions[((i - 1)*m_nWidth) + j].y += fMagnitude * 0.5f;
	}
}

float CWaterGridMesh::OnGetHeight(int x, int z, void *pContext)
{
	return(0.0f);
}

void CWaterGridMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	Animate(pd3dDeviceContext, 0.00003f);

	CHeightMapGridMesh::Render(pd3dDeviceContext);
}

void CWaterGridMesh::Animate(ID3D11DeviceContext *pd3dDeviceContext, float fTimeElapsed)
{
	m_fAccumulatedTime += fTimeElapsed;
	if (m_fAccumulatedTime >= m_fTimeDelta)
	{
		XMFLOAT3 d3dxvPosition(0.0f, 1.0f, 0.0f), d3dxvNormal(0.0f, 1.0f, 0.0f);
		float fyPreviousPosition = 0.0f;
		for (int i = 1; i < m_nLength - 1; i++)
		{
			for (int j = 1; j < m_nWidth - 1; j++)
			{
				m_pd3dxvPreviousPositions[(i*m_nWidth) + j].y = m_fK1 * m_pd3dxvPreviousPositions[(i*m_nWidth) + j].y + m_fK2 * m_pd3dxvCurrentPositions[(i*m_nWidth) + j].y + m_fK3 * (m_pd3dxvCurrentPositions[((i + 1)*m_nWidth) + j].y + m_pd3dxvCurrentPositions[((i - 1)*m_nWidth) + j].y + m_pd3dxvCurrentPositions[(i*m_nWidth) + j + 1].y + m_pd3dxvCurrentPositions[(i*m_nWidth) + j - 1].y);
			}
		}

		memcpy(m_pd3dxvTempPositions, m_pd3dxvPreviousPositions, sizeof(XMFLOAT3)*m_nWidth*m_nLength);
		memcpy(m_pd3dxvPreviousPositions, m_pd3dxvCurrentPositions, sizeof(XMFLOAT3)*m_nWidth*m_nLength);
		memcpy(m_pd3dxvCurrentPositions, m_pd3dxvTempPositions, sizeof(XMFLOAT3)*m_nWidth*m_nLength);

		//for (int i = 1; i < m_nLength - 1; i++)
		//{
		//	for (int j = 1; j < m_nWidth - 1; j++)
		//	{
		//		// Find two tangent vectors, one in +x and one in -z direction. Then take cross product to get normal. Use finite difference to compute tangents.  
		//		float l = m_pd3dxvCurrentPositions[(i*m_nWidth) + j - 1].y;
		//		float r = m_pd3dxvCurrentPositions[(i*m_nWidth) + j + 1].y;
		//		float t = m_pd3dxvCurrentPositions[((i - 1)*m_nWidth) + j - 1].y;
		//		float b = m_pd3dxvCurrentPositions[((i + 1)*m_nWidth) + j + 1].y;
		//		d3dxvNormal = XMFLOAT3((-r + l), (2.0f * m_fSpatialDelta), (b - t));
		//		XMStoreFloat3(&d3dxvNormal, XMVector3Normalize(XMLoadFloat3(&d3dxvNormal)));
		//		m_pd3dxvNormalVectors[(i*m_nWidth) + j] = d3dxvNormal;
		//	}
		//}

		//D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		//pd3dDeviceContext->Map(m_pd3dPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		//XMFLOAT3 *pd3dxvPositions = (XMFLOAT3 *)d3dMappedResource.pData;
		//D3DXVECTOR2 d3dxvTexture1, d3dxvTexture2;
		//for (int i = 0, z = m_zStart; z < (m_zStart + m_nLength); z++, i++)
		//{
		//	for (int j = 0, x = m_xStart; x < (m_xStart + m_nWidth); x++, j++)
		//	{
		//		pd3dxvPositions[(i*m_nWidth) + j] = m_pd3dxvCurrentPositions[(i*m_nWidth) + j];
		//	}
		//}
		//pd3dDeviceContext->Unmap(m_pd3dPositionBuffer, 0);

		m_fAccumulatedTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------------------
CTextureToScreenRectMesh::CTextureToScreenRectMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 6;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	m_pd3dxvPositions[0] = XMFLOAT3(+fx, +fy, 0.0f); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
	m_pd3dxvPositions[1] = XMFLOAT3(+fx, -fy, 0.0f); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
	m_pd3dxvPositions[2] = XMFLOAT3(-fx, -fy, 0.0f); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);
	m_pd3dxvPositions[3] = XMFLOAT3(-fx, -fy, 0.0f); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);
	m_pd3dxvPositions[4] = XMFLOAT3(-fx, +fy, 0.0f); pd3dxvTexCoords[4] = XMFLOAT2(0.0f, 0.0f);
	m_pd3dxvPositions[5] = XMFLOAT3(+fx, +fy, 0.0f); pd3dxvTexCoords[5] = XMFLOAT2(1.0f, 0.0f);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	#ifdef _AABB_
		m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, 0);
		m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, 0);
	#else
		m_bcBoundingCube.Center = { 0.f, 0.f, 0.f };
		m_bcBoundingCube.Extents = { fx, fy, 0.0f };
	#endif
		m_fSize = XMFLOAT3(fx, fy, 0.0f);

}

CTextureToScreenRectMesh::~CTextureToScreenRectMesh()
{
}

//------------------------------------------------------------------------------------------------
CTexturedRectMesh::CTexturedRectMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fLength) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 6;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fLength*0.5f;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	if ((fx != 0.0f) && (fz != 0.0f) && (fy == 0.0f))
	{
		m_pd3dxvPositions[0] = XMFLOAT3(+fx, 0.0f, +fz); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
		m_pd3dxvPositions[1] = XMFLOAT3(+fx, 0.0f, -fz); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
		m_pd3dxvPositions[2] = XMFLOAT3(-fx, 0.0f, -fz); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);
		m_pd3dxvPositions[3] = XMFLOAT3(-fx, 0.0f, -fz); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);
		m_pd3dxvPositions[4] = XMFLOAT3(-fx, 0.0f, +fz); pd3dxvTexCoords[4] = XMFLOAT2(0.0f, 0.0f);
		m_pd3dxvPositions[5] = XMFLOAT3(+fx, 0.0f, +fz); pd3dxvTexCoords[5] = XMFLOAT2(1.0f, 0.0f);
	}
	else if ((fx != 0.0f) && (fy != 0.0f) && (fz == 0.0f))
	{
		m_pd3dxvPositions[0] = XMFLOAT3(+fx, +fy, 0.0f); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
		m_pd3dxvPositions[1] = XMFLOAT3(+fx, -fy, 0.0f); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
		m_pd3dxvPositions[2] = XMFLOAT3(-fx, -fy, 0.0f); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);
		m_pd3dxvPositions[3] = XMFLOAT3(-fx, -fy, 0.0f); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);
		m_pd3dxvPositions[4] = XMFLOAT3(-fx, +fy, 0.0f); pd3dxvTexCoords[4] = XMFLOAT2(0.0f, 0.0f);
		m_pd3dxvPositions[5] = XMFLOAT3(+fx, +fy, 0.0f); pd3dxvTexCoords[5] = XMFLOAT2(1.0f, 0.0f);
	}
	else
	{
		m_pd3dxvPositions[0] = XMFLOAT3(0.0f, +fy, -fz); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
		m_pd3dxvPositions[1] = XMFLOAT3(0.0f, -fy, -fz); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
		m_pd3dxvPositions[2] = XMFLOAT3(0.0f, -fy, +fz); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);
		m_pd3dxvPositions[3] = XMFLOAT3(0.0f, -fy, +fz); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);
		m_pd3dxvPositions[4] = XMFLOAT3(0.0f, +fy, +fz); pd3dxvTexCoords[4] = XMFLOAT2(0.0f, 0.0f);
		m_pd3dxvPositions[5] = XMFLOAT3(0.0f, +fy, -fz); pd3dxvTexCoords[5] = XMFLOAT2(1.0f, 0.0f);
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	#ifdef _AABB_
		m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(-fx, -fy, 0);
		m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(+fx, +fy, 0);
	#else
		m_bcBoundingCube.Center = { 0.f, 0.f, 0.f };
		m_bcBoundingCube.Extents = { fx, fy, 0.0f };
	#endif
		m_fSize = XMFLOAT3(fx, fy, 0.0f);

}

CTexturedRectMesh::~CTexturedRectMesh()
{
}


//KYT '16.01.26 plus 
/*FrustumMesh Mesh*/
//------------------------------------------------------------------------------------------------
C3DFrustumMeshDiffused::C3DFrustumMeshDiffused(ID3D11Device* pd3dDevice, float fFar, float fFov) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 3;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	float yFac = tanf(fFov * PI / 360.0f);
	float xFac = yFac * ASPECT_RATIO;

	D3DXVECTOR3 Forward, Right, Up; //Simply the three columns from your transformation matrix (or the inverse of your view matrix)
	Right = D3DXVECTOR3(1, 0, 0);
	Up = D3DXVECTOR3(0, 1, 0);
	Forward = D3DXVECTOR3(0, 0, 1);


	//D3DXVECTOR3 up, right, look;
	//look = 방향벡터;
	//up = (0, 1, 0);
	//
	//Right = 외적(look, up);
	//
	//D3DXMATRIX ouput;
	//D3DXMatrixRotationAxis(&ouput, &up, 60.0f);
	//ouput = LOCAL * output;

	D3DXVECTOR3 farLeftTop = Forward*fFar - Right*xFac*fFar + Up*yFac*fFar;
	D3DXVECTOR3 farRightTop = Forward*fFar + Right*xFac*fFar + Up*yFac*fFar;
	D3DXVECTOR3 farLeftBottom = Forward*fFar - Right*xFac*fFar - Up*yFac*fFar;
	D3DXVECTOR3 farRightBottom = Forward*fFar + Right*xFac*fFar - Up*yFac*fFar;

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	m_pd3dxvPositions[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pd3dxvPositions[1] = XMFLOAT3(farLeftTop.x, farLeftTop.y, farLeftTop.z);
	m_pd3dxvPositions[2] = XMFLOAT3(farRightTop.x, farRightTop.y, farRightTop.z);

	pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);


	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_nIndices = 20;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 0;
	m_pnIndices[1] = 1;

	m_pnIndices[2] = 0;
	m_pnIndices[3] = 3;

	m_pnIndices[4] = 0;
	m_pnIndices[5] = 2;

	m_pnIndices[6] = 0;
	m_pnIndices[7] = 4;

	m_pnIndices[8] = 1;
	m_pnIndices[9] = 2;

	m_pnIndices[10] = 2;
	m_pnIndices[11] = 4;

	m_pnIndices[12] = 4;
	m_pnIndices[13] = 3;

	m_pnIndices[14] = 1;
	m_pnIndices[15] = 3;

	m_pnIndices[16] = 2;
	m_pnIndices[17] = 3;

	m_pnIndices[18] = 1;
	m_pnIndices[19] = 4;

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	m_bcBoundingCube = AABB();

	m_bcBoundingCube.m_d3dxvMinimum = XMFLOAT3(farLeftBottom.x, farLeftBottom.y, 0);
	m_bcBoundingCube.m_d3dxvMaximum = XMFLOAT3(farRightTop.x, farRightTop.y, fFar);
	m_boundingbox.Center =
		XMFLOAT3
		(
		(m_bcBoundingCube.m_d3dxvMaximum.x + m_bcBoundingCube.m_d3dxvMinimum.x) * 0.5f,
		(m_bcBoundingCube.m_d3dxvMaximum.y + m_bcBoundingCube.m_d3dxvMinimum.y) * 0.5f,
		(m_bcBoundingCube.m_d3dxvMaximum.z + m_bcBoundingCube.m_d3dxvMinimum.z) * 0.5f
		);

	m_boundingbox.Extents =
		XMFLOAT3
		(
		(m_bcBoundingCube.m_d3dxvMaximum.x - m_bcBoundingCube.m_d3dxvMinimum.x) * 0.5f,
		(m_bcBoundingCube.m_d3dxvMaximum.y - m_bcBoundingCube.m_d3dxvMinimum.y) * 0.5f,
		(m_bcBoundingCube.m_d3dxvMaximum.z - m_bcBoundingCube.m_d3dxvMinimum.z) * 0.5f
		);

	//#else
	//	m_bcBoundingCube.Center = { 0.f, 0.f, 0.f };
	//	m_bcBoundingCube.Extents = { fx, fy, 0.0f };
	//	m_fSize = XMFLOAT3(fx, fy, 0.0f);
	//#endif
}

C3DFrustumMeshDiffused::~C3DFrustumMeshDiffused()
{
}
//------------------------------------------------------------------------------------------------
