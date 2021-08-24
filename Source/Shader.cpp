//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "Scene.h"

CShader::CShader() : Object()
{

	m_pd3dVertexShader	 = nullptr;
	m_pd3dVertexLayout	 = nullptr;
	m_pd3dPixelShader	 = nullptr;
	m_pd3dGeometryShader = nullptr;
	m_pd3dHullShader	 = nullptr;
	m_pd3dDomainShader   = nullptr;
	m_nType = 0x00;
	
	//공간분할
	m_vRenderObject.clear();

	m_pd3dVertexLayout = nullptr;
	m_pd3dVertexShader = nullptr;
	m_pd3dSOVertexShader = nullptr;
	m_pd3dDomainShader = nullptr;
	m_pd3dHullShader = nullptr;
	m_pd3dComputeShader = nullptr;
	m_pd3dInputElementDescs = nullptr;
	m_pd3dSOGeometryShader = nullptr;
}

CShader::~CShader()
{
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	m_pd3dVertexLayout = nullptr;

	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	m_pd3dVertexShader = nullptr;

	if (m_pd3dGeometryShader) m_pd3dGeometryShader->Release();
	m_pd3dGeometryShader = nullptr;

	if (m_pd3dSOVertexShader) m_pd3dSOVertexShader->Release();
	m_pd3dSOVertexShader = nullptr;

	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	m_pd3dPixelShader = nullptr;

	if (m_pd3dSOGeometryShader) m_pd3dSOGeometryShader->Release();
	m_pd3dSOGeometryShader = nullptr;
	
	if (m_pd3dDomainShader) m_pd3dDomainShader->Release();
	m_pd3dDomainShader = nullptr;

	if (m_pd3dHullShader) m_pd3dHullShader->Release();
	m_pd3dHullShader = nullptr;

	if (m_pd3dComputeShader) m_pd3dComputeShader->Release();
	m_pd3dComputeShader = nullptr;

	if (m_pd3dInputElementDescs) delete m_pd3dInputElementDescs;
	m_pd3dInputElementDescs = nullptr;
}

