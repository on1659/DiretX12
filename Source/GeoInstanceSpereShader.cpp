#include "stdafx.h"
#include "GeoInstanceSpereShader.h"

//---------------------------------------------------------------- Instance
CGeoInstanceSphereShader::CGeoInstanceSphereShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CGeoMetryInstanceShader(move(pMesh), nObjects)
{

}

CGeoInstanceSphereShader::CGeoInstanceSphereShader(std::unique_ptr<CMesh> pMesh) : CGeoMetryInstanceShader(move(pMesh))
{

}

CGeoInstanceSphereShader::CGeoInstanceSphereShader(int nObjects) : CGeoMetryInstanceShader(nObjects)
{
}

CGeoInstanceSphereShader ::~CGeoInstanceSphereShader()
{
}

void CGeoInstanceSphereShader::CreateShader(ID3D11Device *pd3dDevice)
{
	//GS를 사용하는 큐브는 size와 디폴트 색상을 가진다.
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);
	CreateVertexShaderFromFile(pd3dDevice, L"GS_Instance_Spere.fx", "VS_GS_Instance_Sphere", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreateGeometryShaderFromFile(pd3dDevice, L"GS_Instance_Spere.fx", "GS_GS_Instance_Sphere", "gs_5_0", &m_pd3dGeometryShader);

	#ifdef _MULIT_RENDER_TARGET_
		CreatePixelShaderFromFile(pd3dDevice, L"GS_Instance_Spere.fx", "PS_GS_Instance_Sphere_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	#else
		CreatePixelShaderFromFile(pd3dDevice, L"GS_Instance_Spere.fx", "PS_GS_Instance_Sphere", "ps_5_0", &m_pd3dPixelShader);
	#endif
}

