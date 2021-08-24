#include "stdafx.h"
#include "SkyBoxShader.h"

CSkyBoxShader::CSkyBoxShader()
{
}

CSkyBoxShader::~CSkyBoxShader()
{
}

void CSkyBoxShader::CreateShader(ID3D11Device *pd3dDevice)
{
		D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT nElements = ARRAYSIZE(d3dInputElements);
		
		CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/SkyBox/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
		CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/SkyBox/PS.fxo", &m_pd3dPixelShader);



		//CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSSkyBoxTexturedColor", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
		//#ifdef _MULIT_RENDER_TARGET_
		//		CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSSkyBoxTexturedColor_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
		//#else
		//	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSSkyBoxTexturedColor", "ps_5_0", &m_pd3dPixelShader);
		//#endif


}
