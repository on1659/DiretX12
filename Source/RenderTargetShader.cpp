#include "stdafx.h"
#include "RenderTargetShader.h"


CRenderTargetShader::CRenderTargetShader()
	: m_pd3dShaderResourceView(nullptr)
	, m_pd3dSamplerStates(nullptr)
	, m_pd3dVertexBuffer(nullptr)
	, m_pnVertexStrides(nullptr)
	, m_pnVertexOffsets(nullptr)
	, m_bExpand(false)
	, m_bUseNewRTV(false)
	, m_pd3dDestRTV(nullptr)
{
	m_inputVS_Screen[0] = XMFLOAT4{ -1.0f, +1.0f, 0.0f, 1.0f };
	m_inputVS_Screen[1] = XMFLOAT4{	+1.0f, +1.0f, 0.0f, 1.0f };
	m_inputVS_Screen[2] = XMFLOAT4{ +1.0f, -1.0f, 0.0f, 1.0f };
	m_inputVS_Screen[3] = XMFLOAT4{ +1.0f, -1.0f, 0.0f, 1.0f };

	for (int i = 0; i < 4; ++i) m_initVS_Screen[i] = m_inputVS_Screen[i];

	m_rRect.left   = m_inputVS_Screen[0].x;
	m_rRect.top    = m_inputVS_Screen[0].y;
	m_rRect.right  = m_inputVS_Screen[3].x;
	m_rRect.bottom = m_inputVS_Screen[3].y;


	m_wcVSPath = TEXT("../Assets/07.Shader/RenderTargetShader/VSRenderTargetShader.cso");
	m_wcPSPath = TEXT("../Assets/07.Shader/RenderTargetShader/PSRenderTargetShader.cso");

}


CRenderTargetShader::~CRenderTargetShader()
{
	if (m_pd3dShaderResourceView)m_pd3dShaderResourceView->Release();
	m_pd3dShaderResourceView = nullptr;

	if (m_pd3dVertexBuffer)m_pd3dVertexBuffer->Release();
	m_pd3dVertexBuffer = nullptr;

	if (m_pnVertexOffsets) delete m_pnVertexOffsets;
	m_pnVertexOffsets = nullptr;

	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
	m_pd3dVertexBuffer = nullptr;

	if (m_pd3dDeviceContext)m_pd3dDeviceContext->Release();
	m_pd3dDeviceContext = nullptr;
}

void CRenderTargetShader::ReleaseShaderVariables()
{
	CShader::ReleaseShaderVariables();

	if (m_pd3dShaderResourceView)m_pd3dShaderResourceView->Release();
	m_pd3dShaderResourceView = nullptr;

	if (m_pd3dVertexBuffer)m_pd3dVertexBuffer->Release();
	m_pd3dVertexBuffer = nullptr;

	if (m_pnVertexOffsets) delete m_pnVertexOffsets;
	m_pnVertexOffsets = nullptr;

	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
	m_pd3dVertexBuffer = nullptr;
	
	if (m_pd3dDeviceContext)m_pd3dDeviceContext->Release();
	m_pd3dDeviceContext = nullptr;
}