void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputElements, UINT nElements, ID3D11InputLayout **ppd3dInputLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dVertexShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dVertexShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreateVertexShader(pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), nullptr, ppd3dVertexShader);
		pd3dDevice->CreateInputLayout(pd3dInputElements, nElements, pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), ppd3dInputLayout);
		pd3dVertexShaderBlob->Release();
	}
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			size_t size = strlen(pszShaderName);
			TCHAR* tStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, size, tStr, 256);
			tStr[size] = NULL;
			MessageBox(NULL, tStr, pszFileName, MB_OK);
		}
		else
			MessageBox(NULL, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
}
void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dPixelShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreatePixelShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), nullptr, ppd3dPixelShader);
		pd3dPixelShaderBlob->Release();
	}
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			size_t size = strlen(pszShaderName);
			TCHAR* tStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, size, tStr, 256);
			tStr[size] = NULL;
			MessageBox(NULL, tStr, pszFileName, MB_OK);
		}
		else
			MessageBox(NULL, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}

}
void CShader::CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dPixelShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreateGeometryShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), nullptr, ppd3dGeometryShader);
		pd3dPixelShaderBlob->Release();
	}	
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			size_t size = strlen(pszShaderName);
			TCHAR* tStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, size, tStr, 256);
			tStr[size] = NULL;
			MessageBox(NULL, tStr, pszFileName, MB_OK);
		}
		else
			MessageBox(NULL, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
}
void CShader::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dPixelShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreateGeometryShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), nullptr, ppd3dGeometryShader);
		pd3dPixelShaderBlob->Release();
	}
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			size_t size = strlen(pszShaderName);
			TCHAR* tStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, size, tStr, 256);
			tStr[size] = NULL;
			MessageBox(NULL, tStr, pszFileName, MB_OK);
		}
		else
			MessageBox(NULL, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
}
void CShader::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader, SO_Geometry_INFO so)
{
	HRESULT hResult;

	ID3DBlob *pd3dShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0, nullptr, &pd3dShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreateGeometryShaderWithStreamOutput(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(),
			so.pSODecls, so.NumEntries, so.pBufferStrides, so.NumStrides, so.RasterizedStream, nullptr, &m_pd3dSOGeometryShader);
		pd3dShaderBlob->Release();
	}
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			size_t size = strlen(pszShaderName);
			TCHAR* tStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, size, tStr, 256);
			tStr[size] = NULL;
			MessageBox(NULL, tStr, pszFileName, MB_OK);
		}
		else
			MessageBox(NULL, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
}
void CShader::CreateHullShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11HullShader **ppd3dHullShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dPixelShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreateHullShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), nullptr, ppd3dHullShader);
		pd3dPixelShaderBlob->Release();
	}
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			size_t size = strlen(pszShaderName);
			TCHAR* tStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, size, tStr, 256);
			tStr[size] = NULL;
			MessageBox(NULL, tStr, pszFileName, MB_OK);
		}
		else
			MessageBox(NULL, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
}
void CShader::CreateDomainShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11DomainShader **ppd3dDomainShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dPixelShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreateDomainShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), nullptr, ppd3dDomainShader);
		pd3dPixelShaderBlob->Release();
	}
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			size_t size = strlen(pszShaderName);
			TCHAR* tStr = new TCHAR[size];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszShaderName, size, tStr, 256);
			tStr[size] = NULL;
			MessageBox(NULL, tStr, pszFileName, MB_OK);
		}
		else
			MessageBox(NULL, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
}
void CShader::CreateComputeShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11ComputeShader **ppd3dComputehader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob *pd3dPixelShaderBlob = nullptr, *pd3dErrorBlob = nullptr;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName, pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dPixelShaderBlob, &pd3dErrorBlob, nullptr)))
	{
		pd3dDevice->CreateComputeShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), nullptr, ppd3dComputehader);
		pd3dPixelShaderBlob->Release();
	}
	if (FAILED(hResult))
	{
		if (pszShaderName)
		{
			MessageBox(nullptr, ::tChar(pszShaderName), pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
}

//Prepare ShaderCompile

void CShader::CreateVertexShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11VertexShader **ppd3dVertexShader)
{
	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreateVertexShader(pByteCode, nReadBytes, nullptr, ppd3dVertexShader);
	pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pByteCode, nReadBytes, &m_pd3dVertexLayout);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(nullptr, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;
}
void CShader::CreateSOVertexShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11VertexShader **ppd3dVertexShader, ID3D11InputLayout *ppInputLayout)
{
	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreateVertexShader(pByteCode, nReadBytes, nullptr, ppd3dVertexShader);
	pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pByteCode, nReadBytes, &ppInputLayout);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(nullptr, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"SO 쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;
}
void CShader::CreateGeoMoetryShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, ID3D11GeometryShader **ppd3dGeomeotryShader)
{
	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreateGeometryShader(pByteCode, nReadBytes, nullptr, ppd3dGeomeotryShader);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(nullptr, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;
}
void CShader::CreatePixelShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, ID3D11PixelShader **ppd3dPixelShader)
{
	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreatePixelShader(pByteCode, nReadBytes, nullptr, ppd3dPixelShader);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(nullptr, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;
}
void CShader::CreateComputeShaderFromCompiledFile(ID3D11Device * pd3dDevice, WCHAR * pszFileName, ID3D11ComputeShader ** ppd33dComputeShader)
{
	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreateComputeShader(pByteCode, nReadBytes, nullptr, ppd33dComputeShader);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(nullptr, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;
}
void CShader::CreateDomainShaderFromCompiledFile(ID3D11Device * pd3dDevice, WCHAR * pszFileName, ID3D11DomainShader** ppd33dDomainShader)
{
	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreateDomainShader(pByteCode, nReadBytes, nullptr, ppd33dDomainShader);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(nullptr, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;
}
void CShader::CreateHullShaderFromCompiledFile(ID3D11Device * pd3dDevice, WCHAR * pszFileName, ID3D11HullShader ** ppd33dHullShader)
{
	int nReadBytes = 0;
	BYTE *pByteCode = Helper::ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreateHullShader(pByteCode, nReadBytes, nullptr, ppd33dHullShader);
	if (FAILED(hResult))
	{
		if (pszFileName)
		{
			MessageBox(nullptr, pszFileName, pszFileName, MB_OK);
		}
		else
			MessageBox(nullptr, L"쉐이더 이름이 안들어왔어용", pszFileName, MB_OK);
	}
	if (pByteCode) delete[] pByteCode;
}



void CShader::CreateShader(ID3D11Device *pd3dDevice, UINT nType)
{
	m_nType |= nType;
	GetInputElementDesc(m_nType);
	LPCSTR pszVSShaderName = nullptr, pszVSShaderModel = "vs_5_0", pszPSShaderName = nullptr, pszPSShaderModel = "ps_5_0";
	GetShaderName(m_nType, &pszVSShaderName, &pszVSShaderModel, &pszPSShaderName, &pszPSShaderModel);
	CreateShader(pd3dDevice, nullptr, 0, L"Effect.fx", pszVSShaderName, pszVSShaderModel, pszPSShaderName, pszPSShaderModel);
}

void CShader::CreateShader(ID3D11Device *pd3dDevice, D3D11_INPUT_ELEMENT_DESC *pd3dInputElementDesc, int nInputElements, WCHAR *pszFileName, LPCSTR pszVSShaderName, LPCSTR pszVSShaderModel, LPCSTR pszPSShaderName, LPCSTR pszPSShaderModel)
{
	CreateVertexShaderFromFile(pd3dDevice, pszFileName, pszVSShaderName, pszVSShaderModel, &m_pd3dVertexShader, (pd3dInputElementDesc) ? pd3dInputElementDesc : m_pd3dInputElementDescs, (pd3dInputElementDesc) ? nInputElements : m_nInputElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, pszFileName, pszPSShaderName, pszPSShaderModel, &m_pd3dPixelShader);
}

void CShader::CreateConstBuffers(ID3D11Device *pd3dDevice)
{
}

void CShader::ReleaseShaderVariables()
{

}

void CShader::GetInputElementDesc(UINT nVertexElementType)
{
	m_nInputElements = 0;
	if (nVertexElementType & VERTEX_POSITION_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_COLOR_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_NORMAL_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_0) m_nInputElements++;
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_1) m_nInputElements++;
	if (nVertexElementType & VERTEX_BONE_ID_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_BONE_WEIGHT_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_INSTANCING_ELEMENT) m_nInputElements += 4;
	m_pd3dInputElementDescs = new D3D11_INPUT_ELEMENT_DESC[m_nInputElements];
	UINT nIndex = 0, nSlot = 0;
	if (nVertexElementType & VERTEX_POSITION_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_COLOR_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_NORMAL_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_0) m_pd3dInputElementDescs[nIndex++] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_1) m_pd3dInputElementDescs[nIndex++] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_BONE_ID_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_SINT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_BONE_WEIGHT_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_INSTANCING_ELEMENT)
	{
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
	}
}

