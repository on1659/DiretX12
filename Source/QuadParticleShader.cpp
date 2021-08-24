#pragma once
#include "stdafx.h"
#include "QuadParticleShader.h"

CQuadParticleShader::CQuadParticleShader() : CCubeParticleShader()
{

}

CQuadParticleShader::~CQuadParticleShader()
{
}

void CQuadParticleShader::CreateShader(ID3D11Device *pd3dDevice)
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

	CreateGeoMoetryShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/GS_QuadDraw.fxo", &m_pd3dGeometryShader);
	CreateSOGeoMoetryShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/GSStream.fxo", &m_pd3dSOGeometryShader);

	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/ParticleShader/PS.fxo", &m_pd3dPixelShader);

}


void CQuadParticleShader::CreateConstBuffer(ID3D11Device *pd3dDevice)
{
	CCubeParticleShader::CreateConstBuffer(pd3dDevice);
}

void CQuadParticleShader::UpdateConstBuffer(ID3D11DeviceContext* pd3dDeviceContext)
{
	CCubeParticleShader::UpdateConstBuffer(pd3dDeviceContext);

}
