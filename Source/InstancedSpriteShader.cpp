#include "stdafx.h"
#include "InstancedSpriteShader.h"
#include "UISpriteMesh.h"

CInstancedSpriteShader::CInstancedSpriteShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{
}

CInstancedSpriteShader::CInstancedSpriteShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{

}

CInstancedSpriteShader::CInstancedSpriteShader(int nObjects) : CInstancedObjectsShader(nObjects)
{

}

CInstancedSpriteShader::~CInstancedSpriteShader()
{
}

void CInstancedSpriteShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CInstancedObjectsShader::BuildObjects(pd3dDevice, pContext);
	//Object::SetBlendState("BillBoardSprite");
}

void CInstancedSpriteShader::CreateShader(ID3D11Device *pd3dDevice)
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

	//CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);

	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/InstancedSpriteVS.cso", d3dInputElements, nElements, &m_pd3dVertexShader);
	 CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.ShaderInstanced/InstancedSpritePS.cso", &m_pd3dPixelShader);

}

void CInstancedSpriteShader::Initialize(ID3D11Device *pd3dDevice, float frameFPS, int frameCount, int loop)
{
	m_frameFPS = 1 / frameFPS;			// 증가량
	m_frameCount = frameCount;			// 몇장이니
	m_frameWidth = 1.0f / frameCount;	// 증가값
	m_fFPSCoount = 0;


	m_frameLeft = 0.0f;					// 최초 시작위치
	m_imageWidth = 1.0f / frameCount;	// 이미지 한장당 크기

	m_nCurrentFrame = 0;
	m_fFPSCoount = 0.0f;

	m_nLoop = loop;
	CInstancedSpriteShader::CreateConstBuffer(pd3dDevice);
}

void CInstancedSpriteShader::CreateConstBuffer(ID3D11Device * pd3dDevice)
{
	SpriteAnimation* animation = new SpriteAnimation();

	animation->fSpriteAnimation_FPS	   = m_frameFPS;
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

void CInstancedSpriteShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_frameLeft =(m_nCurrentFrame * m_frameWidth);

	m_fFPSCoount += m_frameFPS;
	if (m_fFPSCoount > 1.0f)
	{
		m_fFPSCoount = 0;
		m_nCurrentFrame++;
	}

	if (m_nCurrentFrame >= m_frameCount)
	{
		m_nCurrentFrame = 0;
		m_nLoop--;
		if (m_nLoop >= 900) m_nLoop++;
	}


	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbAnimationSprite, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	SpriteAnimation *animation = (SpriteAnimation*)d3dMappedResource.pData;

	animation->fSpriteAnimation_FPS = m_frameFPS;
	animation->fSpriteAnimation_Count = (float)m_nCurrentFrame;
	animation->fSpriteAnimation_Width = m_imageWidth;
	animation->fSpriteAnimation_Left = m_frameLeft;
	//(m_imageWidth * (float)m_nCurrentFrame)
	pd3dDeviceContext->Unmap(m_pd3dcbAnimationSprite, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_SPRITE_ANIMATION, 1, &m_pd3dcbAnimationSprite);
}

void CInstancedSpriteShader::Render(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_nLoop <= 0)
	{
		m_frameLeft = 0.0f;					// 최초 시작위치
		m_nCurrentFrame = 0;
		m_fFPSCoount = 0.0f;
		return;
	}


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
	CInstancedSpriteShader::UpdateConstBuffer(pd3dDeviceContext);
	for (auto &pObject : m_vRenderObject)
	{
		pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(pObject->GetWorldMatrix());
	}
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

}

void CInstancedSpriteShader::RenderNoClear(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_nLoop <= 0)
	{
		m_frameLeft = 0.0f;					// 최초 시작위치
		m_nCurrentFrame = 0;
		m_fFPSCoount = 0.0f;
		return;
	}

	CInstancedSpriteShader::UpdateConstBuffer(pd3dDeviceContext);

	CInstancedSpriteShader::OnPrepareRender(pd3dDeviceContext);

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

	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, GetBlendStatePath());
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

}


void CInstancedSpriteShader::StartSprtieAnimation(int loop)
{
	m_nLoop = loop;
	m_frameLeft = 0.0f;					// 최초 시작위치
	m_nCurrentFrame = 0;
	m_fFPSCoount = 0.0f;
}

