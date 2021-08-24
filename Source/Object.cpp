
#include "stdafx.h"
#include "Object.h"

Object::Object()
{
	name.clear();

	//태그
	tag = 0;

	//레이어
	layer = 0;

	//State
	m_pd3dBlendState = nullptr;
	m_pd3dRasterizerState = nullptr;
	m_pd3dDepthStencilState = nullptr;

	m_xmMatrix = XMMatrixIdentity();
	m_xmVector = XMVectorZero();
	m_xmFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	memset(&m_xmFLOAT4X4, 0, sizeof(m_xmFLOAT4X4));
}
	
Object::Object(std::string name, UINT tag)
{
	//이름
	Object::name = name;

	//태그
	Object::tag = tag;

	//레이어
	Object::layer = 0;
	//State
	m_pd3dBlendState = nullptr;
	m_pd3dRasterizerState = nullptr;
	m_pd3dDepthStencilState = nullptr;

	m_xmMatrix = XMMatrixIdentity();
	m_xmVector = XMVectorZero();
	m_xmFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	memset(&m_xmFLOAT4X4, 0, sizeof(m_xmFLOAT4X4));
}
	
	Object::~Object()
	{
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
	if (m_pd3dBlendState) m_pd3dBlendState->Release();

	m_pd3dDepthStencilState = nullptr;
	m_pd3dRasterizerState = nullptr;
	m_pd3dRasterizerState = nullptr;
	}

	void Object::Release()
	{
		if (m_pd3dBlendState)		m_pd3dBlendState->Release();
		if (m_pd3dRasterizerState)   m_pd3dRasterizerState->Release();
		if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
		
		m_pd3dDepthStencilState = nullptr;
		m_pd3dRasterizerState = nullptr;
		m_pd3dRasterizerState = nullptr;
	}

void Object::CreateBlendState(ID3D11Device *pd3dDevice)
{
	Object::SetBlendState("Default");
}

void Object::CreateBlendState(ID3D11Device *pd3dDevice, D3D11_BLEND_DESC desc)
{
	m_pd3dBlendState = nullptr;
	if (pd3dDevice) pd3dDevice->CreateBlendState(&desc, &m_pd3dBlendState);
}

void Object::CreateRasterizerState(ID3D11Device *pd3dDevice, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = cullMode;
	d3dRasterizerDesc.FillMode = fillMode;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);

}

void Object::SetBlendState(ID3D11BlendState * pd3dBlendState)
{
	if (nullptr == pd3dBlendState)return;

	if (m_pd3dBlendState)m_pd3dBlendState->Release();
	m_pd3dBlendState = nullptr;
	m_pd3dBlendState = pd3dBlendState;
}

void Object::SetBlendState(std::string name)
{
	m_strBlendstate = name;
}

void Object::CreateDepthStencilState(ID3D11Device *pd3dDevice)
{
}
