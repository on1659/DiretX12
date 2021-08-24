#include "stdafx.h"
#include "TerrainCubeShader.h"

//----------------------------------------------------------------

CTerrainCubeShader::CTerrainCubeShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{

}

CTerrainCubeShader::CTerrainCubeShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{

}

CTerrainCubeShader::CTerrainCubeShader(int nObjects) : CInstancedObjectsShader(nObjects)
{

}

CTerrainCubeShader::~CTerrainCubeShader()
{

}

void CTerrainCubeShader::CreateShader(ID3D11Device *pd3dDevice)
{
	int nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },										//0
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },										//1
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },										//2
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/VS.fxo", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/PS.fxo", &m_pd3dPixelShader);


	//CreateVertexShaderFromFile(pd3dDevice, L"Instanced.fx", "VSInstanced", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	//#ifdef _MULIT_RENDER_TARGET_
	//	 CreatePixelShaderFromFile(pd3dDevice, L"Instanced.fx", "PSInstanced_GBUFFER", "ps_5_0", &m_pd3dPixelShader);
	//#else
	//	CreatePixelShaderFromFile(pd3dDevice, L"Instanced.fx", "PSInstanced", "ps_5_0", &m_pd3dPixelShader);
	//#endif
}