void CRenderTargetShader::CreaetShader(ID3D11Device* pd3dDevice)
{
	//화면 좌표계 4개 준다
	VS_INPUT v[4];
	for(auto i =0; i< 4; ++i)
		v[i].xf4_Pos = m_inputVS_Screen[i];
	//v[1].xf4_Pos = m_inputVS_Screen[1];
	//v[2].xf4_Pos = m_inputVS_Screen[2];
	//v[3].xf4_Pos = m_inputVS_Screen[3];

	v[0].xf2_UV = XMFLOAT2{ 0.0f, 1.0f };
	v[1].xf2_UV = XMFLOAT2{ 1.0f, 1.0f };
	v[2].xf2_UV = XMFLOAT2{ 0.0f, 0.0f };
	v[3].xf2_UV = XMFLOAT2{ 1.0f, 0.0f };

	//v[0].xf2_UV.x = (m_inputVS_Screen[0].x + 1) * 0.5f;
	//v[0].xf2_UV.y = (m_inputVS_Screen[0].y + 1 ) * 0.5f;
	//
	//v[1].xf2_UV.x = (m_inputVS_Screen[0].x + 1) * 0.5f;
	//v[1].xf2_UV.y = (m_inputVS_Screen[0].y + 1 ) * 0.5f;
	//
	//v[2].xf2_UV.x = (m_inputVS_Screen[0].x + 1) * 0.5f;
	//v[2].xf2_UV.y = (m_inputVS_Screen[0].y + 1 ) * 0.5f;
	//
	//v[3].xf2_UV.x = (m_inputVS_Screen[0].x + 1) * 0.5f;
	//v[3].xf2_UV.y = (m_inputVS_Screen[0].y + 1 ) * 0.5f;

	//버텍스 버퍼 생성
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(VS_INPUT) * 4;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = v;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	m_pnVertexStrides = new UINT[1];
	m_pnVertexOffsets = new UINT[1];

	m_pnVertexStrides[0] = sizeof(VS_INPUT);
	m_pnVertexOffsets[0] = 0;

	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	auto hr = pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pd3dSamplerStates);
	if (FAILED(hr))
	{
		MessageBox(g_hWnd, TEXT("FAILE RTV"), TEXT("FAIL RTV"), MB_OK);
	}


	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CShader::CreateVertexShaderFromCompiledFile(pd3dDevice, m_wcVSPath, d3dInputLayout, nElements, &m_pd3dVertexShader);
	Radar::DXUT_SetDebugName(m_pd3dVertexShader, "VSRenderTargetShader");

	CShader::CreatePixelShaderFromCompiledFile(pd3dDevice, m_wcPSPath, &m_pd3dPixelShader);
	Radar::DXUT_SetDebugName(m_pd3dPixelShader, "PSRenderTargetShader");

}

void CRenderTargetShader::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	pd3dDeviceContext->PSSetShaderResources(0, 1, &m_pd3dShaderResourceView);
	pd3dDeviceContext->PSSetSamplers(0, 1, &m_pd3dSamplerStates);

	// read
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	pd3dDeviceContext->Draw(4, 0);

	// clean 
	//pd3dDeviceContext->PSSetShaderResources(0, 1, NULL);
	pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

}

void CRenderTargetShader::ExpandRTV(const float& x, const float& y)
{
	if (!m_pd3dDeviceContext) return;

	if (Contain(x, y))
	{
		if (m_bExpand) return;

		m_bExpand = true;

		SetScreenPos(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.5f), XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, FRAME_BUFFER_HEIGHT * 0.5f), false);


		VS_INPUT vs_input[4];
		for (auto i = 0; i < 4; ++i)
		{
			vs_input[i].xf4_Pos = m_inputVS_Screen[i];
		}

		vs_input[0].xf2_UV = XMFLOAT2{ 0.0f, 1.0f };
		vs_input[1].xf2_UV = XMFLOAT2{ 1.0f, 1.0f };
		vs_input[2].xf2_UV = XMFLOAT2{ 0.0f, 0.0f };
		vs_input[3].xf2_UV = XMFLOAT2{ 1.0f, 0.0f };

		m_pd3dDeviceContext->UpdateSubresource(m_pd3dVertexBuffer, 0, NULL, vs_input, 0, 0);

	}
	else
	{
		if (m_bExpand)
		{
			m_bExpand = false;

			for (auto i = 0; i < 4; ++i)
				m_inputVS_Screen[i] = m_initVS_Screen[i];

			VS_INPUT vs_input[4];
			for (auto i = 0; i < 4; ++i)
			{
				vs_input[i].xf4_Pos = m_inputVS_Screen[i];
			}

			vs_input[0].xf2_UV = XMFLOAT2{ 0.0f, 1.0f };
			vs_input[1].xf2_UV = XMFLOAT2{ 1.0f, 1.0f };
			vs_input[2].xf2_UV = XMFLOAT2{ 0.0f, 0.0f };
			vs_input[3].xf2_UV = XMFLOAT2{ 1.0f, 0.0f };

			m_pd3dDeviceContext->UpdateSubresource(m_pd3dVertexBuffer, 0, NULL, vs_input, 0, 0);

			rect_update();

		}
	}

}
