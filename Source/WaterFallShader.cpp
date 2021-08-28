#pragma once
#include "stdafx.h"
#include "WaterFallShader.h"

CWaterFallShader::CWaterFallShader() : CCubeParticleShader()
{
	m_fPauseTime = 0.0f;
	m_fMaxPauseTime = 0.0f;
}

CWaterFallShader::~CWaterFallShader()
{
}

void CWaterFallShader::Initialize(ID3D11Device* pd3dDevice, TCHAR *pszFileName, XMFLOAT3 d3dxvPosition, UINT nMaxParticles, float frameFPS, float frameCount)
{
	//m_pd3dsrvTextureArray = pd3dsrvTexArray;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pszFileName, NULL, NULL, &m_pd3dsrvTextureArray, NULL);
	//D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../ice.png"), NULL, NULL, &m_pd3dsrvTextureArray, NULL);

	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pd3dSamplerState);
	//샘플러 상태 추가

	// 깊이 스텐실 상태 설정
	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = false;
	d3dDepthStencilDesc.StencilEnable = false;
	d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dSODepthStencilState);

	// 블렌드 상태 설정
	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	d3dBlendStateDesc.IndependentBlendEnable = false;
	ZeroMemory(&d3dBlendStateDesc.RenderTarget[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	d3dBlendStateDesc.RenderTarget[0].BlendEnable = true;
	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_pd3dBlendState);

	SetBlendState("WaterFall");

	//랜덤값을 위한 텍스쳐
	m_pd3dsrvRandomTexture = Helper::CrseateRandomTexture1DSRV_WaterFall(pd3dDevice);

	m_nMaxParticles = nMaxParticles;

	m_bInitializeParticle = true;

	m_fGameTime = 0;
	m_fTimeStep = 0;
	m_fAge = 0;

	m_d3dxvAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);


	m_frameFPS = 1 / frameFPS;			// 증가량
	m_frameCount = frameCount;			// 몇장이니
	m_frameWidth = 1.0f / frameCount;	// 증가값
	m_fFPSCoount = 0;


	m_frameLeft = 0.0f;					// 최초 시작위치
	m_imageWidth = 1.0f / frameCount;	// 이미지 한장당 크기

	m_nCurrentFrame = 0;
	m_fFPSCoount = 0.0f;

	m_bSoundPlaying = false;

	CreateParticle(pd3dDevice, d3dxvPosition, XMFLOAT3(0, 1, 0), XMFLOAT3(0, 0, 0));
	CreateConstBuffer(pd3dDevice);


}

