#include "stdafx.h"
#include "ContentManager.h"

#include "Mesh.h"
#include "Material.h"
#include "MaterialColors.h"
#include "Texture.h"
#include "InstancedObjectsShader.h"
#include "UIShader.h"
#include "MatrixShader.h"
#include "MaterialColors.h"
#include "BlendShader.h"
#include "BulletShader.h"


#include "BulletComponent.h"

//Mesh
#include "FBXSingleMesh.h"
#include "ShadowMapShader.h"
#include "QuadrangleMesh.h"


CContentManager::CContentManager()
{

	m_pSharedColor = nullptr;

	m_fDefaultMeshSize = 10;


	//Material
	m_vMaterial.clear();

	//vSamplerState
	m_vSamplerState.clear();

	m_pMapArrary = nullptr;

	m_bLoadingInit = false;

	m_pd3dBlendState = nullptr;

	m_mapBlendingBlend.clear();
}

CContentManager::~CContentManager()
{
}

void CContentManager::ReleseInstance()
{
	m_pd3dSamplerState = nullptr;
	m_pMapArrary = nullptr;
	m_bLoadingInit = false;
	m_pSharedColor.reset();
	m_vMaterial.clear();
	return;
	for (auto &pShader : m_vStaticShader)
		pShader->ReleaseObjects();
	m_vStaticShader.clear();

	for (auto &pShader : m_vDynamicShader)
		pShader->ReleaseObjects();
	m_vDynamicShader.clear();

	for (auto &pShader : m_vStaticTerrainShader)
		pShader->ReleaseObjects();
	m_vStaticTerrainShader.clear();

	for (auto &pShader : m_vMatrixShdaer)
		pShader->ReleaseObjects();
	m_vMatrixShdaer.clear();

	for (auto &pShader : m_vBlendShader)
		pShader->ReleaseObjects();
	m_vBlendShader.clear();

}

void CContentManager::Load(ID3D11Device *pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	//SampleState
	m_pd3dSamplerState = nullptr;

	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pd3dSamplerState);

	m_vSamplerState.push_back(m_pd3dSamplerState);

	{
		ID3D11SamplerState*	pd3dSamplerState;
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		pd3dDevice->CreateSamplerState(&samplerDesc, &pd3dSamplerState);

		m_vSamplerState.push_back(pd3dSamplerState);
	}

	{

		ID3D11SamplerState*	pd3dSamplerState;
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		pd3dDevice->CreateSamplerState(&samplerDesc, &pd3dSamplerState);

		m_vSamplerState.push_back(pd3dSamplerState);
	}

	float val = 0.8f;

	//Share Color
	m_pSharedColor = std::make_shared<CMaterialColors>();
	m_pSharedColor->name = "Default_Color";
	m_pSharedColor->m_d3dxcAmbient = XMFLOAT4(val, val, val, 1.0f);
	m_pSharedColor->m_d3dxcDiffuse = XMFLOAT4(val, val, val, 1.0f);
	m_pSharedColor->m_d3dxcSpecular = XMFLOAT4(val, val, val, 1.0f);
	m_pSharedColor->m_d3dxcEmissive = XMFLOAT4(val, val, val, 1.0f);

	//Share Mesh
	//std::shared_ptr<CMesh> pMesh = std::make_shared<CCubeMeshTexturedIlluminated>(m_pd3dDevice, fDefaultMeshSize, fDefaultMeshSize, fDefaultMeshSize);
	//vpMesh.push_back(pMesh);


	//KYT '16.01.21 plus 
	/*
		ShareMaterial 도 미리 하나는 만들자
		추후에 tag, layer 추가할 예정
	*/

	std::vector<std::string> v;
	v.push_back(DEFAULT_TEXTURE_PATH);
	//v.push_back(DEFAULT_NORMA_TEXTURE_PATH);
	CContentManager::MakeMaterial(pd3dDevice,  DEFAULT_TEXTURE_PATH);
	m_vMaterial[0]->name = "Default";

	//KYT '16.02.28
	/*
		Default Shader It is Not mean
	*/

	//Default Shader은 Dynamic임
	//auto pDynamicShader = std::make_shared<CInstancedObjectsShader>();
	//pDynamicShader->CreateShader(pd3dDevice);
	//pDynamicShader->name = "Default";
	//m_vDynamicShader.push_back(move(pDynamicShader));

	//KYT '16.04.03
	/*
		CSV Map Loader
	*/
	m_pMapArrary = Helper::MapData();

	LoadBlendState(pd3dDevice);

}

