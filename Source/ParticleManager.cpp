
#include "stdafx.h"

#include "ParticleManager.h"
#include "CubeParticleShader.h"
#include "QuadParticleShader.h"
#include "WaterFallShader.h"

#include "QuadrangleMesh.h"
#include "BillBoardSpriteShader.h"
#include "BillBoardDisappearShader.h"
#include "FlowerParticleShader.h"


#define _PARTILCE_ON

CParticleManager::CParticleManager()
{
	m_mParticleShader.clear();
	m_fSpeed		= PARTICLE_SPEED;
	m_fSize			= PARTICLE_SIZE;
	m_fDuringTime	= PARTICLE_DURINGTIME;
	m_fEmitCount	= PARTICLE_EMIT_COUNT;
}

CParticleManager::~CParticleManager()
{
}

void CParticleManager::ReleseInstance()
{
	for (auto &pShader : m_mParticleShader)
	{
		auto v = pShader.second;

		for (auto pShader : v)
		{
			pShader->Release();
			pShader->ReleaseShader();
		}
	}

	//CSingleTonBase::ReleseInstance();
}

void CParticleManager::Load(ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	m_nWarerFallLoop = 1;
	m_nWaterFallMaxEmitCount = 2;
	m_nWaterFallBottom = 1;


	#ifdef _PARTILCE_ON
		AddWaterFallPartcielhader("폭포", 27 * 4 * 7, 8.0, 0.5, 6.0f, 49, XMFLOAT3(0, 0, 0), L"../Assets/03_Image/Particle/custom_particle4.png");

		
		AddCubePartcielShader("스매시", 10, m_fSpeed, m_fSize, m_fDuringTime, 30, XMFLOAT3(0.0f, 0.0f, 0.0f), L"../Assets/03_Image/Particle/Smoke.png");
		AddCubePartcielShader("텔레포트", 30, PARTICLE_SPEED, PARTICLE_SIZE, PARTICLE_DURINGTIME, 30, XMFLOAT3(0.0f, 0.0f, 0.0f), L"../Assets/03_Image/Particle/Smash.png");
		AddCubePartcielShader("하이드", 30, PARTICLE_SPEED, PARTICLE_SIZE, PARTICLE_DURINGTIME, 30, XMFLOAT3(0.0f, 0.0f, 0.0f), L"../Assets/03_Image/Particle/Smash.png");
	#endif

	AddAnimationSpriteShader("보라불꽃", "../Assets/03_Image/Sprite/awaken_create.png", "BillBoardSprite",XMFLOAT2(2, 2), 1, 11);
	AddAnimationSpriteShader("스매시", "../Assets/03_Image/Sprite/smash_effect_sprite2.png","SmashParticle", XMFLOAT2(0.8, 0.8), 2.0f, 7.0f);
	AddBillBoardEffectShader("미스", "../Assets/03_Image/Sprite/Miss.png", XMFLOAT2(0.6f, 0.2f));



}

void CParticleManager::CreateConstBuffer(ID3D11Device * pd3dDevice)
{
	XMFLOAT3 p = m_pCamera->GetPositionXMFLOAT3();

	CameraPosition* camera = new CameraPosition();
	camera->gf4Position = XMFLOAT4(p.x, p.y, p.z, 1.0f);


	//Create Bulr Const Buffer 
	D3D11_BUFFER_DESC pd3dDesc;
	ZeroMemory(&pd3dDesc, sizeof(pd3dDesc));
	pd3dDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDesc.ByteWidth = sizeof(CameraPosition);
	pd3dDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pd3dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dFogBufferData;
	ZeroMemory(&d3dFogBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dFogBufferData.pSysMem = camera;
	pd3dDevice->CreateBuffer(&pd3dDesc, &d3dFogBufferData, &m_pd3dcbCameraPosition);

	if (camera)delete camera;
	camera = nullptr;
}

void CParticleManager::UpdateConstBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	auto p = m_pCamera->GetPositionXMFLOAT3();
	XMFLOAT4 pos = XMFLOAT4(p.x, p.y, p.z, 1.0f);
	
	#ifdef DEBUG_MODE_UI
		FONTMGR->MessagePush("카툰", 50, 50, 30, L"%1.f %1.f %1.f", pos.x, pos.y, pos.z);
	#endif

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbCameraPosition, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	CameraPosition *cCameraPos = (CameraPosition*)d3dMappedResource.pData;
	cCameraPos->gf4Position = pos;
	pd3dDeviceContext->Unmap(m_pd3dcbCameraPosition, 0);
	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
}

void CParticleManager::PushParticleContainer(std::string name, std::shared_ptr<CCubeParticleShader> pParticle)
{
	auto &vContainer = FindParticleContainer(name);
	if (vContainer.size() == 0)
	{
		std::vector<std::shared_ptr<CCubeParticleShader>> vectr;
		vectr.push_back(pParticle);
		m_mParticleShader.insert(std::make_pair(name, vectr));
	}
	else
	{
		vContainer.push_back(pParticle);
	}
}