void CWaterFallShader::CreateConstBuffer(ID3D11Device*  pd3dDevice)
{

	CCubeParticleShader::CreateConstBuffer(pd3dDevice);

	SpriteAnimation* animation = new SpriteAnimation();

	animation->fSpriteAnimation_FPS = m_frameFPS;
	animation->fSpriteAnimation_Count = m_frameCount;
	animation->fSpriteAnimation_Width = m_imageWidth;
	animation->fSpriteAnimation_Left = m_frameLeft;

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

void CWaterFallShader::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	CCubeParticleShader::UpdateConstBuffer(pd3dDeviceContext);

	m_frameLeft = (m_nCurrentFrame * m_frameWidth);

	m_fFPSCoount += m_frameFPS;
	if (m_fFPSCoount > 1.0f)
	{
		m_fFPSCoount = 0;
		m_nCurrentFrame++;
	}

	if (m_nCurrentFrame >= m_frameCount)
	{
		m_nCurrentFrame = 0;
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
	pd3dDeviceContext->GSSetConstantBuffers(PS_CB_SLOT_SPRITE_ANIMATION, 1, &m_pd3dcbAnimationSprite);
}

void CWaterFallShader::AfterCreateShader(ID3D11Device*  pd3dDevice)
{

	if (m_pd3dSOGeometryShader)m_pd3dSOGeometryShader->Release();
	m_pd3dSOGeometryShader = nullptr;
	CreateSOGeoMoetryShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/GS_Stream.cso.fxo", &m_pd3dSOGeometryShader);


	if (m_pd3dPixelShader)m_pd3dPixelShader->Release();
	m_pd3dPixelShader = nullptr;
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/WaterFall/PS.fxo", &m_pd3dPixelShader);

	if (m_pd3dGeometryShader)m_pd3dGeometryShader->Release();
	m_pd3dGeometryShader = nullptr;
	CreateGeoMoetryShaderFromCompiledFile(pd3dDevice, L"GS.fxo", &m_pd3dGeometryShader);

}

void CWaterFallShader::AfterAuraCreateShader(ID3D11Device*  pd3dDevice)
{
	if (m_pd3dPixelShader)m_pd3dPixelShader->Release();
	m_pd3dPixelShader = nullptr;
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/WaterFall/PS.fxo", &m_pd3dPixelShader);

	if (m_pd3dGeometryShader)m_pd3dGeometryShader->Release();
	m_pd3dGeometryShader = nullptr;
	CreateGeoMoetryShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/GS_QuadDraw.cso", &m_pd3dGeometryShader);
}

bool CWaterFallShader::Update(float fTimeElapsed)
{
	m_fPauseTime -= fTimeElapsed;
	if (m_fPauseTime > 0)return false;

	CCubeParticleShader::Update(fTimeElapsed);

	//m_fRandPosition = XMFLOAT3((rand() % 10 - 5), (rand() % 2 - 2), rand() % 2 - 2);

	if (m_fGameTime >= m_fDuringTime)
	{
		m_fGameTime = 0;
		m_bInitializeParticle = true;
		m_fPauseTime = m_fMaxPauseTime;
	}
	return true;
}

void CWaterFallShader::Render(ID3D11DeviceContext * pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	if (m_iParticle == 0)UpdateConstBuffer(pd3dDeviceContext);

	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->SOSetTargets(1, &m_pd3dStreamOutVertexBuffer, &m_nOffset);

	pd3dDeviceContext->VSSetShader(m_pd3dSOVertexShader, NULL, 0);
	pd3dDeviceContext->GSSetShader(m_pd3dSOGeometryShader, NULL, 0);
	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dSODepthStencilState, 0);
	//pd3dDeviceContext->GSSetSamplers(0, 1, &m_pd3dSamplerState);
	pd3dDeviceContext->GSSetSamplers(GS_SLOT_PARTICLE_SAMPLER, 1, &m_pd3dSamplerState);
	pd3dDeviceContext->GSSetShaderResources(GS_SLOT_RANDOM_TEXTURED, 1, &m_pd3dsrvRandomTexture);

	if (m_bInitializeParticle)
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dInitialVertexBuffer, &m_nStride, &m_nOffset);
		pd3dDeviceContext->Draw(1, 0);
		m_bInitializeParticle = false;
	}
	else
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &m_nStride, &m_nOffset);
		pd3dDeviceContext->DrawAuto();
	}
	ID3D11Buffer *pd3dBuffer = m_pd3dDrawVertexBuffer;
	m_pd3dDrawVertexBuffer = m_pd3dStreamOutVertexBuffer;
	m_pd3dStreamOutVertexBuffer = pd3dBuffer;

	ID3D11Buffer *pd3dBuffers[1] = { NULL };
	UINT pStreamOffSets[1] = { 0 };
	pd3dDeviceContext->SOSetTargets(1, pd3dBuffers, pStreamOffSets);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->GSSetShader(m_pd3dGeometryShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);

	//pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, NULL, 0xffffffff);

	CONTENTMGR->OMSetBlendingState(pd3dDeviceContext, Object::GetBlendStatePath());
	//CONTENTMGR->DebugginBlendingState(pd3dDeviceContext);


	pd3dDeviceContext->PSSetSamplers(PS_SLOT_PARTICLE_SAMPLER, 1, &m_pd3dSamplerState);
	pd3dDeviceContext->PSSetShaderResources(PS_SLOT_PARTICLE_TEXTURED_ARRARY, 1, &m_pd3dsrvTextureArray);

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &m_nStride, &m_nOffset);
	pd3dDeviceContext->DrawAuto();

	pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

/*

*/