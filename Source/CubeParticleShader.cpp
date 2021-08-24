#pragma once
#include "stdafx.h"
#include "CubeParticleShader.h"

#define PARTICLE_TYPE_EMITTER 0
#define PARTICLES			  500	

float R(float a, float b) {
	return(a + (float)(rand() / (float)RAND_MAX) * (b - a));
}

CCubeParticleShader::CCubeParticleShader()
{
	m_nMaxParticles = 0;
	m_bInitializeParticle = false;
	m_fGameTime = 0.0f;
	m_fTimeStep = 0.0f;
	m_fAge = 0.0f;
	m_nOffset = 0;
	m_nStride = 0;
	m_d3dxvAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_pd3dcbParticleInfo = nullptr;

	m_pd3dInitialVertexBuffer = nullptr;
	m_pd3dStreamOutVertexBuffer = nullptr;
	m_pd3dDrawVertexBuffer = nullptr;
	m_pd3dVertexLayout = nullptr;
	m_pd3dVertexShader = nullptr;
	m_pd3dGeometryShader = nullptr;
	m_pd3dPixelShader = nullptr;
	m_pd3dSOVertexShader = nullptr;
	m_pd3dSOGeometryShader = nullptr;
	m_pd3dRasterizerState = nullptr;
	m_pd3dSODepthStencilState = nullptr;
	m_pd3dDepthStencilState = nullptr;
	m_pd3dBlendState = nullptr;
	m_pd3dsrvRandomTexture = nullptr;
	m_pd3dSamplerState = nullptr;
	m_pd3dsrvTextureArray = nullptr;

	m_fSpeed = PARTICLE_SPEED;
	m_fSize = PARTICLE_SIZE;
	m_fDuringTime = PARTICLE_DURINGTIME;
	m_fEmitCount = PARTICLE_EMIT_COUNT;
	m_fRandPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CCubeParticleShader::~CCubeParticleShader()
{
	if (m_pd3dInitialVertexBuffer) m_pd3dInitialVertexBuffer->Release();
	if (m_pd3dStreamOutVertexBuffer) m_pd3dStreamOutVertexBuffer->Release();
	if (m_pd3dDrawVertexBuffer) m_pd3dDrawVertexBuffer->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dGeometryShader) m_pd3dGeometryShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dSOVertexShader) m_pd3dSOVertexShader->Release();
	if (m_pd3dSOGeometryShader) m_pd3dSOGeometryShader->Release();
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dSODepthStencilState) m_pd3dSODepthStencilState->Release();
	if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
	if (m_pd3dBlendState) m_pd3dBlendState->Release();
	if (m_pd3dsrvRandomTexture) m_pd3dsrvRandomTexture->Release();
	if (m_pd3dSamplerState) m_pd3dSamplerState->Release();
	if (m_pd3dsrvTextureArray) m_pd3dsrvTextureArray->Release();
}

void CCubeParticleShader::ReleaseShader()
{
	if (m_pd3dInitialVertexBuffer) m_pd3dInitialVertexBuffer->Release();
	if (m_pd3dStreamOutVertexBuffer) m_pd3dStreamOutVertexBuffer->Release();
	if (m_pd3dDrawVertexBuffer) m_pd3dDrawVertexBuffer->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dGeometryShader) m_pd3dGeometryShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dSOVertexShader) m_pd3dSOVertexShader->Release();
	if (m_pd3dSOGeometryShader) m_pd3dSOGeometryShader->Release();
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dSODepthStencilState) m_pd3dSODepthStencilState->Release();
	if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
	if (m_pd3dBlendState) m_pd3dBlendState->Release();
	if (m_pd3dsrvRandomTexture) m_pd3dsrvRandomTexture->Release();
	if (m_pd3dSamplerState) m_pd3dSamplerState->Release();
	if (m_pd3dsrvTextureArray) m_pd3dsrvTextureArray->Release();
}

void CCubeParticleShader::Initialize(ID3D11Device *pd3dDevice, TCHAR *pszFileName, XMFLOAT3 d3dxvPosition, UINT nMaxParticles)
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

	//랜덤값을 위한 텍스쳐
	m_pd3dsrvRandomTexture = Helper::CreateRandomTexture1DSRV(pd3dDevice);

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