void CContentManager::FinalLoad(ID3D11Device *pd3dDevice)
{
	//KYT '16.01.18 plus 

	/*
		Instance를 LoadObject에서 만드는데 그 떄 반드시 Mesh는 0이 아니다.
	*/

	//if (vShader) pSharedShader->BuildObjects(pd3dDevice, nullptr);

	//KYT '16.01.20 plus 
	/*
		여기서 만들어준 Shader을 빌드오브젝트 해준 다음에 reset으로 free해준다.
	*/

	if (m_bLoadingInit)
	{
		for (auto &pShader : m_vDynamicShader)pShader->BuildObjects(pd3dDevice);
		return;
	}

	for (auto &pShader : m_vStaticShader) pShader->BuildObjects(pd3dDevice);
	for (auto &pShader : m_vMatrixShdaer) pShader->BuildObjects(pd3dDevice);
	for (auto &pShader : m_vStaticTerrainShader) pShader->BuildObjects(pd3dDevice);
	for (auto &pShader : m_vBlendShader) pShader->BuildObjects(pd3dDevice);

	m_bLoadingInit = true;
	//for (auto &pShader : m_vUIShdaer) pShader->BuildObjects(pd3dDevice);
}

void CContentManager::UILoad(ID3D11Device *pd3dDevice)
{
	for (auto &pShader : m_vUIShdaer) pShader->BuildObjects(pd3dDevice);
}

void CContentManager::ReSetShader(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pd3dDeviceContext)
{
	for (auto& pShader : m_vDynamicShader)
		pShader->CreateShader(pd3dDevice);
	for (auto& pShader : m_vStaticShader)
		pShader->CreateShader(pd3dDevice);
	for (auto& pShader : m_vStaticTerrainShader)
		pShader->CreateShader(pd3dDevice);
	for (auto& pShader : m_vBlendShader)
		pShader->CreateShader(pd3dDevice);

}


//-------------------------------------------------------------------------------
std::unique_ptr<CMesh> CContentManager::GetGetDefaultShaderMesh(float fSize)
{
	if (fSize != 0) m_fDefaultMeshSize = fSize;
	return move(std::make_unique<CCubeMeshTexturedIlluminated>(m_pd3dDevice, m_fDefaultMeshSize, m_fDefaultMeshSize, m_fDefaultMeshSize));
}

std::unique_ptr<CMesh> CContentManager::GetGetDefaultShaderMesh(ID3D11Device *pd3dDevice, float fSize)
{
	return move(std::make_unique<CCubeMeshTexturedIlluminated>(pd3dDevice, fSize, fSize, fSize));
}

std::shared_ptr<CMaterialColors> CContentManager::GetColor()
{
	return m_pSharedColor;
}

std::shared_ptr<CMaterial> CContentManager::GetDefaultMaterial()
{
	return m_vMaterial[0]; //move하면안됌
}

std::shared_ptr<CGameObject> CContentManager::GetDefaultGameObject()
{
	//포인터를 굳이 가지고 있을 필요가 없으므로 move로 넘겨버림
	return move(std::make_shared<CGameObject>());
}

std::shared_ptr<CInstancedObjectsShader> CContentManager::GetDefaultShader()
{
	//move 하면 안됌
	return m_vDynamicShader[0];
}
//-------------------------------------------------------------------------------

std::vector<std::shared_ptr<CInstancedObjectsShader>>CContentManager::GetShaderContainer(SHADER_TYPE type)
{
	if (SHADER_TYPE::SHADER_TYPE_STATIC == type)
		return m_vStaticShader;

	else if (SHADER_TYPE::SHADER_TYPE_DYNAMIC == type)
		return m_vDynamicShader;
	//이런 경우는 없음
	else return m_vStaticShader;
}

std::shared_ptr<CMaterial>
CContentManager::GetMaterial(ID3D11Device* pd3dDevice, std::string path, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	//Material 비어있으면 아마 이런경우는 없을꺼임
	if (m_vMaterial.empty())
	{
		return CContentManager::MakeMaterial(pd3dDevice, path, nStartSampler, nCountSampler, eSampler);
	}
	else
	{
		for (auto &pMaterial : m_vMaterial)
		{
			if (pMaterial->name == path)
				return pMaterial;
		}
		return 	CContentManager::MakeMaterial(pd3dDevice, path, nStartSampler, nCountSampler, eSampler);
	}

}