void CShader::GetShaderName(UINT nVertexElementType, LPCSTR *ppszVSShaderName, LPCSTR *ppszVSShaderModel, LPCSTR *ppszPSShaderName, LPCSTR *ppszPSShaderModel)
{
	int nInputElements = 0, nIndex = 0;
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT)) { *ppszVSShaderName = "VSDiffusedColor", *ppszPSShaderName = "PSDiffusedColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT)) { *ppszVSShaderName = "VSLightingColor", *ppszPSShaderName = "PSLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { *ppszVSShaderName = "VSTexturedColor", *ppszPSShaderName = "PSTexturedColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1)) { *ppszVSShaderName = "VSDetailTexturedColor", *ppszPSShaderName = "PSDetailTexturedColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { *ppszVSShaderName = "VSTexturedLightingColor", *ppszPSShaderName = "PSTexturedLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1)) { *ppszVSShaderName = "VSDetailTexturedLightingColor", *ppszPSShaderName = "PSDetailTexturedLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_BONE_ID_ELEMENT | VERTEX_BONE_WEIGHT_ELEMENT)) { *ppszVSShaderName = "VSSkinnedLightingColor", *ppszPSShaderName = "PSLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_BONE_ID_ELEMENT | VERTEX_BONE_WEIGHT_ELEMENT)) { *ppszVSShaderName = "VSSkinnedTexturedLightingColor", *ppszPSShaderName = "PSTexturedLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_INSTANCING_ELEMENT)) { *ppszVSShaderName = "VSInstancedTexturedLightingColor", *ppszPSShaderName = "PSInstancedTexturedLightingColor"; }
}

void CShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->GSSetShader(m_pd3dGeometryShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);
	 
	pd3dDeviceContext->HSSetShader(m_pd3dHullShader, nullptr, 0);
	pd3dDeviceContext->DSSetShader(m_pd3dDomainShader, nullptr, 0);
}

void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);
}

