#include "stdafx.h"
#include "MatrixShader.h"
#include "stdafx.h"
#include "BillBoardShader.h"
#include "Object.h"

CMatrixShader::CMatrixShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{
	m_vMtxWorld.clear();
}

CMatrixShader::CMatrixShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{
	m_vMtxWorld.clear();
}

CMatrixShader::CMatrixShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
	m_vMtxWorld.clear();
}

CMatrixShader::~CMatrixShader()
{
}

void CMatrixShader::ReleaseObjects()
{
	for (auto &matrix : m_vMtxWorld)matrix->Release();
	for (auto &matrix : m_vRendermtxWorld)matrix->Release();
}

void CMatrixShader::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	for (auto &mtxWorld : m_vRendermtxWorld)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(mtxWorld->matrix);

	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);


	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	m_pMesh->RenderInstancedWithBlending(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);
	m_vRendermtxWorld.clear();
}

void CMatrixShader::SetPosition(float x, float y, float z)
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = x;
	matrix._42 = y;
	matrix._43 = z;
	std::shared_ptr<WorldMatrix> wMtx = std::make_shared<WorldMatrix>();
	wMtx->matrix = XMLoadFloat4x4(&matrix);
	m_vMtxWorld.push_back(wMtx);
}

void CMatrixShader::SetPosition(XMFLOAT3 position)
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = position.x;
	matrix._42 = position.y;
	matrix._43 = position.z;
	std::shared_ptr<WorldMatrix> wMtx = std::make_shared<WorldMatrix>();
	wMtx->matrix = XMLoadFloat4x4(&matrix);
	m_vMtxWorld.push_back(wMtx);
}
