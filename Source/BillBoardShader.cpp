#include "stdafx.h"
#include "BillBoardShader.h"

CBillBoardShader::CBillBoardShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CMatrixShader(move(pMesh), nObjects)
{
	m_bInit = false;
}

CBillBoardShader::CBillBoardShader(std::unique_ptr<CMesh> pMesh) : CMatrixShader(move(pMesh))
{

}

CBillBoardShader::CBillBoardShader(int nObjects) : CMatrixShader(nObjects)
{

}

CBillBoardShader::~CBillBoardShader()
{
}

void CBillBoardShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CInstancedObjectsShader::BuildObjects(pd3dDevice, pContext);
	Object::SetBlendState("Billboard");
	Object::CreateRasterizerState(pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
}

void CBillBoardShader::CreateShader(ID3D11Device *pd3dDevice)
{
	int nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//2
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/BillBoard/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/BillBoard/PS.fxo", &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"BillBoard.fx", "VS_BillBoardColor", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"BillBoard.fx", "PS_BillBoardColor_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"BillBoard.fx", "PS_BillBoardColor", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CBillBoardShader::CreatePixcelShader(ID3D11Device * pd3dDevice, WCHAR * path)
{
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	m_pd3dPixelShader = nullptr;
	CreatePixelShaderFromCompiledFile(pd3dDevice, path, &m_pd3dPixelShader);
}

void CBillBoardShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	//KYT '16.02.27
	/*이거때문에 낫 공간분할*/
	for (auto &mtxWorld : m_vMtxWorld)
	{
		CBillBoardShader::SetLookAt(mtxWorld->matrix, pCamera->GetPositionXMFLOAT3());
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(mtxWorld->matrix);

	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);
		
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
		m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

	m_vMtxWorld.clear();
}

void CBillBoardShader::RenderNoClear(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;


	for (auto &mtxWorld : m_vMtxWorld)
	{
		CBillBoardShader::SetLookAt(mtxWorld->matrix, pCamera->GetPositionXMFLOAT3());
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(mtxWorld->matrix);

	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	CONTENTMGR->DebugginBlendingState(pd3dDeviceContext);
	//CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

}

void CBillBoardShader::SetLookAt(XMMATRIX &xmMatrix, XMFLOAT3 d3dxvTarget)
{

	XMFLOAT4X4 matirx;
	XMStoreFloat4x4(&matirx, xmMatrix);

	XMVECTOR d3dxvUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR d3dxvPosition = XMVectorSet(matirx._41, matirx._42, matirx._43, 1.0f);
	XMVECTOR d3dxvLook = XMVector3Normalize(d3dxvPosition - XMLoadFloat3(&d3dxvTarget));

	xmMatrix.r[0] = XMVector3Normalize(XMVector3Cross(d3dxvUp, d3dxvLook));
	xmMatrix.r[1] = d3dxvUp;
	xmMatrix.r[2] = d3dxvLook;
	xmMatrix.r[3] = d3dxvPosition;

}

void CBillBoardShader::Blend(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_bInit)return;
	m_bInit = true;
}