std::vector<std::shared_ptr<CCubeParticleShader>>& CParticleManager::FindParticleContainer(std::string name)
{
	auto iter = m_mParticleShader.find(name);
	if (iter == m_mParticleShader.end())
	{
		std::vector<std::shared_ptr<CCubeParticleShader>> v;
		return v;
	}
	else
	{
		return iter->second;
	}
}


void CParticleManager::AddCubePartcielShader(std::string name, int nCount)
{
	std::vector<std::shared_ptr<CCubeParticleShader>> vContainer;
	for (int i = 0; i < nCount; ++i)
	{
		auto pShader = std::make_shared<CCubeParticleShader>();
		pShader->Initialize(m_pd3dDevice, _T("../Assets/03_Image/Particle/Smash.png"), XMFLOAT3(0.0f, 0.0f, 0.0f), 30);
		pShader->ParticleStop();
		pShader->CreateShader(m_pd3dDevice);
		vContainer.push_back(pShader);
		vContainer.push_back(pShader);
	}
	m_mParticleShader.insert(std::make_pair(name, vContainer));
}

void CParticleManager::AddCubePartcielShader(std::string name, int nCount, float fSpeed, float fSize, float fAge, int nMaxPariticle, XMFLOAT3 fRandPosition, TCHAR* path)
{
	std::vector<std::shared_ptr<CCubeParticleShader>> vContainer;
	for (int i = 0; i < nCount; ++i)
	{
		auto pShader = std::make_shared<CCubeParticleShader>();
		pShader->Initialize(m_pd3dDevice, path, XMFLOAT3(0.0f, 0.0f, 0.0f), nMaxPariticle);
		pShader->ParticleStop();
		pShader->CreateShader(m_pd3dDevice);
		pShader->SetParitlceData(fSpeed, fSize, fAge, 1, fRandPosition);
		vContainer.push_back(pShader);
	}
	m_mParticleShader.insert(std::make_pair(name, vContainer));
}

void CParticleManager::AddQuadPartcieShader(std::string name, int nCount)
{
	std::vector<std::shared_ptr<CCubeParticleShader>> vContainer;
	for (int i = 0; i < nCount; ++i)
	{
		auto pShader = std::make_shared<CQuadParticleShader>();
		pShader->Initialize(m_pd3dDevice, _T("../Assets/03_Image/Particle/Smash.png"), XMFLOAT3(0.0f, 0.0f, 0.0f), 30);
		pShader->ParticleStop();
		pShader->CreateShader(m_pd3dDevice);
		vContainer.push_back(pShader);
	}
	m_mParticleShader.insert(std::make_pair(name, vContainer));
}

void CParticleManager::AddQuadPartcieShader(std::string name, int nCount, float fSpeed, float fSize, float fAge, int nMaxPariticle, XMFLOAT3 fRandPosition, TCHAR* path)
{
	std::vector<std::shared_ptr<CCubeParticleShader>> vContainer;
	for (int i = 0; i < nCount; ++i)
	{
		auto pShader = std::make_shared<CQuadParticleShader>();
		pShader->Initialize(m_pd3dDevice, path, XMFLOAT3(0.0f, 0.0f, 0.0f), nMaxPariticle);
		pShader->ParticleStop();
		pShader->CreateShader(m_pd3dDevice);
		pShader->SetParitlceData(fSpeed, fSize, fAge, 1, fRandPosition);
		vContainer.push_back(pShader);
	}
	m_mParticleShader.insert(std::make_pair(name, vContainer));
}

void CParticleManager::AddWaterFallPartcielhader(std::string name, int nCount, float fSpeed, float fSize, float fAge, int nMaxPariticle, XMFLOAT3 fRandPosition, TCHAR* path)
{
	auto vPosition = Helper::MapCsvLoader("../Assets/01_MapData/CSV/", "WaterFall");

	for (auto &pos : vPosition)
	{
		for (int i = 0; i < 7; ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				auto pShader = std::make_shared<CFlowerParticleShader>();
				pShader->Initialize(m_pd3dDevice, path, pos, nMaxPariticle);
				pShader->PauseTime(i * 2);
				pShader->CreateShader(m_pd3dDevice);
				//pShader->AfterCreateShader(m_pd3dDevice);
				pShader->SetParitlceData(fSpeed, fSize, fAge, 10);
				m_vWaterFallShader.push_back(pShader);
			}
		}
	}
}

std::shared_ptr<CBillBoardShader> CParticleManager::FindSpriteAnimationShader(std::string name)
{
	auto iter = m_mBilBoardEffectShader.find(name);
	if (iter == m_mBilBoardEffectShader.end())
		return nullptr;
	else
		return iter->second;
}

