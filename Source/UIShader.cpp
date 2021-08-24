#include "stdafx.h"
#include "UIShader.h"
#include "Component.h"
#include "UIMesh.h"
#include "UIState.h"
#include "UIEatItem.h"

//-------------------------------------------------------직교투영 JJY

CUIShader::CUIShader(int nObjects) : CInstancedObjectsShader(nObjects)
{
	m_bInitallyShader = true;
	m_bVisible = true;

	m_uUIPosition.gameObject = std::make_shared<CGameObject>();
	m_uUIPosition.gameObject->SetCompoent(std::make_unique<UIState>());

	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

CUIShader::~CUIShader()
{

}

void CUIShader::ReleseInstance()
{
	m_uUIPosition.gameObject->Release();
}

void CUIShader::CreateShader(ID3D11Device *pd3dDevice)
{
	UINT nSlot = 0;

	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },									//0
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },										//2
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/UI/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/UI/PS.fxo", &m_pd3dPixelShader);

	//CreateVertexShaderFromFile(pd3dDevice, L"UI.fx", "VSInstancedTexturedColorUI", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"UI.fx", "PSInstancedTexturedColorUI_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"UI.fx", "PSInstancedTexturedColorUI", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CUIShader::CreateAfterShader(ID3D11Device * pd3dDevice, bool isGaugeBar)
{
	if (m_pd3dPixelShader)m_pd3dPixelShader->Release();
	m_pd3dPixelShader = nullptr;

	if(isGaugeBar)
		CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/UI/Gauge/PSGauge.fxo", &m_pd3dPixelShader);
	else
		CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/UI/Gauge/PSGaugeBackground.fxo", &m_pd3dPixelShader);


}

void CUIShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{

	if (false == m_bVisible) return;

	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	if (m_bInitallyShader)UpdateConstBuffer(pd3dDeviceContext);

	m_pMesh->RenderInstanced(pd3dDeviceContext, 1, 0);

}

void CUIShader::RenderTransparent(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{

	if (false == m_bVisible) return;

	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	if (m_bInitallyShader)UpdateConstBuffer(pd3dDeviceContext);

	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, 1, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

}


void CUIShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = static_cast<XMMATRIX*>(d3dMappedResource.pData);
	pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(m_uUIPosition.gameObject->GetWorldMatrix());
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);
	m_bInitallyShader = false;
}


void CUIShader::SetUIShader(std::shared_ptr<CUIShader> shader)
{
	m_uUIPosition.gameObject->SetShader(shader);
}

void CUIShader::ReSetPosition(RECT rect)
{
	float cx, cy, width, height;

	width = (float)(rect.right - rect.left);
	height = (float)(rect.bottom - rect.top);

	cx = rect.left + (width * 0.5f);
	cy = rect.top + (height * 0.5f);


	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = cx - (FRAME_BUFFER_WIDTH * 0.5f);
	matrix._42 = (cy - (FRAME_BUFFER_HEIGHT * 0.5f)) * -1.0f;
	matrix._43 = 0;
	m_uUIPosition.gameObject->SetWorldMatrix(matrix);

	m_uUIPosition.rect = rect;
	m_bInitallyShader = true;


	m_fWidth = width;
	m_fHeight = height;
}

void CUIShader::SetPosition(RECT rect)
{
	float cx, cy, width, height;

	width = (rect.right - rect.left);
	height = (rect.bottom - rect.top);

	cx = rect.left + (width * 0.5f);
	cy = rect.top + (height * 0.5f);


	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = cx - (FRAME_BUFFER_WIDTH * 0.5f);
	matrix._42 = (cy - (FRAME_BUFFER_HEIGHT * 0.5f)) * -1.0f;
	matrix._43 = 0;
	m_uUIPosition.gameObject->SetWorldMatrix(matrix);

	m_uUIPosition.rect = rect;
	m_bInitallyShader = true;


	m_fWidth = width;
	m_fHeight = height;
}

