#include "stdafx.h"
#include "UIGauge.h"
#include "UIMesh.h"
#include "UISpriteMesh.h"
CUIGauge::CUIGauge() : Object()
{
}


CUIGauge::~CUIGauge()
{

}

void CUIGauge::Initialize(ID3D11Device * pd3dDevice, XMFLOAT2 position, XMFLOAT2 fSize, float frameFPS)
{
	auto pMesh = std::make_unique<CUISpriteMesh>(pd3dDevice, fSize.x, fSize.y);
	m_uiUIGauge = std::make_shared<CUIShader>();
	m_uiUIGauge->CreateShader(pd3dDevice);
	m_uiUIGauge->SetMesh(move(pMesh));
	m_uiUIGauge->CreateAfterShader(pd3dDevice, true);
	m_uiUIGauge->SetUIMaterial(pd3dDevice, "../Assets/03_Image/3D_UI/3d_dpy_respawn_time_bar.png");
	m_uiUIGauge->SetPosition(position.x, position.y);
	m_uiUIGauge->SetUIShader(m_uiUIGauge);
	m_uiUIGauge->BuildObjects(pd3dDevice);
	m_uiUIGauge->SetBlendState("Gauge");

	pMesh.reset();
	pMesh = std::make_unique<CUISpriteMesh>(pd3dDevice, fSize.x, fSize.y);
	m_uiUIGaugeBackground = std::make_shared<CUIShader>();
	m_uiUIGaugeBackground->CreateShader(pd3dDevice);
	m_uiUIGaugeBackground->CreateAfterShader(pd3dDevice, false);
	m_uiUIGaugeBackground->SetMesh(move(pMesh));
	m_uiUIGaugeBackground->SetUIMaterial(pd3dDevice, "../Assets/03_Image/3D_UI/3d_dpy_respawn_time_view.png");
	m_uiUIGaugeBackground->SetPosition(position.x, position.y);
	m_uiUIGaugeBackground->SetUIShader(m_uiUIGaugeBackground);
	m_uiUIGaugeBackground->BuildObjects(pd3dDevice);
	m_uiUIGaugeBackground->SetBlendState("UINoBlend");

	m_fPercent = 1.0f;
	m_fSpeed = frameFPS;


	CreateConstBuffer(pd3dDevice);
}


void CUIGauge::CreateConstBuffer(ID3D11Device* pd3dDevice)
{
	cbUIConstBuffer* gauge = new cbUIConstBuffer();
	gauge->gUIGage_RIGHT = 1.0f;


	//Create Bulr Const Buffer 
	D3D11_BUFFER_DESC pd3dDesc;
	ZeroMemory(&pd3dDesc, sizeof(pd3dDesc));
	pd3dDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDesc.ByteWidth = sizeof(cbUIConstBuffer);
	pd3dDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pd3dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dFogBufferData;
	ZeroMemory(&d3dFogBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dFogBufferData.pSysMem = gauge;
	pd3dDevice->CreateBuffer(&pd3dDesc, &d3dFogBufferData, &m_pd3dcbGaugeBuffer);

	if (gauge)delete gauge;
	gauge = nullptr;
}

void CUIGauge::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_fPercent -= m_fSpeed;

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbGaugeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	cbUIConstBuffer *gauge = (cbUIConstBuffer*)d3dMappedResource.pData;
	gauge->gUIGage_RIGHT = m_fPercent;
	pd3dDeviceContext->Unmap(m_pd3dcbGaugeBuffer, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_SPRITE_ANIMATION, 1, &m_pd3dcbGaugeBuffer);

}

void CUIGauge::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	CUIGauge::UpdateConstBuffer(pd3dDeviceContext);
	m_uiUIGauge->RenderTransparent(pd3dDeviceContext);
	m_uiUIGaugeBackground->RenderTransparent(pd3dDeviceContext);
}


