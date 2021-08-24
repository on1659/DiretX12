#include "stdafx.h"
#include "BillBoardSpriteShader.h"
#include "UISpriteMesh.h"

CBillBoardSpriteShader::CBillBoardSpriteShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CBillBoardShader(move(pMesh), nObjects)
{
	m_bInit = false;
}

CBillBoardSpriteShader::CBillBoardSpriteShader(std::unique_ptr<CMesh> pMesh) : CBillBoardShader(move(pMesh))
{

}

CBillBoardSpriteShader::CBillBoardSpriteShader(int nObjects) : CBillBoardShader(nObjects)
{

}

CBillBoardSpriteShader::~CBillBoardSpriteShader()
{
}

void CBillBoardSpriteShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CInstancedObjectsShader::BuildObjects(pd3dDevice, pContext);
	Object::SetBlendState("BillBoardSprite");
	Object::CreateRasterizerState(pd3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);
}

void CBillBoardSpriteShader::CreateShader(ID3D11Device *pd3dDevice)
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


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/BillBoard/BillBoardSpriteVS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/BillBoard/BillBoardSpritePS.fxo", &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"BillBoard.fx", "VS_BillBoardColor", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"BillBoard.fx", "PS_BillBoardColor_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"BillBoard.fx", "PS_BillBoardColor", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

void CBillBoardSpriteShader::Initialize(ID3D11Device *pd3dDevice, std::string path, XMFLOAT3 position, XMFLOAT2 fSize, float frameFPS, int frameCount, int loop)
{
	m_pMesh = std::make_unique<CUISpriteMesh>(pd3dDevice, fSize.x, fSize.y);
	SetMaterial(pd3dDevice, path);

	CInstancedObjectsShader::BuildObjects(pd3dDevice);
	Object::SetBlendState("BillBoardSprite");


	CMatrixShader::SetPosition(position);

	m_frameFPS = 1 / frameFPS;			// 증가량
	m_frameCount = frameCount;			// 몇장이니
	m_frameWidth = 1.0f / frameCount;	// 증가값
	m_fFPSCoount = 0;


	m_frameLeft = 0.0f;					// 최초 시작위치
	m_imageWidth = 1.0f / frameCount;	// 이미지 한장당 크기

	m_nCurrentFrame = 0;
	m_fFPSCoount = 0.0f;

	m_nLoop = loop;
	CBillBoardSpriteShader::CreateConstBuffer(pd3dDevice);
}

void CBillBoardSpriteShader::CreateConstBuffer(ID3D11Device * pd3dDevice)
{
	SpriteAnimation* animation = new SpriteAnimation();

	animation->fSpriteAnimation_FPS   = m_frameFPS;
	animation->fSpriteAnimation_Count = m_frameCount;
	animation->fSpriteAnimation_Width = m_imageWidth;
	animation->fSpriteAnimation_Left  = m_frameLeft;


	//Create Bulr Const Buffer 
	D3D11_BUFFER_DESC pd3dDesc;
	ZeroMemory(&pd3dDesc, sizeof(pd3dDesc));
	pd3dDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDesc.ByteWidth = sizeof(SpriteAnimation);
	pd3dDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pd3dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dFogBufferData;
	ZeroMemory(&d3dFogBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dFogBufferData.pSysMem = animation;
	pd3dDevice->CreateBuffer(&pd3dDesc, &d3dFogBufferData, &m_pd3dcbAnimationSprite);

	if (animation)delete animation;
	animation = nullptr;
}

void CBillBoardSpriteShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_frameLeft = 1.0f  * (m_nCurrentFrame * m_frameWidth);

	m_fFPSCoount += m_frameFPS;
	if (m_fFPSCoount > 1.0f)
	{
		m_fFPSCoount = 0;
		m_nCurrentFrame++;
	}
	
	if (m_nCurrentFrame > m_frameCount)
	{
		m_nCurrentFrame = 0;
		if(m_nLoop != -999)
			m_nLoop--;
	}

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbAnimationSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	SpriteAnimation *animation = (SpriteAnimation*)d3dMappedResource.pData;
	
	animation->fSpriteAnimation_FPS   = m_frameFPS;
	animation->fSpriteAnimation_Count = m_frameCount;
	animation->fSpriteAnimation_Width = m_imageWidth;
	animation->fSpriteAnimation_Left  = m_frameLeft;

	pd3dDeviceContext->Unmap(m_pd3dcbAnimationSprite, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_SPRITE_ANIMATION, 1, &m_pd3dcbAnimationSprite);

}

void CBillBoardSpriteShader::Render(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_nLoop <= 0)
	{
		m_frameLeft		= 0.0f;					// 최초 시작위치
		m_nCurrentFrame = 0;
		m_fFPSCoount	= 0.0f;
		m_vMtxWorld.clear();
		return;
	}

	CBillBoardSpriteShader::UpdateConstBuffer(pd3dDeviceContext);

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

	//CONTENTMGR->DebugginBlendingState(pd3dDeviceContext);
	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}
void CBillBoardSpriteShader::StartBillBoard(int loop)
{
	StartSprtieAnimation(loop);
}

void CBillBoardSpriteShader::StartSprtieAnimation(int loop)
{
	m_nLoop = loop;
	m_frameLeft = 0.0f;					// 최초 시작위치
	m_nCurrentFrame = 0;
	m_fFPSCoount = 0.0f;
}

