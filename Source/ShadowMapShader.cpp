#include "stdafx.h"
#include "ShadowMapShader.h"

CShadowMapShader::CShadowMapShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CMatrixShader(move(pMesh), nObjects)
{
	m_nCount = 0;
	m_nMaxCount = 0;
}

CShadowMapShader::CShadowMapShader(std::unique_ptr<CMesh> pMesh) : CMatrixShader(move(pMesh))
{
	m_nCount = 0;
	m_nMaxCount = 0;
}

CShadowMapShader::CShadowMapShader(int nObjects) : CMatrixShader(nObjects)
{
	m_nCount = 0;
	m_nMaxCount = 0;
}

CShadowMapShader::~CShadowMapShader()
{
}

void CShadowMapShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CInstancedObjectsShader::BuildObjects(pd3dDevice, pContext);
	CShadowMapShader::SetBlendState("Default");
}

void CShadowMapShader::CreateShader(ID3D11Device *pd3dDevice)
{
	UINT nSlot = 0;

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
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"BillBoard.fx", "PS_BillBoardColor_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"BillBoard.fx", "PS_BillBoardColor", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CShadowMapShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	//KYT '16.02.27
	/*
		이거때문에 낫 공간분할
	*/
	for (auto &mtxWorld : m_vMtxWorld)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(mtxWorld->matrix);
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
		m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

	m_nCount = 0;
}

void CShadowMapShader::RenderInstanced(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
}

void CShadowMapShader::SetFirstPosition(const float& x, const float& y,const float& z)
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = x;
	matrix._42 = y;
	matrix._43 = z;
	std::shared_ptr<WorldMatrix> wMtx = std::make_shared<WorldMatrix>();
	wMtx->matrix = XMLoadFloat4x4(&matrix);

	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(90),
		XMConvertToRadians(0), XMConvertToRadians(0));
	wMtx->matrix = mtxRotate * wMtx->matrix;

	m_xmDefaultMatrix = wMtx->matrix;

	m_vMtxWorld.push_back(wMtx);
}


//KYT '16.05.08
/*
 똥그라미 그림자
*/
void CShadowMapShader::SetPosition(const XMFLOAT3& position)
{

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, m_vMtxWorld[0]->matrix);
	matrix._41 = position.x;
	matrix._42 = BLOCK_SCALE * 1.01;
	matrix._43 = position.z;
	m_vMtxWorld[0]->matrix = XMLoadFloat4x4(&matrix);

	return;


	matrix._41 = position.x;
	matrix._42 = BLOCK_SCALE * 1.01;
	matrix._43 = position.z;

	m_xmDefaultMatrix.r[3] = XMLoadFloat3(&position);


	if (m_vMtxWorld.size() > m_nMaxCount)
	{
		m_vMtxWorld[m_nCount]->matrix = m_xmDefaultMatrix;
		m_nCount++;
	}
	else
	{
		std::shared_ptr<WorldMatrix> wMtx = std::make_shared<WorldMatrix>();
		wMtx->matrix = m_xmDefaultMatrix;
		m_vMtxWorld.push_back(wMtx);
		m_nMaxCount++;
	}

	//shared_ptr<WorldMatrix> world = std::make_shared<WorldMatrix>();
	//XMFLOAT4X4 matrix;
	//matrix._41 = position.x;
	//matrix._42 = BLOCK_SCALE * 1.01;
	//matrix._43 = position.z;
	//world->matrix = XMLoadFloat4x4(&matrix);
	//
	//m_vMtxWorld.push_back(world);
	//m_vMtxWorld[0]->matrix = XMLoadFloat4x4(&matrix);

}

void CShadowMapShader::SetPosition(const XMVECTOR& position)
{
	m_xmDefaultMatrix.r[4] = position;

	if (m_vMtxWorld.size() > m_nMaxCount)
	{
		m_vMtxWorld[m_nCount]->matrix = m_xmDefaultMatrix;
		m_nCount++;
	}
	else
	{
		std::shared_ptr<WorldMatrix> wMtx = std::make_shared<WorldMatrix>();
		wMtx->matrix = m_xmDefaultMatrix;
		m_vMtxWorld.push_back(wMtx);
		m_nMaxCount++;
	}
}

void CShadowMapShader::SetPosition_Ver2(const XMFLOAT3& position)
{
}




//KYT '16.05.08
/*
	CShadowMapShader::CShadowMapShader()
	{
		m_pnVertexStrides = nullptr;
		m_pnVertexOffsets = nullptr;
		m_pd3dRasterizerState = nullptr;
	}


	CShadowMapShader::~CShadowMapShader()
	{
	}

	void CShadowMapShader::CreateShader(ID3D11Device * pd3dDevice)
	{
		//화면 좌표계 4개 준다
		ShadowsMap_VS_INPUT v[4];
		v[0].d3dxPosition = XMFLOAT4(-1, -1, 0, 1);
		v[1].d3dxPosition = XMFLOAT4(-1, 1, 0, 1);
		v[2].d3dxPosition = XMFLOAT4(1, -1, 0, 1);
		v[3].d3dxPosition = XMFLOAT4(1, 1, 0, 1);

		v[0].d3dxTextCoord = XMFLOAT2(0, 1);
		v[1].d3dxTextCoord = XMFLOAT2(0, 0);
		v[2].d3dxTextCoord = XMFLOAT2(1, 1);
		v[3].d3dxTextCoord = XMFLOAT2(1, 0);

		//버텍스 버퍼 생성
		D3D11_BUFFER_DESC d3dBufferDesc;
		ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
		d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dBufferDesc.ByteWidth = sizeof(ShadowsMap_VS_INPUT) * 4;
		d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		d3dBufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA d3dBufferData;
		ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		d3dBufferData.pSysMem = v;
		pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


		m_pnVertexStrides = new UINT[1];
		m_pnVertexOffsets = new UINT[1];
		m_pnVertexStrides[0] = sizeof(ShadowsMap_VS_INPUT);
		m_pnVertexOffsets[0] = 0;

		UINT nSlot = 0;
		D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
		{
			{ "POSITION",	 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,		nSlot++, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
			{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
			{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
			{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6

		};

		UINT nElements = ARRAYSIZE(d3dInputLayout);
		CreateVertexShaderFromFile(pd3dDevice, L"Shadow.fx", "VSShadowMap", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
		CreatePixelShaderFromFile(pd3dDevice, L"Shadow.fx", "PSShadowMap", "ps_5_0", &m_pd3dPixelShader);

		//

	}

	void CShadowMapShader::Render(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
	{
		pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
		pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);

	}
*/

