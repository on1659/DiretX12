#include "stdafx.h"
#include "FlowerParticleShader.h"

#include "QuadParticleShader.h"

CFlowerParticleShader::CFlowerParticleShader() : CQuadParticleShader()
{
	m_fPauseTime = 0.0f;
	m_fMaxPauseTime = 0.0f;
}

CFlowerParticleShader::~CFlowerParticleShader()
{
}

void CFlowerParticleShader::Initialize(ID3D11Device *pd3dDevice, TCHAR *pszFileName, XMFLOAT3 d3dxvPosition, UINT nMaxParticles)
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



	CreateParticle(pd3dDevice, d3dxvPosition, XMFLOAT3(0, 1, 0), XMFLOAT3(0, 1, 0));


}


bool CFlowerParticleShader::Update(float fTimeElapsed)
{
	m_fPauseTime -= fTimeElapsed;
	if (m_fPauseTime > 0)return false;

	CCubeParticleShader::Update(fTimeElapsed);

	m_fRandPosition = XMFLOAT3((rand() % 10 - 5), (rand() % 2 - 2), rand() % 2 - 2);

	if (m_fGameTime >= m_fDuringTime)
	{
		m_fGameTime = 0;
		m_bInitializeParticle = true;
		m_fPauseTime = m_fMaxPauseTime;
	}
	return true;
}

void CFlowerParticleShader::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	if (m_fPauseTime > 0)return;

	UpdateConstBuffer(pd3dDeviceContext);

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
