#include "stdafx.h"
#include "ChochShader.h"

//----------------------------------------------------------------

CChochShader::CChochShader(std::unique_ptr<CMesh> pMesh, int nObjects) : CInstancedObjectsShader(move(pMesh), nObjects)
{

}

CChochShader::CChochShader(std::unique_ptr<CMesh> pMesh) : CInstancedObjectsShader(move(pMesh))
{

}

CChochShader::CChochShader(int nObjects) : CInstancedObjectsShader(nObjects)
{

}

CChochShader::~CChochShader()
{

}

void CChochShader::CreateShader(ID3D11Device *pd3dDevice)
{
	UINT nSlot = 0;
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//0
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//1
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },								//2
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,  nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },								//3
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//4
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//5
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },	//6
	};

	UINT nElements = ARRAYSIZE(d3dInputElements);


	CreateVertexShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/VS.cso", d3dInputElements, nElements, &m_pd3dVertexShader);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"../Assets/07.Shader/Instanced/PS-fog.cso", &m_pd3dPixelShader);


}