ID3D11Buffer *CShader::CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = d3dUsage;
	d3dBufferDesc.ByteWidth = nStride * nElements;
	d3dBufferDesc.BindFlags = nBindFlags;
	d3dBufferDesc.CPUAccessFlags = nCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;

	ID3D11Buffer *pd3dInstanceBuffer = nullptr;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, (pBufferData) ? &d3dBufferData : nullptr, &pd3dInstanceBuffer);
	return(pd3dInstanceBuffer);
}

//SpacePartitionRender
void CShader::SpacePartitioningRender(ID3D11DeviceContext *pd3dDeviceContext,std::shared_ptr<CCamera> pCamera)
{
	OnPrepareRender(pd3dDeviceContext);
}

void CShader::PushRenderObject(std::shared_ptr<CGameObject> object)
{
	m_vRenderObject.push_back(object);
}

//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
CObjectsShader::CObjectsShader(int nObjects)
{
	//KYT '16.01.25 plus
	/*디폴트 오브젝트 개수 설정*/
	m_nObjects = 10000;
	
	
	//m_ppObjects = nullptr;
	if(nObjects > m_nObjects) m_nObjects = nObjects;



	//if (m_nObjects > 0)
	//{
	//	m_ppObjects = new CGameObject*[m_nObjects];
	//	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i] = nullptr;
	//}
	//
	//m_pMaterial = nullptr;
	//m_pContext = nullptr;
	//if (nObjects > 0) m_vsObjects.reserve(nObjects);

	m_nIndexToAdd = 0;

	//KYT '16.01.20 plus
	/*Material 를 생성자에서 넣어준다.*/
	m_sMaterial = CONTENTMGR->GetDefaultMaterial();
	//m_sMaterial = nullptr;

	//KYT '16.01.20 plus
	/*Default Object를 사용하는지 안하는지 판단.*/
	//m_bIsDefaultUseObject = true;
	//m_vsObjects.push_back(CONTENTMGR->GetDefaultGameObject());
}

CObjectsShader::~CObjectsShader()
{
}

//KYT '16.01.25 minus
/*
Shader에서 Object Vector 가지고 있는걸 포기한다.
*/
//void CObjectsShader::SetObject(int nIndex, std::shared_ptr<CGameObject> pGameObject)
//{	
//	//KYT '16.01.20 plus
//	/* Default Object를 사용하지 않으니 false 넣는다.*/
//	m_bIsDefaultUseObject = false;
//
//	m_vsObjects[nIndex] = pGameObject;
//	pGameObject->SetShader((std::shared_ptr<CShader>)(this));
//
//#ifdef _SPACEPARTITIONING
//		CSpacePartition->AddObject(pGameObject);
//#endif
//}
//
//
//void CObjectsShader::SetEnviornmentObject(int nIndex, std::shared_ptr<CGameObject> pGameObject)
//{
//	pGameObject->SetShader((std::shared_ptr<CShader>)(this));
//	m_vsObjects[nIndex] = pGameObject;
//}
//
//void CObjectsShader::AddEnviornmentObject(std::shared_ptr<CGameObject> pGameObject)
//{	
//	//KYT '16.01.20 plus
//	/*일단 기본 Object를 삭제하고,
//	Default Object를 사용하지 않으니 false 넣는다.*/
//	if (m_bIsDefaultUseObject)m_vsObjects.clear();
//	m_bIsDefaultUseObject = false;
//
//	pGameObject->SetShader((std::shared_ptr<CShader>)(this));
//	m_nIndexToAdd++;
//	m_vsObjects.push_back(pGameObject);
//}


//KYT '16.01.25 change
/*
 RenderVector에 추가해준다.
*/
void CObjectsShader::AddObject(std::shared_ptr<CGameObject> pGameObject)
{
	m_vRenderObject.push_back(pGameObject);
}

void CObjectsShader::SetMaterial(std::shared_ptr<CMaterial> pMaterial)
{
	m_sMaterial = pMaterial;
}

void CObjectsShader::SetMaterial(ID3D11Device *pd3dDevice, std::string path, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	m_sMaterial = CONTENTMGR->GetMaterial(pd3dDevice, path, nStartSampler, nCountSampler, eSampler);
}

void CObjectsShader::SetMaterial(ID3D11Device *pd3dDevice, std::vector<std::string> vPath, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	m_sMaterial = CONTENTMGR->GetMaterial(pd3dDevice, vPath, nStartSampler, nCountSampler, eSampler);
}

