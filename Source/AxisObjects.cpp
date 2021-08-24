#include "stdafx.h"
#include "AxisObjects.h"

CAxisObjects::CAxisObjects(CGameObject* pOwner) : m_pOwnerObject(pOwner)
{
}

CAxisObjects::~CAxisObjects()
{
	SAFE_DELETE (m_pWorldAxisObject);
}

void CAxisObjects::CreateAxis(ID3D11Device *pd3dDevice)
{
	// World Center Axis
	auto pWorldCenterX = std::make_unique<CLineMesh>(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(100, 0, 0), XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
	auto pWorldCenterY = std::make_unique<CLineMesh>(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 100, 0), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	auto pWorldCenterZ = std::make_unique<CLineMesh>(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 100), XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));

	m_pWorldAxisObject = new CGameObject;
	m_pWorldAxisObject->SetMesh(std::move(pWorldCenterX), 0);
	m_pWorldAxisObject->SetMesh(std::move(pWorldCenterY), 1);
	m_pWorldAxisObject->SetMesh(std::move(pWorldCenterZ), 2);

	m_sShader = std::make_shared<CShader>();
	m_sShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT);

}

void CAxisObjects::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	m_sShader->Render(pd3dDeviceContext, pCamera);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	XMStoreFloat4x4(&pcbWorldMatrix->m_d3dxmtxWorld, XMMatrixIdentity());
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);

	m_pWorldAxisObject->RenderMesh(pd3dDeviceContext, pCamera);
}