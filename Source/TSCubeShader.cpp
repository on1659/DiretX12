#include "stdafx.h"
#include "TSCubeShader.h"
#include "TSCubeMesh.h"

CTSCubeShader::CTSCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects)
{

}

CTSCubeShader::CTSCubeShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{
}

CTSCubeShader::CTSCubeShader(int nObjects)
{
}

CTSCubeShader::~CTSCubeShader()
{
}

void CTSCubeShader::CreateShader(ID3D11Device *pd3dDevice)
{
	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());

	bCubeSizeSetting = true;
	m_pd3dcbSize = nullptr;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(fSize);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pd3dcbSize);

	UINT nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
			  // { "SIZE", 0, DXGI_FORMAT_R32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot,   0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	//CreateVertexShaderFromFile(pd3dDevice, TEXT("NormalnBumpCube.fx"), "VS_TS", "vs_5_0", &m_pd3dVertexShader,d3dInputElements, nElements, &m_pd3dVertexLayout);
	//  CreateHullShaderFromFile(pd3dDevice, TEXT("NormalnBumpCube.fx"), "HS_TS", "hs_5_0", &m_pd3dHullShader);
	//CreateDomainShaderFromFile(pd3dDevice, TEXT("NormalnBumpCube.fx"), "DS_TS", "ds_5_0", &m_pd3dDomainShader);
	//
	//
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"NormalnBumpCube.fx", "PS_TS_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"NormalnBumpCube.fx", "PS_TS", "ps_5_0", &m_pd3dPixelShader);
	//#endif

	CreateVertexShaderFromCompiledFile(pd3dDevice, TEXT("../Assets/07.Shader/NormalBump/NormalBumpVS_LimLighting.cso"), d3dInputElements, nElements, &m_pd3dVertexShader);
	CreateDomainShaderFromCompiledFile(pd3dDevice, TEXT("../Assets/07.Shader/NormalBump/NormalBumpDS_LimLighting.cso"), &m_pd3dDomainShader);
	  CreateHullShaderFromCompiledFile(pd3dDevice, TEXT("../Assets/07.Shader/NormalBump/NormalBumpHS_LimLighting.cso"),   &m_pd3dHullShader);
	 CreatePixelShaderFromCompiledFile(pd3dDevice, TEXT("../Assets/07.Shader/NormalBump/NormalBumpPS_LimLighting.cso"),  &m_pd3dPixelShader);


}

void CTSCubeShader::CreateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbSize, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	fSize *size = (fSize*)d3dMappedResource.pData;
	size->size = XMFLOAT4(m_f3BumpScale.x, m_f3BumpScale.y, m_f3BumpScale.z, 1.0f);
	pd3dDeviceContext->Unmap(m_pd3dcbSize, 0);
	pd3dDeviceContext->DSSetConstantBuffers(DS_CB_SLOT_BUMP, 1, &m_pd3dcbSize);
	CTexture::UpdateDomainConstBuffer(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));
}

void CTSCubeShader::AnimateObjects(float fTimeElapsed)
{
	static XMFLOAT3 d3dxOffsetTS(0.0f, 0.0f, 0.0f);
	d3dxOffsetTS.y += 0.05f * 0.01f;
	d3dxOffsetTS.x = 0.0025f * sinf(8.0f * d3dxOffsetTS.y);
	m_d3dxmtxTexture._41 += d3dxOffsetTS.x;
	m_d3dxmtxTexture._42 += d3dxOffsetTS.y;

	if (m_d3dxmtxTexture._41 > 1.0f)m_d3dxmtxTexture._41 = 0.0f;
	if (m_d3dxmtxTexture._42 > 1.0f)m_d3dxmtxTexture._42 = 0.0f;

	CInstancedObjectsShader::AnimateObjects(fTimeElapsed);
}

void CTSCubeShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	//KYT '16.02.29
	/*
	 cuz. 이렇게 하면 초기화 할 때 MRT를 신경쓰지 않아도 된다.
	*/
	if (bCubeSizeSetting)
	{
		CTSCubeShader::CreateConstBuffer(pd3dDeviceContext);
		bCubeSizeSetting = false;

		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

		//KYT '16.01.25 minus
		/*
		Shader에서 Object Vector 가지고 있는걸 포기한다.
		고로 Render도 변해야 한다.
		*/
		for (auto &pObject : m_vRenderObject)
		{
			pd3dxmtxInstances[0] = XMMatrixTranspose(pObject->GetWorldMatrix());
		}
		pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);
		//CTexture::UpdateDomainConstBuffer(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));
		return;
	}

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;
	for (auto &pObject : m_vRenderObject)
	{
		pd3dxmtxInstances[0] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);
	//CTexture::UpdateDomainConstBuffer(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));

	//CTexture::UpdateDomainConstBuffer(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));
	
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);
	
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	
	m_pMesh->RenderInstanced(pd3dDeviceContext, 1, 0);
	
	
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(nullptr);

	
	
	
	
	
	
	
	
	
	
	//CInstancedObjectsShader::Render(pd3dDeviceContext, pCamera);
}

//void CTSCubeShader::RenderTerrain(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
//{
//	CTSCubeShader::Render(pd3dDeviceContext, pCamera);
//}
