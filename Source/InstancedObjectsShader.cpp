
#include "stdafx.h"
#include "InstancedObjectsShader.h"


//---------------------------------------------------------------------------------
CInstancedObjectsShader::CInstancedObjectsShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CObjectsShader(nObjects)
{
	m_nInstanceBufferStride = sizeof(UINT);
	m_nInstanceBufferOffset = 0;
	m_pd3dInstanceBuffer = nullptr;

	//KYT '16.01.20 plus
	/*
		Mesh 를 생성자에서 넣어준다.
	*/
	m_pMesh = move(pMesh);

	m_nType = VERTEX_INSTANCING_ELEMENT;
}

CInstancedObjectsShader::CInstancedObjectsShader(std::unique_ptr<CMesh> pMesh) : CObjectsShader(0)
{
	m_nInstanceBufferStride = sizeof(XMMATRIX);
	m_nInstanceBufferOffset = 0;
	m_pd3dInstanceBuffer = nullptr;

	//KYT '16.01.20 plus
	/*Mesh 를 생성자에서 넣어준다.*/
	m_pMesh = move(pMesh);

	m_nType = VERTEX_INSTANCING_ELEMENT;
}

CInstancedObjectsShader::CInstancedObjectsShader(int nObjects) : CObjectsShader(nObjects)
{
	m_nInstanceBufferStride = sizeof(XMMATRIX);
	m_nInstanceBufferOffset = 0;
	m_pd3dInstanceBuffer = nullptr;

	//KYT '16.01.20 plus
	/*Default Mesh를 사용한다.*/
	m_pMesh = CONTENTMGR->GetGetDefaultShaderMesh();

	m_nType = VERTEX_INSTANCING_ELEMENT;
}

CInstancedObjectsShader::~CInstancedObjectsShader()
{
}


//KYT '16.01.20 minus
void CInstancedObjectsShader::SetMesh(std::unique_ptr<CMesh> pMesh)
{
	m_pMesh.release();
	m_pMesh = move(pMesh);
}

//KYT '16.01.20 plus
/*가상함수로 모든 쉐이더마다 써줄지, 경배처럼 Type으로 하나의 Create에서 해줄지 결정*/
void CInstancedObjectsShader::CreateShader(ID3D11Device *pd3dDevice)
{
	if (m_pd3dVertexLayout)m_pd3dVertexLayout->Release();
	m_pd3dVertexLayout = nullptr;

	if (m_pd3dVertexShader)m_pd3dVertexShader->Release();
	m_pd3dVertexShader = nullptr;

	if (m_pd3dPixelShader)m_pd3dPixelShader->Release();
	m_pd3dPixelShader = nullptr;

	UINT nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,	  nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//1
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//2
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,  nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/VS.cso", d3dInputElements, nElements, &m_pd3dVertexShader);
	 CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/PS.cso", &m_pd3dPixelShader);

	//CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	//CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/PS.fxo", &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"Instanced.fx", "VSInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"Instanced.fx", "PSInstanced_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"Instanced.fx", "PSInstanced", "ps_5_0", &m_pd3dPixelShader);
	//#endif
	//KYT '16.01.18 minus
	//앞으로 사용하지 않을꺼임
	/*if (m_pMesh) CreateShader(pd3dDevice, m_pMesh->GetType());*/
}

void CInstancedObjectsShader::CreateBlendShader(ID3D11Device * pd3dDevice)
{
	UINT nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//1
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//2
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,  nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"Bush.fx", "VSInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Bush.fx", "PSInstanced", "ps_5_0", &m_pd3dPixelShader);
	//KYT '16.01.18 minus
	//앞으로 사용하지 않을꺼임
	/*if (m_pMesh) CreateShader(pd3dDevice, m_pMesh->GetType());*/

}

//KYT '16.01.18 minus
/*앞으로 사용하지 않을꺼임 Cuz. 쉐이더 검색은 수동으로 할꺼임 */
//void CInstancedObjectsShader::CreateShader(ID3D11Device *pd3dDevice, UINT nType)
//{
//	CObjectsShader::CreateShader(pd3dDevice, nType);
//}

void CInstancedObjectsShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CObjectsShader::BuildObjects(pd3dDevice, pContext);

	
	//KYT '16.01.20 plus
	/*
		오브젝트 개수를 미리 많이 잡아 둘 수 있는데 그런 경우에는 미리 버퍼의 크기를 잡아둬야한다.
	   그럼 추가한 오브젝트의 수보다 미리 잡은 오브젝트의 수가 반드시 더 크거나 같으므로 이런 코드가 들어간 것이다.
	*/
	//if (m_vsObjects.size() > m_nObjects )m_nObjects = m_vsObjects.size();

	m_pd3dInstanceBuffer = CreateBuffer(pd3dDevice, m_nInstanceBufferStride, m_nObjects, nullptr, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_pMesh->AssembleToVertexBuffer(1, &m_pd3dInstanceBuffer, &m_nInstanceBufferStride, &m_nInstanceBufferOffset);
}

void CInstancedObjectsShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();

	//if (m_pMesh) m_pMesh.reset();

	if (m_pd3dInstanceBuffer) m_pd3dInstanceBuffer->Release();
} 

void CInstancedObjectsShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	//KYT '16.01.25 minus
	/*
	Shader에서 Object Vector 가지고 있는걸 포기한다.
	고로 Render도 변해야 한다.
	*/
	for (auto &pObject : m_vRenderObject)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(nullptr);

	//KYT '16.02.05 
	/*아직 삭제하기에는 이름*/
#ifdef _SPACEPARTITIONING
	m_vRenderObject.clear();
#endif
}

void CInstancedObjectsShader::RenderNoClear(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	//KYT '16.01.25 minus
	/*
		Shader에서 Object Vector 가지고 있는걸 포기한다.
		고로 Render도 변해야 한다.
	*/
	for (auto &pObject : m_vRenderObject)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);	
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(nullptr);

}


void CInstancedObjectsShader::RenderPlayer(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	//KYT '16.01.25 minus
	/*
		Shader에서 Object Vector 가지고 있는걸 포기한다.
		고로 Render도 변해야 한다.
	*/
	//	for (auto &pObject : m_vRenderObject)
	
	pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(m_vRenderObject[0]->GetWorldMatrix());
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, 1, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(nullptr);


	if (m_vRenderObject.size() > 1)
	{
		auto obj = m_vRenderObject[0];
		m_vRenderObject.clear();
		m_vRenderObject.push_back(obj);
	}
}



//'16.03.31 KYT
/*
  지형은 무조건 그리기 때문에 map, unMap을 할 필요가 없다.
*/
void CInstancedObjectsShader::RenderTerrain(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	if (m_vRenderObject.size() > 0)
	{
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

		int nInstances = 0;
		pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

		//KYT '16.01.25 minus
		/*
		Shader에서 Object Vector 가지고 있는걸 포기한다.
		고로 Render도 변해야 한다.
		*/
		for (auto &pObject : m_vRenderObject)
		{
			pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
		}
		m_nObjects = m_vRenderObject.size();
		pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);
	}

	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, m_nObjects, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(nullptr);

	m_vRenderObject.clear();
}

  void CInstancedObjectsShader::ReSetShader()
  {
	  if (m_pd3dVertexLayout)m_pd3dVertexLayout->Release();
	  m_pd3dVertexLayout = nullptr; 
  }