void CCubeParticleShader::CreateParticle(ID3D11Device *pd3dDevice, XMFLOAT3& Position, XMFLOAT3& Direction, XMFLOAT3& Accelerater)
{
	m_d3dxvEmitPosition = Position;
	m_d3dxvEmitDirection = Direction;
	m_d3dxvAcceleration = Accelerater;
	m_nOffset = 0;
	m_nStride = sizeof(CParticleVertex);

	CreateConstBuffer(pd3dDevice);
}

void CCubeParticleShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/VS.fxo", d3dInputLayout, 5, &m_pd3dVertexShader);
	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/VSStream.fxo", d3dInputLayout, 5, &m_pd3dSOVertexShader);

	CreateGeoMoetryShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/GS_CubeDraw.fxo", &m_pd3dGeometryShader);
	CreateSOGeoMoetryShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/GSStream.fxo", &m_pd3dSOGeometryShader);

	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/PS.fxo", &m_pd3dPixelShader);



	//CreateVertexShaderFromFile(pd3dDevice, L"SmashParticle.fx", "VSParticleStreamOut", "vs_5_0", &m_pd3dSOVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	//CreateVertexShaderFromFile(pd3dDevice, L"SmashParticle.fx", "VSParticleDraw", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	//
	//CreateSOGeometryShaderFromFile(pd3dDevice, L"SmashParticle.fx", "GSParticleStreamOut", "gs_5_0", &m_pd3dSOGeometryShader);
	//CreateGeometryShaderFromFile(pd3dDevice, L"SmashParticle.fx", "GSParticleDraw", "gs_5_0", &m_pd3dGeometryShader);
	//
	//#ifdef _MULIT_RENDER_TARGET_
	//	CreatePixelShaderFromFile(pd3dDevice, L"SmashParticle.fx", "PSParticleDraw_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"SmashParticle.fx", "PSParticleDraw", "ps_5_0", &m_pd3dPixelShader);
	//#endif


}

bool CCubeParticleShader::Update(float fTimeElapsed)
{
	m_fTimeStep = fTimeElapsed;
	m_fGameTime = m_fGameTime + fTimeElapsed;
	return false;
}

void CCubeParticleShader::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_fGameTime > m_fDuringTime * 5.0f) return;

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

	CONTENTMGR->DebugginBlendingState(pd3dDeviceContext);

	//pd3dDeviceContext->PSSetSamplers(0, 1, &m_pd3dSamplerState);
	pd3dDeviceContext->PSSetSamplers(PS_SLOT_PARTICLE_SAMPLER, 1, &m_pd3dSamplerState);
	pd3dDeviceContext->PSSetShaderResources(PS_SLOT_PARTICLE_TEXTURED_ARRARY, 1, &m_pd3dsrvTextureArray);

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &m_nStride, &m_nOffset);
	pd3dDeviceContext->DrawAuto();

	pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);

}

void CCubeParticleShader::CreateConstBuffer(ID3D11Device *pd3dDevice)
{

	// 래스터라이즈 설정
	D3D11_RASTERIZER_DESC d3dRasterizerStateDesc;
	ZeroMemory(&d3dRasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerStateDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerStateDesc.FillMode = D3D11_FILL_SOLID;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerStateDesc, &m_pd3dRasterizerState);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CParticleVertex);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	CParticleVertex particle;
	ZeroMemory(&particle, sizeof(CParticleVertex));
	D3D11_SUBRESOURCE_DATA d3dSubResourceData;
	d3dSubResourceData.pSysMem = &particle;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubResourceData, &m_pd3dInitialVertexBuffer);


	//std::cout << "Count : " << m_nMaxParticles <<
	//	" Size Of : " << sizeof(CParticleVertex)  * m_nMaxParticles << "Byte " << " KB : " << (sizeof(CParticleVertex)  * m_nMaxParticles) / 1024 << std::endl;

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_nStride = sizeof(CParticleVertex);
	d3dBufferDesc.ByteWidth = sizeof(CParticleVertex) * m_nMaxParticles;//PARTICLES = 500
	d3dBufferDesc.BindFlags = D3D11_BIND_STREAM_OUTPUT;

	//---------------------------------------------------------------------------
	//d3dBufferDesc.Usage					= D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	//d3dBufferDesc.CPUAccessFlags		= D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	//d3dBufferDesc.MiscFlags				= D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	//d3dBufferDesc.StructureByteStride	= D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;



	HRESULT hResult = pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dStreamOutVertexBuffer);
	hResult = pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dDrawVertexBuffer);
	//버퍼 두개를 만든다.

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(CB_PARTICLEINFO);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbParticleInfo);
	//상수버퍼를 만듬.(파티클 정보)
}

