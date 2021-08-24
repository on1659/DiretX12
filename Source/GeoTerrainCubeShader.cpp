
#include "stdafx.h"
#include "GeoTerrainCubeShader.h"

//---------------------------------------------------------------- Instance
CGeoTerrainCubeShader::CGeoTerrainCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CGeoMetryInstanceShader(move(pMesh), nObjects)
{

}

CGeoTerrainCubeShader::CGeoTerrainCubeShader(std::unique_ptr<CMesh> pMesh) : CGeoMetryInstanceShader(move(pMesh))
{

}

CGeoTerrainCubeShader::CGeoTerrainCubeShader(int nObjects) : CGeoMetryInstanceShader(nObjects)
{
}

CGeoTerrainCubeShader ::~CGeoTerrainCubeShader()
{
}

void CGeoTerrainCubeShader::CreateShader(ID3D11Device *pd3dDevice)
{
	UINT nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);
	CreateVertexShaderFromFile(pd3dDevice, L"GS_TerrainMeshCube.fx", "VS_GS_INSTANCE_Cube", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreateGeometryShaderFromFile(pd3dDevice, L"GS_TerrainMeshCube.fx", "GS_GS_INSTANCE_Cube_Texutre", "gs_5_0", &m_pd3dGeometryShader);

#ifdef _MULIT_RENDER_TARGET_
	CreatePixelShaderFromFile(pd3dDevice, L"GS_TerrainMeshCube.fx", "PS_GS_INSTANCE_Cube_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
#else
	CreatePixelShaderFromFile(pd3dDevice, L"GS_TerrainMeshCube.fx", "PS_GS_INSTANCE_Cube", "ps_5_0", &m_pd3dPixelShader);
#endif
}

