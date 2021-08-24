#include "stdafx.h"
#include "BillBoardDisappearShader.h"

CBillBoardDisappearShader::CBillBoardDisappearShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CBillBoardShader(move(pMesh), nObjects)
{
	m_bInit = false;
	m_fAlpha = 1.0f;
}

CBillBoardDisappearShader::CBillBoardDisappearShader(std::unique_ptr<CMesh> pMesh) : CBillBoardShader(move(pMesh))
{
	m_fAlpha = 1.0f;
}

CBillBoardDisappearShader::CBillBoardDisappearShader(int nObjects) : CBillBoardShader(nObjects)
{

}

CBillBoardDisappearShader::~CBillBoardDisappearShader()
{
}


void CBillBoardDisappearShader::CreateConstBuffer(ID3D11Device * pd3dDevice)
{

	m_fAlpha = 1.0f;
	m_fSpeed = -0.01f;


	cbBillBoaredAlpha* alpha = new cbBillBoaredAlpha();
	alpha->alpha = m_fAlpha;

	//Create Bulr Const Buffer 
	D3D11_BUFFER_DESC pd3dDesc;
	ZeroMemory(&pd3dDesc, sizeof(pd3dDesc));
	pd3dDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDesc.ByteWidth = sizeof(cbBillBoaredAlpha);
	pd3dDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pd3dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dFogBufferData;
	ZeroMemory(&d3dFogBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dFogBufferData.pSysMem = alpha;
	std::cout << sizeof(cbBillBoaredAlpha) << std::endl;
	pd3dDevice->CreateBuffer(&pd3dDesc, &d3dFogBufferData, &m_pd3dcbBillBoardAlpha);

	if (alpha)delete alpha;
	alpha = nullptr;
}

void CBillBoardDisappearShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{

	if (m_fAlpha < 0)m_fAlpha = 1.0f;
	m_fAlpha += m_fSpeed;
#ifdef DEBUG_MODE_UI
	FONTMGR->MessagePush("기본", 30, 280, 20, YT_Color(0, 0, 0), L"(Alpah : %1.3f)", m_fAlpha);
#endif
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbBillBoardAlpha, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	cbBillBoaredAlpha *alpha = (cbBillBoaredAlpha*)d3dMappedResource.pData;
	alpha->alpha = m_fAlpha;
	pd3dDeviceContext->Unmap(m_pd3dcbBillBoardAlpha, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_SPRITE_ANIMATION, 1, &m_pd3dcbBillBoardAlpha);

}


void CBillBoardDisappearShader::CreateShader(ID3D11Device *pd3dDevice)
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


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/BillBoard/BillBoardAlphaVS.cso", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/BillBoard/BillBoardAlphaPS.cso", &m_pd3dPixelShader);

}



void CBillBoardDisappearShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
#ifdef DEBUG_MODE_UI
	FONTMGR->MessagePush("카툰", 20, 50, 15,YT_Color(255,0,0), L"size : %d", m_lMtxWorld.size());
#endif
	if (m_lMtxWorld.size() == 0)return;

	//CBillBoardDisappearShader::UpdateConstBuffer(pd3dDeviceContext);

	OnPrepareRender(pd3dDeviceContext);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstances = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	//KYT '16.02.27
	/*이거때문에 낫 공간분할*/
	for (auto &mtxWorld : m_lMtxWorld)
	{

		CBillBoardShader::SetLookAt(mtxWorld->matrix, pCamera->GetPositionXMFLOAT3());

		
		XMFLOAT4X4 mtx;
		XMStoreFloat4x4(&mtx, mtxWorld->matrix);
		auto camPos = pCamera->GetLookVectorXMFLOAT3();
		mtx._41 -= camPos.x;
		mtx._43 -= camPos.z;

		XMStoreFloat4x4(&m_mtxUpdateMatrix, mtxWorld->matrix);

		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(XMLoadFloat4x4(&mtx));
		m_mtxUpdateMatrix._42 += 0.01f;

		if (m_mtxUpdateMatrix._42 > 2.3f)
			mtxWorld->life = false;

		mtxWorld->matrix  = XMLoadFloat4x4(&m_mtxUpdateMatrix);
	}

	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	//CONTENTMGR->DebugginBlendingState(pd3dDeviceContext);
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	if (m_pd3dRasterizerState)pd3dDeviceContext->RSSetState(nullptr);

	m_lMtxWorld.remove_if
	(
		[](auto matrix)
		{
			return !matrix->life;
		}
	);
	
}

void CBillBoardDisappearShader::SetPosition(float x, float y, float z)
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	matrix._41 = x;
	matrix._42 = y;
	matrix._43 = z;
	std::shared_ptr<WorldMatrix> wMtx = std::make_shared<WorldMatrix>();
	wMtx->matrix = XMLoadFloat4x4(&matrix);
	m_lMtxWorld.push_back(wMtx);

}

void CBillBoardDisappearShader::SetLookAt(XMMATRIX &xmMatrix, XMFLOAT3 d3dxvTarget, XMFLOAT3 look)
{

	XMFLOAT4X4 matirx;
	XMStoreFloat4x4(&matirx, xmMatrix);

	XMVECTOR d3dxvUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR d3dxvPosition = XMVectorSet(matirx._41, matirx._42, matirx._43, 1.0f);
	XMVECTOR d3dxvLook = XMVector3Normalize(d3dxvPosition - XMLoadFloat3(&d3dxvTarget));

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, d3dxvPosition);
	pos.x += look.x;
	pos.z += look.z;
	
	xmMatrix.r[0] = XMVector3Normalize(XMVector3Cross(d3dxvUp, d3dxvLook));
	xmMatrix.r[1] = d3dxvUp;
	xmMatrix.r[2] = d3dxvLook;
	xmMatrix.r[3] = XMLoadFloat3(&pos);

}