void CCubeParticleShader::UpdateConstBuffer(ID3D11DeviceContext* pd3dDeviceContext)
{
	//-----------------------------------------------------------------------------------------------

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbParticleInfo, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	CB_PARTICLEINFO *pcbParticleInfo = (CB_PARTICLEINFO*)d3dMappedResource.pData;
	pcbParticleInfo->m_fGameTime = m_fGameTime;
	pcbParticleInfo->m_fTimeStep = m_fTimeStep;
	pcbParticleInfo->m_d3dxvAcceleration = m_d3dxvAcceleration;
	pcbParticleInfo->m_d3dxvEmitDirection = m_d3dxvEmitDirection;
	pcbParticleInfo->m_d3dxvEmitPosition = m_d3dxvEmitPosition;
	pcbParticleInfo->m_uiTextureType = rand() % 4;
	pcbParticleInfo->fSize = m_fSize;
	pcbParticleInfo->fSpeed = m_fSpeed;
	pcbParticleInfo->fDuringTime = m_fDuringTime;
	pcbParticleInfo->fEmitCount = m_fEmitCount;

	pd3dDeviceContext->Unmap(m_pd3dcbParticleInfo, 0);

	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_PARTICLE_INFO, 1, &m_pd3dcbParticleInfo);
}

void CCubeParticleShader::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;
	D3D11_SO_DECLARATION_ENTRY pSODecls[] = {
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "VELOCITY", 0, 0, 3, 0 },
		{ 0, "SIZE", 0, 0, 2, 0 },
		{ 0, "TYPE", 0, 0, 1, 0 },
		{ 0, "AGE", 0, 0, 1, 0 }
	};
	UINT pBufferStrides[1] = { sizeof(CParticleVertex) };//개수 많이 지정해야되지않나

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateGeometryShaderWithStreamOutput
		(
			pd3dShaderBlob->GetBufferPointer(),
			pd3dShaderBlob->GetBufferSize(),
			pSODecls,			// 스트림아웃 디클러레이션
			5,					// 엔트리 개수
			pBufferStrides,		// 버퍼 스트라이드
			1,					// 스트라이드 개수
			0,					// 래스터라이즈 스트림
			NULL,
			ppd3dGeometryShader
		);

		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(g_hWnd, L"SOGSERROR", L"SmashParticle", MB_OK);
		exit(0);
	}
}

void CCubeParticleShader::CreateSOGeoMoetryShaderFromCompiledFile(ID3D11Device * pd3dDevice, WCHAR * pszFileName, ID3D11GeometryShader** ppd3dGeomeotryShader)
{

	D3D11_SO_DECLARATION_ENTRY pSODecls[] = {
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "VELOCITY", 0, 0, 3, 0 },
		{ 0, "SIZE", 0, 0, 2, 0 },
		{ 0, "TYPE", 0, 0, 1, 0 },
		{ 0, "AGE", 0, 0, 1, 0 }
	};
	UINT pBufferStrides[1] = { sizeof(CParticleVertex) };//개수 많이 지정해야되지않나

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult =
		pd3dDevice->CreateGeometryShaderWithStreamOutput
		(
			pByteCode,
			nReadBytes,
			pSODecls,			// 스트림아웃 디클러레이션
			5,					// 엔트리 개수
			pBufferStrides,		// 버퍼 스트라이드
			1,					// 스트라이드 개수
			0,					// 래스터라이즈 스트림
			NULL,
			ppd3dGeomeotryShader
		);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(g_hWnd, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(g_hWnd, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;

}