std::shared_ptr<CMaterial>
CContentManager::GetMaterial(ID3D11Device* pd3dDevice, std::vector<std::string> vPath, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	//Material 비어있으면 아마 이런경우는 없을꺼임
	if (m_vMaterial.empty())
	{
		return CContentManager::MakeMaterial(pd3dDevice, vPath, nStartSampler, nCountSampler, eSampler);
	}
	else
	{
		for (auto &pMaterial : m_vMaterial)
		{
			if (pMaterial->name == vPath[0])
				return pMaterial;
		}
		return 	CContentManager::MakeMaterial(pd3dDevice, vPath, nStartSampler, nCountSampler, eSampler);
	}

}

std::shared_ptr<CMaterial> CContentManager::GetUIMaterial(ID3D11Device * pd3dDevice, std::string path, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	std::string name = path;

	for (auto &pMaterial : m_vMaterial)
	{
		if (pMaterial->name == name)
		{
			name.append(std::to_string(m_vMaterial.size()));
		}
	}

	return 	CContentManager::MakeUIMaterial(pd3dDevice, name, path, nStartSampler, nCountSampler, eSampler);
}

/*private*/
//-------------------------------------------------------------------------------
std::shared_ptr<CTexture> CContentManager::DefaultGetTexture(ID3D11Device* pd3dDevice, std::string name)
{
	HRESULT hResult;
	ID3D11ShaderResourceView *pd3dsrvTexture = nullptr;
	auto pSharedTexture = std::make_shared<CTexture>(1, 1, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER);
	hResult = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T(DEFAULT_TEXTURE_PATH), nullptr, nullptr, &pd3dsrvTexture, nullptr);
	if (FAILED(hResult)) MessageBox(g_hWnd, L"Default Path Texture Error", _T(DEFAULT_TEXTURE_PATH), MB_OK);
	pSharedTexture->SetTexture(0, pd3dsrvTexture);
	pSharedTexture->SetSampler(0, m_pd3dSamplerState);
	pd3dsrvTexture->Release();
	return pSharedTexture;
}

std::shared_ptr<CMaterial> CContentManager::MakeMaterial(ID3D11Device* pd3dDevice, std::string name, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{


	//string = name;
	//TCHAR = path;
	//Material->name 에 경로를 넣어준다.

	HRESULT hResult;

	char* msg;
	size_t tempSize = name.size();
	msg = new char[tempSize];
	strcpy(msg, name.c_str());

	// char* to TCHAR*
	TCHAR path[256] = { 0, };
	int len = strlen(msg);
	::MultiByteToWideChar(CP_ACP, 0, msg, -1, path, len + 1);


	//Share Material
	auto pSharedMaterial = std::make_shared<CMaterial>(m_pSharedColor);
	pSharedMaterial->name = name;
	ID3D11ShaderResourceView *pd3dsrvTexture = nullptr;
	auto pSharedTexture = std::make_shared<CTexture>(1, nCountSampler, PS_SLOT_TEXTURE, nStartSampler);

	try
	{
		hResult = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, path, nullptr, nullptr, &pd3dsrvTexture, nullptr);
	//	throw 1;
	}
	catch (int e)
	{
		std::wstring wstr = path;
		MessageBox(g_hWnd, TEXT("Error Try"), wstr.c_str(), MB_OK);
		
		
		//if(hResult != S_OK)
		//	return MakeMaterial(pd3dDevice, name);
		
	}
	if (pd3dsrvTexture == nullptr)
	{
		::GMessageBox("Texture Error", name);
	}
	if (FAILED(hResult)) ::GMessageBox("Texture Error", name);

	pSharedTexture->SetTexture(0, pd3dsrvTexture);
	for (int i = 0; i < nCountSampler; ++i)
		pSharedTexture->SetSampler(i, m_vSamplerState[eSampler]);
	if (pd3dsrvTexture)pd3dsrvTexture->Release();
	pSharedMaterial->SetTexture(pSharedTexture);
	m_vMaterial.push_back(move(pSharedMaterial));

	//마지막에 넣은걸 뺴야하니
	return m_vMaterial.back();
}