void CObjectsShader::SetUIMaterial(ID3D11Device * pd3dDevice, std::string path, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	m_sMaterial = CONTENTMGR->GetUIMaterial(pd3dDevice, path, nStartSampler, nCountSampler, eSampler);
}

void CObjectsShader::SetMaterial_NormalNBump(ID3D11Device *pd3dDevice, std::string texture, std::string normal, std::string bump)
{
	std::vector<std::string> vPath;
	vPath.push_back(texture);
	vPath.push_back(normal);
	vPath.push_back(bump);
	m_sMaterial = CONTENTMGR->GetMaterial(pd3dDevice, vPath);
}
 
void CObjectsShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	m_pContext = pContext;
}

void CObjectsShader::ReleaseObjects()
{

}

void CObjectsShader::AnimateObjects(float fTimeElapsed)
{
	//KYT '16.01.25 minus
	/*
		Shader에서 Object Vector 가지고 있는걸 포기한다.
		고로 Animate도 변해야 한다.
	*/

	//for (auto &p : m_vsObjects)
	//{
	//	if (!p) break;
	//	p->Animate(fTimeElapsed, nullptr);
	//}
	
	//for (auto &pObject : m_vRenderObject)
	//{
	//	if (!pObject) break;
	//	pObject->Animate(fTimeElapsed, nullptr);
	//	
	//	//KYT '16.03.01
	//	/*UPDATE 부분의 위치를 바꾼다*/
	//	SPACE->Update(pObject);
	//
	//}

	for (auto &p : m_vRenderObject)
	{
		p->Animate(fTimeElapsed);
		p->Update(nullptr);
	}
}

void CObjectsShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	CShader::OnPrepareRender(pd3dDeviceContext);
}

void CObjectsShader::Render(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	CShader::Render(pd3dDeviceContext, pCamera);
	
	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);
	

	//KYT '16.01.25 minus
	/*
		Shader에서 Object Vector 가지고 있는걸 포기한다.
		고로 Render도 변해야 한다.
	*/

	for (auto &pObject : m_vRenderObject)
	{
		#ifdef _SPACEPARTITIONING
			pObject->Render(pd3dDeviceContext, pCamera);
		#else
			if (pObject->IsVisible(pCamera))
			pObject->Render(pd3dDeviceContext, pCamera);
		#endif
	}
	
	//그려줬으니 초기화를 해야한다.
	m_vRenderObject.clear();


	//KYT '16.01.25 minus
	/*
		Shader에서 Object Vector 가지고 있는걸 포기한다.
		고로 Render도 변해야 한다.
	*/
	//
	//for (int j = 0; j < m_nIndexToAdd; j++)
	//{
	//	if (m_vsObjects[j])
	//	{
	//		if (m_vsObjects[j]->IsVisible(pCamera))
	//		{
	//			m_vsObjects[j]->Render(pd3dDeviceContext, pCamera);
	//		}
	//	}
	//}
}

CGameObject *CObjectsShader::PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo)
{
	//KYT '16.01.25 minus
	/*
	 미사용으로
	*/

	//int nIntersected = 0;
	//float fNearHitDistance = FLT_MAX;
	//CGameObject *pSelectedObject = nullptr;
	//MESHINTERSECTINFO d3dxIntersectInfo;
	//for (int i = 0; i < m_nIndexToAdd; i++)
	//{
	//	nIntersected = m_vsObjects[i]->PickObjectByRayIntersection(pd3dxvPickPosition, pd3dxmtxView, &d3dxIntersectInfo);
	//	if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
	//	{
	//		fNearHitDistance = d3dxIntersectInfo.m_fDistance;
	//		pSelectedObject = m_vsObjects[i].get();
	//		if (pd3dxIntersectInfo) *pd3dxIntersectInfo = d3dxIntersectInfo;
	//	}
	//}
	//return(pSelectedObject);
	return nullptr;
}


//KYT '12.02.08
/*이제안쓰니 제거*/
//SpacePartitionRender
/*
void CObjectsShader::SpacePartitioningRender(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
	CShader::Render(pd3dDeviceContext, pCamera);

	if (m_sMaterial) m_sMaterial->UpdateConstBuffer(pd3dDeviceContext);

	for (auto p : m_vRenderObject)
	{
		p->IsVisible(pCamera);
		p->Render(pd3dDeviceContext, pCamera);
	}

	m_vRenderObject.clear();
}

void CObjectsShader::SpacePartitioningFrustm(ID3D11DeviceContext *pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{

}
*/
//--------------------------------------------------------------------