void CParticleManager::AddAnimationSpriteShader(std::string name, std::string path, std::string blending, XMFLOAT2 fSize, float frameFPS, int frameCount)
{
	auto pShader = std::make_shared<CBillBoardSpriteShader>();
	pShader->CreateShader(m_pd3dDevice);
	pShader->Initialize(m_pd3dDevice, path, XMFLOAT3(0, 0, 30), fSize, frameFPS, frameCount, 0);
	pShader->SetBlendState(blending);
	m_mBilBoardEffectShader.insert(std::make_pair(name, pShader));
}

void CParticleManager::AddBillBoardEffectShader(std::string name, std::string path, XMFLOAT2 fSize)
{
	auto pShader = std::make_shared<CBillBoardDisappearShader>();
	pShader->CreateShader(m_pd3dDevice);
	pShader->SetMesh(move(std::make_unique<CQuadrangleMesh>(m_pd3dDevice, fSize.x, fSize.y)));
	pShader->SetMaterial(m_pd3dDevice, path);
	pShader->BuildObjects(m_pd3dDevice);
	pShader->SetBlendState("BillBoardAlpha");
	m_mBilBoardEffectShader.insert(std::make_pair(name, pShader));
}

void CParticleManager::StartParticle(const std::string& name, XMFLOAT3 xmPosition, int loop)
{
	auto vContainer = FindParticleContainer(name);
	auto size = vContainer.size();
	for (auto i = 0; i < size; ++i)
	{
		if (vContainer[i]->IsActivity())
		{
			vContainer[i]->SetPosition(xmPosition.x, xmPosition.y + 0.5f, xmPosition.z);
			vContainer[i]->ParticleRestart();
			loop--;
			if (loop == 0)break;
		}
	}
}

void CParticleManager::StartParticle(const std::string& name, const float& x, const float& y, const float & z, int loop)
{

	auto vContainer = FindParticleContainer(name);
	auto size = vContainer.size();
	if (size == 0)return;
	for (auto i = 0; i < size; ++i)
	{
		if (vContainer[i]->IsActivity())
		{
			vContainer[i]->SetPosition(XMFLOAT3(x, y, z));
			vContainer[i]->ParticleRestart();
			loop--;
			if (loop == 0)break;
		}
	}
}
 
void CParticleManager::StartSprtieAnimation(const std::string & name, const float & x, const float & y, const float & z, int loop)
{
	auto pShader = FindSpriteAnimationShader(name);
  	if (nullptr == pShader)return;
	
	pShader->SetPosition(x, y, z); 
	pShader->StartBillBoard(loop);
}

void CParticleManager::StartSprtieAnimation(const std::string & name, const XMFLOAT3& position, int loop)
{
	auto pShader = FindSpriteAnimationShader(name);
	if (nullptr == pShader)return;
	pShader->SetPosition(position);
	pShader->StartBillBoard(loop);
}

void CParticleManager::Render(ID3D11DeviceContext* pd3dDeviceContext, std::shared_ptr<CCamera> pCamera)
{
#ifdef _PARTILCE_ON

	CParticleManager::UpdateConstBuffer(pd3dDeviceContext);

	for (auto &pShader : m_mParticleShader)
	{
		auto vContainer = pShader.second;

		for (auto &pShader : vContainer)
		{
			pShader->Render(pd3dDeviceContext);
		}
	}
	for (auto &pShader : m_vWaterFallShader) 
		pShader->Render(pd3dDeviceContext);
#endif

	for (auto &pShader : m_mBilBoardEffectShader)
		pShader.second->Render(pd3dDeviceContext, pCamera);
}

void CParticleManager::Animate(float fTimeElasped)
{
#ifdef _PARTILCE_ON
	//---Update--

	bool input = false;
	if (INPUT->KeyDown(YK_7))
	{
		CONTENTMGR->ModifyBlendState();
	}

	if (INPUT->OnlyKeyDown(YT_KEY::YK_F6))
	{
		auto f3 = Helper::RuntimeDataModify(XMFLOAT4(), "xf3_ParticleData");
		m_fSpeed = f3.x; m_fSize = f3.y; m_fDuringTime = f3.z; m_fEmitCount = f3.w;
		input = true;
	}

	for (auto &pShader : m_mParticleShader)
	{
		auto vContainer = pShader.second;

		for (auto &pShader : vContainer)
		{
			pShader->Update(fTimeElasped);

			if (input)
				pShader->SetParitlceData(m_fSpeed, m_fSize, m_fDuringTime, m_fEmitCount);

		}
	}

	for (auto &pShader : m_vWaterFallShader)
	{
		pShader->Update(fTimeElasped);
		if (input)pShader->SetParitlceData(m_fSpeed, m_fSize, m_fDuringTime, m_fEmitCount);
	}
#endif

}