std::shared_ptr<CMaterial> CContentManager::MakeMaterial(ID3D11Device* pd3dDevice, std::vector<std::string> vPath, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	//string = name;
	//TCHAR = path;
	//Material->name 에 경로를 넣어준다.

	HRESULT hResult;

	int n = vPath.size();

	//Share Material
	auto pSharedMaterial = std::make_shared<CMaterial>(m_pSharedColor);
	auto pSharedTexture = std::make_shared<CTexture>(n, nCountSampler, PS_SLOT_TEXTURE, nStartSampler);
	pSharedMaterial->name = vPath[0];

	for (int i = 0; i < n; ++i)
	{
		std::string name = vPath[i];

		//char* msg;
		//int tempSize = name.size();
		//msg = new char[tempSize];
		//strcpy_s(msg, tempSize, name.c_str());

		// char* to TCHAR*
		TCHAR path[256] = { 0, };
		int len = strlen(name.c_str());
		::MultiByteToWideChar(CP_ACP, 0, name.c_str(), -1, path, len + 1);

		ID3D11ShaderResourceView *pd3dsrvTexture = nullptr;
		hResult = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, path, nullptr, nullptr, &pd3dsrvTexture, nullptr);
		if (FAILED(hResult))
		{
			::GMessageBox("Texture Path Error!", vPath[i]);
			continue;
		}
		pSharedTexture->SetTexture(i, pd3dsrvTexture);
		pd3dsrvTexture->Release();
	}

	for (int i = 0; i < nCountSampler; ++i)
		pSharedTexture->SetSampler(i, m_vSamplerState[eSampler]);

	pSharedMaterial->SetTexture(pSharedTexture);

	m_vMaterial.push_back(move(pSharedMaterial));
	//마지막에 넣은걸 뺴야하니
	return m_vMaterial.back();
}

std::shared_ptr<CMaterial> CContentManager::MakeUIMaterial(ID3D11Device * pd3dDevice, std::string name, std::string sPath, int nStartSampler, int nCountSampler, YT_SAMPLER eSampler)
{
	HRESULT hResult;

	char* msg;
	size_t tempSize = sPath.size();
	msg = new char[tempSize];
	strcpy(msg, sPath.c_str());

	// char* to TCHAR*
	TCHAR path[256] = { 0, };
	int len = strlen(msg);
	::MultiByteToWideChar(CP_ACP, 0, msg, -1, path, len + 1);


	//Share Material
	auto pSharedMaterial = std::make_shared<CMaterial>(m_pSharedColor);
	pSharedMaterial->name = name;
	ID3D11ShaderResourceView *pd3dsrvTexture = nullptr;
	auto pSharedTexture = std::make_shared<CTexture>(1, nCountSampler, PS_SLOT_TEXTURE, nStartSampler);

	hResult = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, path, nullptr, nullptr, &pd3dsrvTexture, nullptr);
	if (pd3dsrvTexture == nullptr)
	{
		::GMessageBox("Texture Error", name);
	}
	if (FAILED(hResult)) ::GMessageBox("Texture Error", name);

	pSharedTexture->SetTexture(0, pd3dsrvTexture);
	for (int i = 0; i < nCountSampler; ++i)
		pSharedTexture->SetSampler(i, m_vSamplerState[eSampler]);
	if (pd3dsrvTexture)pd3dsrvTexture->Release();
	pSharedMaterial->SetTexture(pSharedTexture);
	m_vMaterial.push_back(move(pSharedMaterial));

	//마지막에 넣은걸 뺴야하니
	return m_vMaterial.back();
}

//UI Shader
std::shared_ptr<CUIShader> CContentManager::MakeUIShaderFactoty(ID3D11Device* pd3dDevice)
{
	auto pUIShader = std::make_shared<CUIShader>();
	pUIShader->CreateShader(pd3dDevice);
	pUIShader->name = "UI";
	m_vUIShdaer.push_back(pUIShader);
	return (move(pUIShader));
}