void CUIShader::SetPosition(float cx, float cy, float width, float height)
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = (cx - (FRAME_BUFFER_WIDTH * 0.5f));
	matrix._42 = (cy - (FRAME_BUFFER_HEIGHT * 0.5f)) * -1.0f;
	matrix._43 = 0;
	m_uUIPosition.gameObject->SetWorldMatrix(matrix);

	RECT rect;
	rect.left = (long)(cx - (width * 0.5f));
	rect.right = (long)(cx + (width * 0.5f));
	rect.top = (long)(cy - (height * 0.5f));
	rect.bottom = (long)(cy + (height * 0.5f));

	m_uUIPosition.rect = rect;
	m_bInitallyShader = true;

	m_fWidth = width;
	m_fHeight = height;

}

void CUIShader::SetPosition(float cx, float cy)
{
	if (m_pMesh)  m_fWidth = m_pMesh->GetMeshSize().x;
	if (m_pMesh) m_fHeight = m_pMesh->GetMeshSize().y;

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = (cx - (FRAME_BUFFER_WIDTH * 0.5f));
	matrix._42 = (cy - (FRAME_BUFFER_HEIGHT * 0.5f)) * -1.0f;
	matrix._43 = 0;
	m_uUIPosition.gameObject->SetWorldMatrix(matrix);


	RECT rect;
	rect.left = (long)(cx - (m_fWidth * 0.5f));
	rect.right = (long)(cx + (m_fWidth * 0.5f));
	rect.top = (long)(cy - (m_fHeight * 0.5f));
	rect.bottom = (long)(cy + (m_fHeight * 0.5f));

	m_uUIPosition.rect = rect;
	m_bInitallyShader = true;
}

bool CUIShader::Contain()
{
	return Contain(INPUT->GetMousePoint());
}

bool CUIShader::Contain(POINT pt)
{
	return	m_uUIPosition.rect.left < m_uUIPosition.rect.right && m_uUIPosition.rect.top < m_uUIPosition.rect.bottom && pt.x >= m_uUIPosition.rect.left &&
		pt.x < m_uUIPosition.rect.right && pt.y >= m_uUIPosition.rect.top && pt.y < m_uUIPosition.rect.bottom;
}

bool CUIShader::Contain(int x, int y)
{
	return m_uUIPosition.rect.left < m_uUIPosition.rect.right && m_uUIPosition.rect.top < m_uUIPosition.rect.bottom
		&& x >= m_uUIPosition.rect.left && x < m_uUIPosition.rect.right && y >= m_uUIPosition.rect.top && y < m_uUIPosition.rect.bottom;
}

bool CUIShader::MouseLeftClick()
{
	return (Contain(INPUT->GetMousePoint()) & INPUT->MouseLeftDown());
}

bool CUIShader::MouseRightClick()
{
	return (Contain(INPUT->GetMousePoint()) & INPUT->MouseRightDown());
}

void CUIShader::Animate(float fTimeElapsed)
{
	m_uUIPosition.gameObject->Animate(fTimeElapsed);
}

void CUIShader::SetComponet(std::unique_ptr<Component> component)
{
	if (nullptr == component)return;
	if (nullptr == m_uUIPosition.gameObject) return;
	m_uUIPosition.gameObject->SetCompoent(move(component));
}

void CUIShader::OffSetSize(ID3D11DeviceContext * pd3dDeviceContext, float x, float y, float z)
{
	if (m_pMesh)m_pMesh->OffSetSize(pd3dDeviceContext, x, y, z);
}

void CUIShader::SetSize(ID3D11DeviceContext * pd3dDeviceContext, float x, float y, float z)
{
	if (m_pMesh)m_pMesh->SetSize(pd3dDeviceContext, x, y, z);
}


void CUIShader::Start()
{
	m_uUIPosition.gameObject->StartComponet(m_uUIPosition.gameObject);

}
XMFLOAT4 CUIShader::GetPosXMFLOAT4()
{
	XMFLOAT4 f;
	f.x = m_uUIPosition.gameObject->m_d3dxmtxWorld._41 + (FRAME_BUFFER_WIDTH * 0.5f);
	f.y = -m_uUIPosition.gameObject->m_d3dxmtxWorld._42 + (FRAME_BUFFER_HEIGHT * 0.5f);
	f.w = m_fWidth;
	f.z = m_fHeight;
	return f;
}
/*
vector<CUldokim> vDoULee;
vDoLee.push_back(CUIdokim(pd3dDevice, "코딩","C++","난디졌다...",L"죄송합니다");

for(auto &teaWoo : vDoULee)
teaWoo->Render(pd3dDeviceContext);
*/