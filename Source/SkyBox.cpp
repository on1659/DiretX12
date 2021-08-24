#include "stdafx.h"
#include "SkyBox.h"
#include "InstancedObjectsShader.h"

CSkyBox::CSkyBox(ID3D11Device *pd3dDevice) : CGameObject(1)
{
	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = false;
	d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	d3dDepthStencilDesc.StencilEnable = false;
	d3dDepthStencilDesc.StencilReadMask = 0xFF;
	d3dDepthStencilDesc.StencilWriteMask = 0xFF;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dDepthStencilState);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::Render(ID3D11DeviceContext *pd3dDeviceContext,std::shared_ptr<CCamera> pCamera)
{
	XMFLOAT3 f3vCameraPos = pCamera->GetPositionXMFLOAT3();
	SetPosition(f3vCameraPos.x, f3vCameraPos.y, f3vCameraPos.z);
	Update(nullptr);

	CGameObject::UpdateConstBuffer(pd3dDeviceContext, XMLoadFloat4x4(&m_d3dxmtxWorld));

	if (m_sShader) m_sShader->Render(pd3dDeviceContext, pCamera);

	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	CSkyBoxMesh *pSkyBoxMesh = (CSkyBoxMesh *)m_vsMeshes[0].get();
	pSkyBoxMesh->OnPrepareRender(pd3dDeviceContext);

	//KYT '12.02.28
	/* Souerce Delete */
	m_sMaterial->m_pTexture->UpdateConstBuffer(pd3dDeviceContext);

	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);

	for (int i = 0; i < 6; i++)
	{
		pd3dDeviceContext->DrawIndexed(4, 0, i * 4);
	}

	pd3dDeviceContext->OMSetDepthStencilState(nullptr, 1);
}