void CContentManager::ModifyBlendState()
{
	auto v = Helper::RuntimeDataModify<int>(std::vector<int>(), "blendState", "txt", 10);
	if (v.size() == 0)
	{
		::GMessageBox(L"BlendState Nope Update", L"여기는 ContentManager ModifyBlendState");
		return;
	}

	if (m_pd3dBlendState) m_pd3dBlendState->Release();
	m_pd3dBlendState = nullptr;
	int i = 0;

	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));

	d3dBlendStateDesc.AlphaToCoverageEnable					= static_cast<bool>(v[i++]);			//1
	d3dBlendStateDesc.IndependentBlendEnable				= static_cast<bool>(v[i++]);			//2
	d3dBlendStateDesc.RenderTarget[0].BlendEnable			= static_cast<bool>(v[i++]);			//3

	d3dBlendStateDesc.RenderTarget[0].SrcBlend				= static_cast<D3D11_BLEND>(v[i++]);		//4
	d3dBlendStateDesc.RenderTarget[0].DestBlend				= static_cast<D3D11_BLEND>(v[i++]);		//5
	d3dBlendStateDesc.RenderTarget[0].BlendOp				= static_cast<D3D11_BLEND_OP>(v[i++]);	//6

	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha			= static_cast<D3D11_BLEND>(v[i++]);		//7
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha		= static_cast<D3D11_BLEND>(v[i++]);		//8
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha			= static_cast<D3D11_BLEND_OP>(v[i++]);	//9

	d3dBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = static_cast<UINT8>(v[i++]);			//10
	if (m_pd3dDevice) m_pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_pd3dBlendState);

	if (m_pd3dBlendState)SOUNDMGR->Play("스타트");


}

void CContentManager::LoadBlendState(ID3D11Device* pd3dDevice)
{
	AddBlendState
	(
		pd3dDevice,
		"Default",

		true,
		false,
		false,

		D3D11_BLEND_ONE,
		D3D11_BLEND_ONE,
		D3D11_BLEND_OP_ADD,

		D3D11_BLEND_ONE,
		D3D11_BLEND_ZERO,
		D3D11_BLEND_OP_ADD,

		D3D11_COLOR_WRITE_ENABLE_ALL
	);
	AddBlendState
	(
		pd3dDevice,
		"UINoBlend",

		true,
		false,
		false,

		D3D11_BLEND_ONE,
		D3D11_BLEND_ONE,
		D3D11_BLEND_OP_ADD,

		D3D11_BLEND_ONE,
		D3D11_BLEND_ZERO,
		D3D11_BLEND_OP_ADD,

		D3D11_COLOR_WRITE_ENABLE_ALL
	);

	AddBlendState
	(
		pd3dDevice
		, "UI_Translucence"

		, true
		, false
		, true

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ONE
		, D3D11_BLEND_OP_MIN

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ZERO
		, D3D11_BLEND_OP_ADD

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);

	AddBlendState
	(
		  pd3dDevice
		, "Bush"

		, false
		, false
		, true

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ZERO
		, D3D11_BLEND_OP_ADD

		, D3D11_BLEND_ONE	//3이어야 하는데
		, D3D11_BLEND_ONE
		, D3D11_BLEND_OP_REV_SUBTRACT

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);

	AddBlendState
	(
		pd3dDevice
		, "CubeItem"

		, false
		, true
		, true

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ONE
		, D3D11_BLEND_OP_MAX

		, D3D11_BLEND_ZERO
		, D3D11_BLEND_ONE
		, D3D11_BLEND_SRC_COLOR

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);

	AddBlendState
	(
		pd3dDevice
		, "Particle"

		, false
		, false
		, true

		, D3D11_BLEND_SRC_ALPHA
		, D3D11_BLEND_ONE
		, D3D11_BLEND_OP_ADD

		, D3D11_BLEND_ZERO
		, D3D11_BLEND_ZERO
		, D3D11_BLEND_OP_ADD

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);

	AddBlendState
	(
		  pd3dDevice
		, "Billboard"

		, true
		, false
		, false

		, D3D11_BLEND_SRC_ALPHA
		, D3D11_BLEND_INV_SRC_ALPHA
		, D3D11_BLEND_OP_ADD

		, D3D11_BLEND_SRC_ALPHA
		, D3D11_BLEND_INV_SRC_ALPHA
		, D3D11_BLEND_OP_ADD

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);


	AddBlendState
	(
		  pd3dDevice
		, "Billboard"

		, true
		, false
		, true

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ONE
		, D3D11_BLEND_OP_ADD

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ZERO
		, D3D11_BLEND_OP_ADD

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);


	AddBlendState
	(
		pd3dDevice
		, "ParticleBlend"

		, false
		, false
		, true

		, D3D11_BLEND_SRC_ALPHA
		, D3D11_BLEND_INV_SRC_ALPHA
		, D3D11_BLEND_OP_ADD

		, D3D11_BLEND_ZERO
		, D3D11_BLEND_ZERO
		, D3D11_BLEND_OP_ADD

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);


	AddBlendState
	(
		pd3dDevice
		, "WaterFall"

		, 1
		, 0
		, 1

		, 2
		, 2
		, 5

		, 2
		, 2
		, 5

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);


	AddBlendState
	(
		pd3dDevice
		, "Water"

		, true
		, false
		, true

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ONE
		, D3D11_BLEND_OP_ADD

		, D3D11_BLEND_ONE
		, D3D11_BLEND_ZERO
		, D3D11_BLEND_OP_ADD

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);

	AddBlendState
	(
		pd3dDevice
		, "BillBoardSprite"

		, false
		, true
		, true

		, 2
		, 2
		, 4

		, 2
		, 1
		, 3

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);


	AddBlendState
	(
		pd3dDevice
		, "Gauge"

		, 1
		, 0
		, 0

		, 2
		, 2
		, 1

		, 2
		, 2
		, 1

		, D3D11_COLOR_WRITE_ENABLE_ALL
	);


	AddBlendState
	(
		pd3dDevice
		, "BillBoardAlpha"

		, 1
		, 0
		, 1

		, 2
		, 2
		, 4

		, 2
		, 1
		, 1

		, 15
	);


	AddBlendState
	(
		pd3dDevice
		, "WaterFall_Floor"
		, 0
		, 1
		, 1
		, 1
		, 2
		, 5
		, 2
		, 2
		, 3
		, 14
	);


	AddBlendState
	(
		pd3dDevice
		, "SmashParticle"
		, 0
		, 1
		, 1

		, 2
		, 2
		, 5

		, 1
		, 2
		, 3

		, 15
	);
}

