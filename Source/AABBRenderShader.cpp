#include "stdafx.h"
#include "AABBRenderShader.h"
#include "Mesh.h"
//---------------------------------------------------------------- Instance
CAABBRenderShader::CAABBRenderShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CGeoMetryInstanceShader(move(pMesh), nObjects)
{

}

CAABBRenderShader::CAABBRenderShader(std::unique_ptr<CMesh> pMesh) : CGeoMetryInstanceShader(move(pMesh))
{

}

CAABBRenderShader::CAABBRenderShader(int nObjects) : CGeoMetryInstanceShader(nObjects)
{
}

CAABBRenderShader ::~CAABBRenderShader()
{
}

void CAABBRenderShader::CreateShader(ID3D11Device *pd3dDevice)
{
	//WireFrame
	Object::CreateRasterizerState(pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);

	m_pd3dVSBuffer = nullptr;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(cbAABB);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pd3dVSBuffer);


	int nSlot = 0;
	//GS를 사용하는 큐브는 size와 디폴트 색상을 가진다.
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }

	};

	UINT nElements = ARRAYSIZE(d3dInputElements);


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/AABB/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreateGeoMoetryShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/AABB/GS.fxo", &m_pd3dGeometryShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/AABB/PS.fxo", &m_pd3dPixelShader);



	//CreateVertexShaderFromFile(pd3dDevice, L"GS_AABB.fx", "VS_GS_AABB_Cube", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//CreateGeometryShaderFromFile(pd3dDevice, L"GS_AABB.fx", "GS_GS_AABB", "gs_5_0", &m_pd3dGeometryShader);
	//
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"GS_AABB.fx", "PS_GS_AABB_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"GS_AABB.fx", "PS_GS_AABB", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CAABBRenderShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext,XMFLOAT3 fSize)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dVSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	cbAABB *pd3dcbAABB = (cbAABB *)d3dMappedResource.pData;
	pd3dcbAABB->m_fSize = fSize;
	pd3dDeviceContext->Unmap(m_pd3dVSBuffer, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_AABB, 1, &m_pd3dVSBuffer);
}

void CAABBRenderShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, AABB aabb, std::vector<std::shared_ptr<CGameObject>> vObject)
{
	//Render Clear X
	XMFLOAT3 fSize;
	fSize.x = (aabb.m_d3dxvMaximum.x - aabb.m_d3dxvMinimum.x) * 0.5f;
	fSize.y = (aabb.m_d3dxvMaximum.y - aabb.m_d3dxvMinimum.y) * 0.5f;
	fSize.z = (aabb.m_d3dxvMaximum.z - aabb.m_d3dxvMinimum.z) * 0.5f;

	CAABBRenderShader::UpdateConstBuffer(pd3dDeviceContext, fSize);

	CInstancedObjectsShader::OnPrepareRender(pd3dDeviceContext);
	
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;
	
	for (auto &pObject : vObject) pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());

	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);


	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	 
	//Render
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	//Render
	
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

}

void CAABBRenderShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, BoundingBox boundingbox, std::vector<std::shared_ptr<CGameObject>> vObject)
{
	if (vObject.size() > 0)
	{
		if (vObject[0]->name == "Player")
		{
			::ConsoleShow(boundingbox.Extents, "Player AABB Extends : ");
			::ConsoleShow(vObject[0]->GetBoundingBox().Extents, "Player OBJ  Extends : ");
		}
	}

	//Render Clear X
	XMFLOAT3 fSize = boundingbox.Extents;

	CAABBRenderShader::UpdateConstBuffer(pd3dDeviceContext, fSize);

	CInstancedObjectsShader::OnPrepareRender(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	for (auto &pObject : vObject)
	{
		if (nInstances > 9995)break;
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);


	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	//Render
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	//Render

	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

}

void CAABBRenderShader::Render(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, BoundingBox boundingbox, std::shared_ptr<CGameObject> pObject)
{
	//Render Clear X
	XMFLOAT3 fSize = boundingbox.Extents;
	::ConsoleShow(fSize, "fSize : ");
	std::cout << std::endl;
	CAABBRenderShader::UpdateConstBuffer(pd3dDeviceContext, fSize);

	CInstancedObjectsShader::OnPrepareRender(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pObject->SetPosition(boundingbox.Center);

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	if (nInstances < 9995) pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());

	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);


	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	//Render
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	//Render

	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);
}

void CAABBRenderShader::Render(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, std::vector<BoundingBox> vBoundingbox, std::shared_ptr<CGameObject> pObject)
{
	//Render Clear X
	std::wstring wstr;
	wstr = L"fSize\n";
	for (auto &boundingbox : vBoundingbox)
	{
		XMFLOAT3 fSize = boundingbox.Extents;

		CAABBRenderShader::UpdateConstBuffer(pd3dDeviceContext, fSize);
		
		wstr += L"(" + std::to_wstring(boundingbox.Center.x) + L"," + std::to_wstring(boundingbox.Center.y) + L"," + std::to_wstring(boundingbox.Center.z) + L")\n";
		wstr += L"(" + std::to_wstring(boundingbox.Extents.x) + L"," + std::to_wstring(boundingbox.Extents.y) + L"," + std::to_wstring(boundingbox.Extents.z) + L")\n";
		wstr += L"----------------------------\n";

		CInstancedObjectsShader::OnPrepareRender(pd3dDeviceContext);

		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

		pObject->SetPosition(boundingbox.Center);

		int nInstances = 0;
		pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;
		if (nInstances < 9995) pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
		pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

		CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
		if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

		//Render
		m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
		//Render

		pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);
	}
	//FONTMGR->MessagePush("기본", wstr, 655, 150, 25, YT_Color(0, 255, 255));
}



void CAABBRenderShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera, std::vector<std::shared_ptr<CGameObject>> vObject)
{
	//Render Clear X
	CInstancedObjectsShader::OnPrepareRender(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX*)d3dMappedResource.pData;

	for (auto &pObject : vObject)
	{

		//KYT '16.04.04
		/*
		 바운딩 박스에 들어가는 Size 수정
		 기존 : AABB
		 개선 : Boundingbox
		*/
		XMFLOAT3 fSize;
		auto boundbox = pObject->GetBoundingBox();
		fSize.x =boundbox.Extents.x;//(aabb.m_d3dxvMaximum.x - aabb.m_d3dxvMinimum.x) * 0.5f;
		fSize.y =boundbox.Extents.y;//(aabb.m_d3dxvMaximum.y - aabb.m_d3dxvMinimum.y) * 0.5f;
		fSize.z =boundbox.Extents.z;//(aabb.m_d3dxvMaximum.z - aabb.m_d3dxvMinimum.z) * 0.5f;

		CAABBRenderShader::UpdateConstBuffer(pd3dDeviceContext, fSize);

		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}

	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);


	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);

	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

}