void CContentManager::DebugginBlendingState(ID3D11DeviceContext *pd3dDeviceContext, bool start)
{
	if (!m_pd3dBlendState) return;
	if(start) pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, nullptr, 0xffffffff);
	else  pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

ID3D11BlendState* CContentManager::FindBlendState(std::string str)
{
	auto b = m_mapBlendingBlend.find(str);

	if (b == m_mapBlendingBlend.end())
		return nullptr;
	else
		return b->second;
}

void CContentManager::OMSetBlendingState(ID3D11DeviceContext * pd3dDeviceContext, std::string name)
{
	auto state = FindBlendState(name);
	if (nullptr == state) return;
	pd3dDeviceContext->OMSetBlendState(state, nullptr, 0xffffffff);
}

void CContentManager::AddBlendState
(	
	  ID3D11Device* pd3dDevice
	, std::string	name
	, bool			AlphaToCoverageEnable
	, bool			IndependentBlendEnable
	, bool			BlendEnable
	, UINT			SrcBlend
	, UINT			DestBlend
	, UINT			BlendOp
	, UINT			SrcBlendAlpha
	, UINT			DestBlendAlpha
	, UINT			BlendOpAlpha
	, UINT			RenderTargetWriteMask
)
{
	if (FindBlendState(name)) return;
	ID3D11BlendState* blend;
	blend = nullptr;
	D3D11_BLEND_DESC desc;

	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable					= static_cast<BOOL>(AlphaToCoverageEnable);
	desc.IndependentBlendEnable					= static_cast<BOOL>(IndependentBlendEnable);
	desc.RenderTarget[0].BlendEnable			= static_cast<BOOL>(BlendEnable);
	desc.RenderTarget[0].SrcBlend				= static_cast<D3D11_BLEND>(SrcBlend);
	desc.RenderTarget[0].DestBlend				= static_cast<D3D11_BLEND>(DestBlend);
	desc.RenderTarget[0].BlendOp				= static_cast<D3D11_BLEND_OP>(BlendOp);
	desc.RenderTarget[0].SrcBlendAlpha			= static_cast<D3D11_BLEND>(SrcBlendAlpha);
	desc.RenderTarget[0].DestBlendAlpha			= static_cast<D3D11_BLEND>(DestBlendAlpha);
	desc.RenderTarget[0].BlendOpAlpha			= static_cast<D3D11_BLEND_OP>(BlendOpAlpha);
	desc.RenderTarget[0].RenderTargetWriteMask  = static_cast<UINT8>(RenderTargetWriteMask);
	
	if (m_pd3dDevice)m_pd3dDevice->CreateBlendState(&desc, &blend);
	m_mapBlendingBlend.insert(std::make_pair(name, blend));

}






