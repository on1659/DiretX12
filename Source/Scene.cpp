
#include "stdafx.h"
#include "Scene.h"


//Struct

//SigleTon
#include "Timer.h"
#include "InputManager.h"
#include "FontManager.h"
#include "CubeTerrainManager.h"
#include "UIManager.h"
#include "ParticleManager.h"
#include "GameFramework.h"

#ifdef _QURD_ARRARY
#include "SpacePartitionQuad.h"
#else
#include "SpacePartition.h"
#endif

#include "AxisObjects.h"

//Mesh
#include "ModelParsingMesh.h"
#include "GeoCubeMesh.h"
#include "GeoSphereMesh.h"
#include "UIMesh.h"
#include "FBXSingleMesh.h"
#include "QuadrangleMesh.h"
#include "TSCubeMesh.h"
#include "GeoAABBMesh.h"
#include "WARP3DMesh.h"

#include "UISpriteMesh.h"

//Object
/*오보젝트는 쉐이더 h 에서 불러온다*/

//Script
#include "RotatingScript.h"
#include "RevolvingObject.h"
#include "RangeMove.h"
#include "Minyoung.h"
#include "MonsterPatrol.h"
#include "ItemSpeedUp.h"
#include "ItemHide.h"
#include "ItemThrow.h"
#include "ItemCooldownDecrease.h"
#include "BulletComponent.h"
#include "MonsterAnimationComponent.h"
#include "MonsterState.h"
#include "MonsterComponent.h"
#include "ItemTrap.h"
#include "ItemFlahsBang.h"

//Shader
#include "GeoInstanceingCubeShader.h"
#include "GeoInstanceSpereShader.h"
#include "InstancedObjectsShader.h"
#include "UIShader.h"
#include "TSCubeShader.h"
#include "BillBoardShader.h"
#include "FireParticleShader.h"
#include "BillBoardSpriteShader.h"
#include "WaterFallMeshShader.h"
//#include "ParticlePangShader.h"
#include "ParticleShader.h"
#include "AABBRenderShader.h"
#include "FBXSkinningShader.h"
#include "BlendShader.h"
#include "UIState.h"
#include "Warp3DSkinningShader.h"
#include "Warp3DSkinningShaderforBunny.h"
#include "BulletShader.h"
#include "ShadowMapShader.h"
#include "MonsterHeadCubeShader.h"
#include "HeadBangBangShader.h"
#include "TrapShader.h"

//Environment
#include "SkyBoxShader.h"
#include "CubeTerrainManager.h"

//Player
#include "PlayerManager.h" 
#include "OtherPlayer.h"

#include "UIGauge.h"

//
#include "GameSystem.h"
#include "BasicSystem.h"

#include "BillBoardDisappearShader.h"

#include "TSCubeMesh.h"

//Resource
#include "resource.h"


#include "CommClientDefine.h"

CScene::CScene() :
	m_ppObjectShaders(nullptr)
	, m_nObjectShaders(0)
	, m_ppInstancingShaders(nullptr)
	, m_nInstancingShaders(0)
	, m_pCamera(nullptr)
	, m_pSelectedObject(nullptr)
	, m_bMouseClick(false)
{


	m_ppObjects.clear();

	m_pGameSystem = nullptr;

	bGamePlaying = true;


	m_vDefaultMaterial_path.push_back(DEFAULT_TEXTURE_PATH);
	m_vDefaultMaterial_path.push_back(DEFAULT_NORMA_TEXTURE_PATH);

}

CScene::~CScene()
{
}

//Input Msg	
bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		//m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONUP:
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	//키보드입력을 받음
	switch (nMessageID)
	{

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_U:
			m_bMouseClick = (m_bMouseClick ? false : true);
			break;

		case 'P':
			break;

		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
			//KYT '12.02.07 plus
			/*
			PLAYER 싱글톤으로 변경
			*/
			PLAYER->OnProcessingKeyboardMessage(nMessageID, wParam);
			m_pCamera = PLAYER->GetCamera();
			break;

		case VK_M:
			AddThrowSkill(PLAYER->GetPlayer()->GetPositionXMFLOAT3(), PLAYER->GetPlayer()->GetLook(), 0);
			break;

		}

	}

	return (false);
}

bool CScene::ProcessInput(POINT ptOldCursorPos)
{
	return(false);
}

//Create
void CScene::BuildObjects(ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	//
	ID3D11SamplerState *pd3dSamplerState = nullptr;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	auto pSkyboxTexture = std::make_shared<CTexture>(1, 1, PS_SLOT_TEXTURE_SKYBOX, PS_SLOT_SAMPLER);
	pSkyboxTexture->SetSampler(0, pd3dSamplerState);
	pd3dSamplerState->Release();

	std::shared_ptr<CMaterial> pSkyboxMaterial = std::make_shared<CMaterial>();
	pSkyboxMaterial->SetTexture(pSkyboxTexture);
	OnChangeSkyBoxTextures(pd3dDevice, pSkyboxMaterial.get(), 0);

	std::unique_ptr<CSkyBoxMesh> pSkyBoxMesh = std::make_unique<CSkyBoxMesh>(pd3dDevice, 10, 10, 10);
	std::shared_ptr<CSkyBox> pSkyBox = std::make_shared<CSkyBox>(pd3dDevice);
	pSkyBox->SetMesh(move(pSkyBoxMesh), 0);
	pSkyBox->SetMaterial(pSkyboxMaterial);

	std::shared_ptr<CSkyBoxShader> pSkyBoxShader = std::make_shared<CSkyBoxShader>();
	pSkyBoxShader->CreateShader(pd3dDevice);
	pSkyBox->SetShader(pSkyBoxShader);

	m_ppObjects.push_back(pSkyBox);



	CScene::Load(pd3dDevice);
}

//Release Object
void CScene::ReleaseObjects()
{
	m_ppObjects.clear();

	for (int j = 0; j < m_nObjectShaders; j++)
	{
		if (m_ppObjectShaders[j]) m_ppObjectShaders[j]->ReleaseObjects();
		//if (m_ppObjectShaders[j]) m_ppObjectShaders[j]->Release();
	}
	if (m_ppObjectShaders) delete[] m_ppObjectShaders;

	for (int j = 0; j < m_nInstancingShaders; j++)
	{
		if (m_ppInstancingShaders[j]) m_ppInstancingShaders[j]->ReleaseObjects();
		//if (m_ppInstancingShaders[j]) m_ppInstancingShaders[j]->Release();
	}
	if (m_ppInstancingShaders) delete[] m_ppInstancingShaders;

	if (m_pGameSystem) delete m_pGameSystem;
	m_pGameSystem = nullptr;
	
	if (m_pd3dDevice)m_pd3dDevice->Release();
	m_pd3dDevice = nullptr;

	if (m_aabbShader) m_aabbShader->ReleaseObjects();
	m_aabbShader.reset();

	if (m_pBulletItemShader)
		m_pBulletItemShader->ReleaseObjects();
	else 
		m_pBulletItemShader.reset();


	if (m_pSkillThrowShader)
		m_pSkillThrowShader->ReleaseObjects();
	else
		m_pSkillThrowShader.reset();

	if (m_pFlashThrowShader)
		m_pFlashThrowShader->ReleaseObjects();
	else
		m_pFlashThrowShader.reset();

	if (m_pTrapShader)
		m_pTrapShader->ReleaseObjects();
	else
		m_pTrapShader.reset();

	for (int i = 0; i < 2; ++i)
	{
		if (m_pMonsterHeadCubeShader[i])
			m_pMonsterHeadCubeShader[i]->ReleaseObjects();
		else
			m_pMonsterHeadCubeShader[i].reset();
	}

	for (auto &pShader : m_vMonsterShader)
	{
		if (pShader)
			pShader->ReleaseObjects();
		else
			pShader.reset();
	}

	if (m_pHeadBangBangShader)
		m_pHeadBangBangShader->ReleaseObjects();
	else
		m_pHeadBangBangShader.reset();
	
	
	if (m_pGauge)m_pGauge.reset();

}

void CScene::Load(ID3D11Device* pd3dDevice)
{
	std::shared_ptr<CInstancedObjectsShader> pShader;

	//---------------------------------------------------------------------------------------------------------
		m_aabbShader = std::make_shared<CAABBRenderShader>();
		m_aabbShader->SetMesh(std::make_unique<CGeoCubeMesh>(pd3dDevice, 50, 5000, 50));
		m_aabbShader->CreateShader(pd3dDevice);
		m_aabbShader->BuildObjects(pd3dDevice);
	//---------------------------------------------------------------------------------------------------------


	//------------------------------------------------------ Bullet Shader ---------------------------------------------------------------------------------------------------------------------------------------------------------------

		//AppleItem
		m_pBulletItemShader.reset();
		m_pBulletItemShader = std::make_shared<CBulletShader>();
		m_pBulletItemShader->CreateShader(pd3dDevice);
		m_pBulletItemShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/05_Item/item_Apple.warp3d")));
		m_pBulletItemShader->BuildObjects(pd3dDevice);

		//Bullet
		m_pSkillThrowShader = std::make_shared<CBulletShader>();
		m_pSkillThrowShader->CreateShader(pd3dDevice);
		m_pSkillThrowShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/05_Item/item_WartermelonBar.warp3d")));
		m_pSkillThrowShader->BuildObjects(pd3dDevice);

		//Flash
		m_pFlashThrowShader = std::make_shared<CBulletShader>();
		m_pFlashThrowShader->CreateShader(pd3dDevice);
		m_pFlashThrowShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/05_Item/item_Doughnut.warp3d")));
		m_pFlashThrowShader->BuildObjects(pd3dDevice);

	//------------------------------------------------------ Bullet Shader ---------------------------------------------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------- TrapShader ----------------------------------------------------------------------------------------------------------------------------------------------------------------

		m_pTrapShader = std::make_shared<CTrapShader>();
		m_pTrapShader->CreateShader(pd3dDevice);
		m_pTrapShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/05_Item/item_Trap.warp3d")));
		m_pTrapShader->BuildObjects(pd3dDevice);

	//------------------------------------------------------- TrapShader ----------------------------------------------------------------------------------------------------------------------------------------------------------------



	//------------------------------------------------------ Monster HeadBangBang ---------------------------------------------------------------------------------------------------------------------------------------------------------------

		//A-Team
		m_pMonsterHeadCubeShader[MonsterTeam_A_Team] = std::make_shared<CMonsterHeadCubeShader>();
		m_pMonsterHeadCubeShader[MonsterTeam_A_Team]->CreateShader(pd3dDevice);
		m_pMonsterHeadCubeShader[MonsterTeam_A_Team]->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/02_ModelData/04_Others/Mark/voxel_redDiamond.FBX", 0.016f, SH_FBXTYPE::FBX_VOXEL)));
		m_pMonsterHeadCubeShader[MonsterTeam_A_Team]->BuildObjects(pd3dDevice);

		//B-Team
		m_pMonsterHeadCubeShader[MonsterTeam_B_Team] = std::make_shared<CMonsterHeadCubeShader>();
		m_pMonsterHeadCubeShader[MonsterTeam_B_Team]->CreateShader(pd3dDevice);
		m_pMonsterHeadCubeShader[MonsterTeam_B_Team]->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/02_ModelData/04_Others/Mark/voxel_blueDiamond.FBX", 0.016f, SH_FBXTYPE::FBX_VOXEL)));
		m_pMonsterHeadCubeShader[MonsterTeam_B_Team]->BuildObjects(pd3dDevice);

	//------------------------------------------------------ Monster HeadBangBang ---------------------------------------------------------------------------------------------------------------------------------------------------------------



	//------------------------------------------------------ Monster Shader ---------------------------------------------------------------------------------------------------------------------------------------------------------------

		//Monster A팀
		pShader.reset();
		pShader = CONTENTMGR->MakeDynamicShaderFactoty<CWarp3DSkinningShaderforBunny>(pd3dDevice, "MonsterShader_Team_A");
		pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/03_Monster/Zombunny.warp3d")));
		//pShader->SetMaterial(pd3dDevice, "../Assets/02_ModelData/03_Monster/tex_Zombunny_diffuse.png");
		pShader->SetMaterial(pd3dDevice, "../Assets/02_ModelData/03_Monster/tex_pinkBunny.png");
		m_vMonsterShader.push_back(pShader);

		//Monster B팀
		pShader.reset();
		pShader = CONTENTMGR->MakeDynamicShaderFactoty<CWarp3DSkinningShaderforBunny>(pd3dDevice, "MonsterShader_Team_B");
		pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/03_Monster/Zombunny.warp3d")));
		pShader->SetMaterial(pd3dDevice, "../Assets/02_ModelData/03_Monster/tex_blueskyBunny.png");
		m_vMonsterShader.push_back(pShader);

	//------------------------------------------------------ Monster Shader ---------------------------------------------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------ HeadBangBang ---------------------------------------------------------------------------------------------------------------------------------------------------------------

		m_pHeadBangBangShader = std::make_unique<CHeadBangBangShader>();
		m_pHeadBangBangShader->CreateShader(pd3dDevice);
		m_pHeadBangBangShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/Star.warp3d")));
		m_pHeadBangBangShader->BuildObjects(pd3dDevice);

	//------------------------------------------------------ HeadBangBang ---------------------------------------------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------ Gauge Object ---------------------------------------------------------------------------------------------------------------------------------------------------------------

		m_pGauge = std::make_unique<CUIGauge>();
		m_pGauge->Initialize(pd3dDevice, XMFLOAT2(FRAME_BUFFER_WIDTH * 0.5f, 200), XMFLOAT2(300, 100), 0.002f);
		m_fRespawnStartTime  = 0.0f;
		m_fRespawnDuringTime = 10.0f;
		m_bRespawnVisible = false;

	//------------------------------------------------------ Gauge Object ---------------------------------------------------------------------------------------------------------------------------------------------------------------

}

void CScene::LoadStaticFBX(ID3D11Device* pd3dDevice)
{
	auto vTwPath = WORLDMAP->GetMapPath();

	//------------------------------------------------------------------------------------------------------------
	std::shared_ptr<CInstancedObjectsShader> pShader;

	std::vector<std::string> shadername;
	std::deque<std::string> meshname;
	std::deque<std::string> csvname;

	shadername.push_back("Apple"); //500000~ 504000~
	meshname.push_back("../Assets/02_ModelData/05_Item/item_Apple.warp3d");
	csvname.push_back("item_Apple");
	
	shadername.push_back("BugerJelly");
	meshname.push_back("../Assets/02_ModelData/05_Item/item_BugerJelly.warp3d");
	csvname.push_back("item_BugerJelly");
	
	shadername.push_back("Doughnut");
	meshname.push_back("../Assets/02_ModelData/05_Item/item_Doughnut.warp3d");
	csvname.push_back("item_Doughnut");

	shadername.push_back("Cherry");
	meshname.push_back("../Assets/02_ModelData/05_Item/item_Cherry.warp3d");
	csvname.push_back("item_Cherry");


	//---------------------------------------------------------------------------------------------------------

	int itemType = 0;
	std::vector<std::shared_ptr<CGameObject>> vItemObject;
	std::vector<std::shared_ptr<CGameObject>> vTrapItemObject;

	for (auto &str : shadername)
	{
		//KYT '17.04.17
		/*
			Path Push
		*/
		vTwPath.push_back(str);

		pShader.reset();
		pShader = CONTENTMGR->MakeStaticShaderFactoty<CInstancedObjectsShader>(pd3dDevice, str);

		float scale = 0.02f;
		pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, meshname.front())));
		pShader->CreateShader(pd3dDevice);

		
		auto vObjectPosition = Helper::MapCsvLoader(XMFLOAT4(), "../Assets/01_MapData/CSV/", csvname.front());
		if (vObjectPosition.size() == 0) ::GMessageBox("CSV Data Empty", str);

		for (auto &objPos : vObjectPosition)
		{

			auto pObject = std::make_shared<CGameObject>();

			pObject->SetPosition(objPos.x, objPos.y, objPos.z);
			pObject->name = str;
			pObject->tag = TAG_TYPE::TAG_ITEM;
			pObject->nGID = objPos.w;

			switch (itemType)
			{
			case 0:
				pObject->SetCompoent(std::make_unique<ItemThrow>("ItemThrow"));
				break;
			case 1:
				pObject->SetCompoent(std::make_unique<ItemCooldownDecrease>("ItemCooldownDecrease"));
				break;
			case 2:
				pObject->SetCompoent(std::make_unique<ItemFlahsBang>("ItemFlahsBang"));
				break;
			case 3:
				auto trap = std::make_unique<ItemTrap>("ItemTrap");
				pObject->SetCompoent(move(trap));
				vTrapItemObject.push_back(pObject);
				break;
			}

			pObject->SetShader(pShader);
			SPACE->AddStaticObject(pObject);

			if (itemType != 3)  vItemObject.push_back(pObject);
		}

		m_vItemShader.push_back(pShader);
		meshname.pop_front();
		csvname.pop_front();
		++itemType;
	}


	//------------------- Blend Bush ----------------------------------
	pShader.reset();
	pShader = CONTENTMGR->MakeBlendShaderFactoty<CBlendShader>(pd3dDevice, "BlendBush");
	pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/Bush01.warp3d")));
	pShader->SetBlendState("Bush");

	//KYT '17.04.17
	/*
		Path Push
	*/
	vTwPath.push_back("BlendBush");

	//KYT '16.04.27
	/*
	여기서 CSV에 있는 x,y,z값을 뽑아옵니다.
	*/
	auto vObjectPosition = Helper::MapCsvLoader("../Assets/01_MapData/CSV/", "Bush");
	if (vObjectPosition.size() == 0) ::GMessageBox("CSV Data Empty", "BlendBush");

	for (auto &pObjectPosition : vObjectPosition)
	{
		pObjectPosition.x *= BLOCK_SCALE;
		pObjectPosition.y *= BLOCK_SCALE;
		pObjectPosition.z *= BLOCK_SCALE;

		std::shared_ptr<CGameObject> pObject = std::make_shared<CGameObject>("BlendBush");
		pObject->SetPosition(pObjectPosition);
		pObject->SetShader(pShader);

		SPACE->AddBlendObject(pObject);
	}
	//-------------------  Blend Bush ----------------------------------


	////RSH '16.08.19
	///*
	//- 폭포메쉬 렌더링 작업 중. 최신 쉐이더 개발 중
	//*/
	//{
	//	pShader.reset();
	//	//pShader = std::make_shared<CInstancedObjectsShader>();
	//	pShader = <CBillBoardSpriteShader>(pd3dDevice, "BlendBush"); //std::make_shared<CBillBoardSpriteShader>();
	//	pShader->CreateShader(pd3dDevice);
	//	pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/WaterFall.FBX", 1.0f, SH_FBXTYPE::FBX_VOXEL)));
	//	pShader->SetMaterial(pd3dDevice, "../Assets/03_Image/Sprite/waterfall_sprite_190_200.png");
	//
	//	auto pObject = std::make_shared<CGameObject>();
	//	pObject->SetPosition(0.0f, 0.0f, 0.0f);
	//	pObject->SetShader(pShader);
	//	//pShader->AddObject(pObject);
	//	//pShader->BuildObjects(pd3dDevice);
	//
	//	SPACE->AddBlendObject(pObject);
	//}
	//-------------------  Blend Item  ----------------------------------
	pShader.reset();
	pShader = CONTENTMGR->MakeBlendShaderFactoty<CBlendShader>(pd3dDevice, "BlendItem");
	pShader->SetMesh(move(std::make_unique<Default_Mesh>(pd3dDevice, 0.65f, 0.65f, 0.65f)));	//원래 0.6
	pShader->SetMaterial(pd3dDevice, "../Assets/03_Image/Miscellaneous/blend_test.png");
	pShader->SetBlendState("CubeItem");
	m_vItemShader.push_back(pShader);

	//KYT '17.04.17
	/*
		Path Push
	*/
	vTwPath.push_back("BlendItem");

	float nCubeheight = 1.3f;
	for (auto &pItemObject : vItemObject)
	{
		auto pos = pItemObject->GetPositionXMFLOAT3();
		pos.y = nCubeheight;
		std::shared_ptr<CGameObject> pObject = std::make_shared<CGameObject>("BlendCube");
		pObject->SetPosition(pos);
		pObject->SetShader(pShader);
		pItemObject->SetChild(pObject);

		auto rotSrcipt = std::make_unique<CRotatingScript>("로테이셩");
		rotSrcipt->SetRotationSpeed(30.0f);														//회전속도
		rotSrcipt->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		pObject->SetCompoent(move(rotSrcipt));

		SPACE->AddChlidObject(pObject);
	}

	for (auto &pItemObject : vTrapItemObject)
	{
		auto pos = pItemObject->GetPositionXMFLOAT3();
		pos.y = nCubeheight;
		std::shared_ptr<CGameObject> pObject = std::make_shared<CGameObject>("BlendCube");
		pObject->SetPosition(pos);
		pObject->SetShader(pShader);
		pItemObject->SetChild(pObject);

		pItemObject->SetChild(pObject);

		auto rotSrcipt = std::make_unique<CRotatingScript>("로테이셩");
		rotSrcipt->SetRotationSpeed(30.0f);														//회전속도
		rotSrcipt->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		pObject->SetCompoent(move(rotSrcipt));


		pItemObject->SetChild(pObject);

		SPACE->AddChlidObject(pObject);
	}

	//-------------------  Blend Item  ----------------------------------

	{
		auto pDynamicShader = CONTENTMGR->MakeStaticShaderFactoty<CTSCubeShader>(pd3dDevice, "Grass");
		pDynamicShader->SetMesh(move(std::make_unique<CTSCubeMesh>(pd3dDevice, 5, 5, 5)));
		static_cast<CTSCubeShader*>(pDynamicShader.get())->SetBumpScale();

		//KYT '17.04.17
		/*
			Path Push
		*/
		vTwPath.push_back("Grass");


		std::vector<std::string> path;
		//path.push_back("../Assets/03_Image/StoneImage/stones.bmp");
		//path.push_back("../Assets/03_Image/StoneImage/stones_nm.jpg");
		//path.push_back("../Assets/03_Image/StoneImage/stones_dm.jpg");

		path.push_back("../Assets/03_Image/TJ/diffuse.png");
		path.push_back("../Assets/03_Image/TJ/normal.png");
		path.push_back("../Assets/03_Image/TJ/displacement.png");

		pDynamicShader->SetMaterial(pd3dDevice, path);
		

		auto pObject = std::make_shared<CGameObject>();
		pObject->SetPosition(15, 3.5, 15);
		pObject->SetShader(pDynamicShader);
		SPACE->AddStaticObject(pObject);

		//for (int x = 0; x < 10; ++x)
		{
		//	for (int z = 0; z < 10; ++z)
			{
				auto pObject = std::make_shared<CGameObject>();
				pObject->SetPosition(6, 3.5, 6);
				pObject->SetShader(pDynamicShader);
				SPACE->AddStaticObject(pObject);

			}
		}


	}

	{
		auto pDynamicShader = CONTENTMGR->MakeStaticShaderFactoty<CTSCubeShader>(pd3dDevice, "Stone");
		pDynamicShader->SetMesh(move(std::make_unique<CTSCubeMesh>(pd3dDevice, 3, 3, 3)));
		static_cast<CTSCubeShader*>(pDynamicShader.get())->SetBumpScale(0.01f, 0.01f, 0.01f);
		//KYT '17.04.17
		/*
			Path Push
		*/
		vTwPath.push_back("Stone");

		std::vector<std::string> path;
		path.push_back("../Assets/03_Image/StoneImage/stones.bmp");
		path.push_back("../Assets/03_Image/StoneImage/stones_nm.jpg");
		path.push_back("../Assets/03_Image/StoneImage/stones_dm.jpg");

		pDynamicShader->SetMaterial(pd3dDevice, path);

		auto pObject = std::make_shared<CGameObject>();
		pObject->SetPosition(15, 3.5, 15);
		pObject->SetShader(pDynamicShader);


		auto rota = std::make_unique<CRotatingScript>("StoneRotating");
		rota->Start(pObject);
		rota->SetRotationAxis(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
		rota->SetRotationSpeed(1.0f);
		pObject->SetCompoent(std::move(rota));

		SPACE->AddStaticObject(pObject);
	}



	int pos[2] = { 860, 150 };
	int size[2] = { 320, 380 };
	int color[3] = { 237, 180, 237 };

	for (auto &d : vTwPath)
	{
		m_mTerrainMap[d] = true;
	}

	TwBar* twBar = TwNewBar("Map Voxcel Model");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	TwSetParam(twBar, NULL, "size", TW_PARAM_INT32, 2, size);
	TwSetParam(twBar, NULL, "color", TW_PARAM_INT32, 3, color);
	TwSetParam(twBar, NULL, "position", TW_PARAM_INT32, 2, pos);

	for (auto &d : vTwPath)
	{
		if (0 == TwAddVarRW(twBar, d.c_str(), TW_TYPE_BOOL8, &(m_mTerrainMap.find(d)->second), "key=F11"))
		{
			std::cout << "Error : " << d << std::endl;
		}
	}
}

void CScene::LoadDynamicFBX(ID3D11Device* pd3dDevice)
{
	return;
	std::shared_ptr<CInstancedObjectsShader> pShader;

	pShader.reset();

#ifdef _FBXANIMATION_LOADING_
	//pShader = CONTENTMGR->MakeDynamicShaderFactoty<CFBXSkinnedShaderforRunningZombunny>(pd3dDevice, "Monster");
	pShader = CONTENTMGR->MakeDynamicShaderFactoty<CWarp3DSkinningShaderforBunny>(pd3dDevice, "Monster");
#else
	pShader = CONTENTMGR->MakeDynamicShaderFactoty<CFBXSkinnedShaderforRunningZombunny>(pd3dDevice, "Monster");
#endif

	//pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/02_ModelData/03_Monster/Zombunny_running.fbx", BUNNY_SCALE, SH_FBXTYPE::FBX_MODEL)));
	pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/03_Monster/Zombunny.warp3d")));

	pShader->SetMaterial(pd3dDevice, "../Assets/02_ModelData/03_Monster/tex_blueskyBunny.png");
	//pShader->SetMaterial(pd3dDevice, "../Assets/02_ModelData/03_Monster/tex_ZomBunny_normals.png");

	std::vector<CPlayer*> vPlayer;
	vPlayer.push_back(PLAYER->GetPlayer().get());

	m_vMonsterShader.push_back(pShader);

	auto vOther = PLAYER->GetOtherPlayer();
	for (auto &p : vOther) vPlayer.push_back(p.get());

	for (int i = 0; i < MONSTER_MAXNUM; ++i)
	{
		auto pObject = std::make_shared<CGameObject>("Monster");

		auto scr = std::make_unique<MonsterState>();
		scr->SetPlayer(vPlayer);
		scr->SetMonsterShader(m_vMonsterShader);
		pObject->SetCompoent(move(scr));
		pObject->SetCompoent(std::make_unique<MonsterComponent>());
		pObject->tag = TAG_TYPE::TAG_BUNNY_MONSTER;
		float x = 0.0f;	float z = 0.0f;	float y = 1.0f;

		x = 83 + (i * 3);//rand() % (int)(22) + 137;
		z = 83 + (i * rand()%10);
		//rand() % (int)(20) + 12;
			   //x = rand() % (int)(22) + 80;
			   //z = rand() % (int)(20) + 12;

		pObject->SetPosition(x, y, z);
		pObject->SetShader(pShader);

		//RSH '16.07.31
		/*
		인스턴싱 애니메이션이 적용됨에 따라
		몬스터 상태 변경 스크립트 추가
		*/
		pObject->SetCompoent(std::make_unique<CMonsterAnimationComponent>());

		auto patrolCompoment = std::make_unique<CMonsterPatrol>();
		patrolCompoment->InitializePath(i);
		pObject->SetCompoent(move(patrolCompoment));

		SPACE->AddObject(pObject);
	}

}

void CScene::LoadSystem()
{
	if (m_pGameSystem) m_pGameSystem = nullptr;
	m_pGameSystem = new CBasicSystem();
	m_pGameSystem->Load(m_pd3dDevice);
	
	int nPlayerSize;

#	ifdef _CONNECT_SERVER_
		nPlayerSize = TCP->GetRoomPlayerInfoListSize();
	#else
		nPlayerSize = NON_SERVER_CLIENT_NUMBER;
	#endif

	if (nPlayerSize == 2)
	{
		m_pGameSystem->LoadPlayer(PLAYER->GetPlayer().get(), PLAYER->GetOtherPlayer(0).get());
	}
	else
	{
		m_pGameSystem->LoadPlayer
		(
			PLAYER->GetPlayer().get()
			, PLAYER->GetOtherPlayer(0).get()
			, PLAYER->GetOtherPlayer(1).get()
			, PLAYER->GetOtherPlayer(2).get()
		);
	}

	m_pGameSystem->LastLoad();
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	if (m_pGameSystem)
	{
		m_pGameSystem->Update(fTimeElapsed);

		//if (m_pGameSystem->IsGameEnd()) m_pGameSystem->ChangeState();
	}

	

	LIGHTMGR->Update(fTimeElapsed, m_pCamera);
	PARTICLE->Animate(fTimeElapsed);
	WORLDMAP->Animate(fTimeElapsed);

	//--------------------------------------------------------------
	
	#ifdef _SPACEPARTITIONING

		for (int i = 0; i < 2; ++i)
		{
			auto vShader = CONTENTMGR->GetAnimateObjectShaderContainer(i);
			for (auto pShader : vShader)
				pShader->AnimateObjects(fTimeElapsed);
		}

		UIMGR->Animate(fTimeElapsed);

		//초기화가 먼저야
		int nRoomCount = 0;
		auto vRoom = SPACE->GetRoomContainer();
		XMVECTOR camPositon = m_pCamera->GetPosition();
		int nObjectCount = 0;

		//방체크
		for (auto &pRoom : vRoom)
		{
			//프러스텀 안하는 방은 업데이트만
			for (auto &pObject : pRoom->GetDynamicObjectsContainer())
			{
				pObject->Animate(fTimeElapsed);
				SPACE->Update(pObject);
			}

			for (auto &pObject : pRoom->GetBlendObjectContainer())
			{
				pObject->Animate(fTimeElapsed);
				SPACE->UpdateBlend(pObject);
			}

			for (auto &pObject : pRoom->GetStaticObjectsContainer())
				pObject->Animate(fTimeElapsed);

			for (auto &pObject : pRoom->GetStaticTerrainContainer())
				pObject->Animate(fTimeElapsed);

			auto vChildObject = pRoom->GetChlidObjectContainer();

			for (auto &pObject : vChildObject)
				pObject->Animate(fTimeElapsed);

			vChildObject.remove_if
			(
				[](const auto& pObject
					)
			{
				if (false == pObject->IsActive())
				{
					pObject->ChildDestory(pObject);
					pObject->Release();
					return true;
				}
				else
					return false;
			}
			);

			nObjectCount += pRoom->GetChlidObjectContainer().size();
		}

	#endif


	if (m_pHeadBangBangShader)	m_pHeadBangBangShader->AnimateHeadBangBang(fTimeElapsed);
	if (m_pBulletItemShader)		m_pBulletItemShader->AnimateBullet(fTimeElapsed);
	if (m_pSkillThrowShader)		m_pSkillThrowShader->AnimateBullet(fTimeElapsed);

}

//OnCollision
void CScene::OnCollision(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pGameSystem)
	{
		m_pGameSystem->OnCollision();
	}


}
//Render
void CScene::OnPreRender(ID3D11DeviceContext *pd3dDeviceContext)
{

}

void CScene::SetViewport(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pCamera->SetViewport(pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
}

void CScene::RenderAway(ID3D11DeviceContext *pd3dDeviceContext)
{
	//--------------------------------------- 카메라를 업데이트한다. 1번 ---------------------------------------//
	if (m_pCamera)	m_pCamera->UpdateConstBuffers(pd3dDeviceContext);
	{
		m_pCamera->UpdateConstBuffers(pd3dDeviceContext);
		m_pCamera->UpdateShadowConstBufferUpdate(pd3dDeviceContext);
	}
	//---------------------------------------카메라를 업데이트한다. ---------------------------------------//
		
}

//SkyBox, Terrain ...
void CScene::RenderEnvironment(ID3D11DeviceContext	*pd3dDeviceContext)
{
	CScene::RenderAway(pd3dDeviceContext);


	//--------------------------------------- 스카이박스를 그린다 CCamera*---------------------------------------//
	m_ppObjects[0]->Render(pd3dDeviceContext, m_pCamera);
	//--------------------------------------- 스카이박스를 그린다 ---------------------------------------//

	WORLDMAP->Render(pd3dDeviceContext);


	if (m_pMonsterHeadCubeShader[0])m_pMonsterHeadCubeShader[0]->Render(pd3dDeviceContext, m_pCamera);
	if (m_pMonsterHeadCubeShader[1])m_pMonsterHeadCubeShader[1]->Render(pd3dDeviceContext, m_pCamera);

	if (m_pHeadBangBangShader)	m_pHeadBangBangShader->RenderMonster(pd3dDeviceContext, m_pCamera);
	if (m_pBulletItemShader)    m_pBulletItemShader->Render(pd3dDeviceContext);
	if (m_pSkillThrowShader)    m_pSkillThrowShader->Render(pd3dDeviceContext);
	//--------------------------------------- 지형을 그린다 ---------------------------------------//
	//if (m_ppObjects[1]->IsVisible(m_pCamera))m_ppObjects[1]->Render(pd3dDeviceContext, m_pCamera);
	//--------------------------------------- 지형을 그린다 ---------------------------------------//
}

void CScene::RenderBasic(ID3D11DeviceContext *pd3dDeviceContext)
{
	CScene::RenderAway(pd3dDeviceContext);


	////--------------------------------------- 플레이어를 그린다 ---------------------------------------//
	PLAYER->Render(pd3dDeviceContext);
	////--------------------------------------- 플레이어를 그린다 ---------------------------------------//


	if (m_pGameSystem) m_pGameSystem->Render(pd3dDeviceContext);
}

//Static Shader
void CScene::RenderSpacePartition(ID3D11DeviceContext *pd3dDeviceContext)
{
	CScene::RenderAway(pd3dDeviceContext);


	//초기화가 먼저야
	m_vRenderShader.clear();

	int nViewCount = 0;
	auto vRoom = SPACE->GetRoomContainer();
	XMVECTOR camPositon = m_pCamera->GetPosition();

	//방체크
	//int nCameRoom = SPACE->FindSpaceNum(camPositon);

	int monstercount = 0;
	for (auto &pRoom : vRoom)
	{
		//if (vViewRoomContainer.end() == std::find(vViewRoomContainer.begin(), vViewRoomContainer.end(), pRoom->GetRoomContainerCount()))
		//	continue;

		//각 방별로 카메라와 프러스텀하는지

		if (m_pCamera->IsInFrustumQuad(pRoom->GetAABB()))
			//if (m_pCamera->IsInFrustumQuad(pRoom->GetAABB()))
		{
			nViewCount++;

			#pragma region [DynamicShader]
						//DynamicShader Container
						auto vObjectContainer = pRoom->GetDynamicObjectsContainer();
						for (auto &pObject : vObjectContainer)
						{
							if (pObject->IsVisible(m_pCamera))
							{
								auto pShader = (CInstancedObjectsShader*)pObject->GetShader().get();

								if (pShader == nullptr)
									continue;

								pShader->AddObject(pObject);

								bool check = true;
								for (auto &pRenderShader : m_vRenderShader)
								{
									if (pShader->name == pRenderShader->name)
									{
										check = false;
										break;
									}
								}

								if (check)	m_vRenderShader.push_back(pShader);
								pShader = nullptr;
							}
						}
				#pragma endregion

			#pragma region[StaticShader]
						auto vObjectStaticContainer = pRoom->GetStaticObjectsContainer();
						for (auto &pObject : vObjectStaticContainer)
						{
							#ifdef DEBUG_MODE_UI
								if (pObject->name == "NewTrapObject")
								{
									FONTMGR->MessagePush(250, 110, 30, YT_Color(25, 25, 25), L"Object Id : %d", pObject->nGID);
									FONTMGR->MessagePush(250, 140, 30, YT_Color(25, 25, 25), L"Shader Name : %s", pObject->GetShader()->name);
								}
							#endif

							if (pObject->IsVisible(m_pCamera))
							{
								//Shader Push
								auto pShader = (CInstancedObjectsShader*)pObject->GetShader().get();
								if (pShader == nullptr)continue;
								pShader->AddObject(pObject);

								bool check = true;
								for (auto &pRenderShader : m_vRenderShader)
								{
									if (pShader->name == pRenderShader->name)
									{
										check = false;
										break;
									}
								}
								if (check) m_vRenderShader.push_back(pShader);
								pShader = nullptr;
							}
						}
			#pragma endregion

			#pragma region[StaticTerrain Shader]
						auto vStaticTerrainContainer = pRoom->GetStaticTerrainContainer();
						for (auto &pObject : vStaticTerrainContainer)
						{
							if (pObject->IsVisible(m_pCamera, camPositon))
							{
								//Shader Push
								auto pShader = (CInstancedObjectsShader*)pObject->GetShader().get();
								pShader->AddObject(pObject);

								bool check = true;
								for (auto &pRenderShader : m_vRenderShader)
								{
									if (pShader->name == pRenderShader->name)
									{
										check = false;
										break;
									}
								}
								if (check) m_vRenderShader.push_back(pShader);
								pShader = nullptr;
							}
						}
			#pragma endregion

			#pragma region[ChildContainer Shader]
						auto vChildContainer = pRoom->GetChlidObjectContainer();
						for (auto &pObject : vChildContainer)
						{
							if (pObject->IsVisible(m_pCamera, camPositon))
							{
								//Shader Push
								auto pShader = (CInstancedObjectsShader*)pObject->GetShader().get();
								pShader->AddObject(pObject);

								bool check = true;
								for (auto &pRenderShader : m_vRenderShader)
								{
									if (pShader->name == pRenderShader->name)
									{
										check = false;
										break;
									}
								}
								if (check) m_vRenderShader.push_back(pShader);
								pShader = nullptr;
							}
						}
			#pragma endregion
						
			#pragma region [BlendShader]
				auto vBlendObjectContainer = pRoom->GetBlendObjectContainer();
				for (auto &pObject : vBlendObjectContainer)
						{
							if (pObject->IsVisible(m_pCamera))
							{
								auto pShader = (CInstancedObjectsShader*)pObject->GetShader().get();

								if (pShader == nullptr)
									continue;

								pShader->AddObject(pObject);

								bool check = true;
								for (auto &pRenderShader : m_vRenderShader)
								{
									if (pShader->name == pRenderShader->name)
									{
										check = false;
										break;
									}
								}

								if (check)	m_vRenderShader.push_back(pShader);
								pShader = nullptr;
							}
						}
			#pragma endregion
		}
	}
	#ifdef DEBUG_MODE_UI
		FONTMGR->MessagePush("기본", 250, 50, 15, YT_Color(220, 120, 0), L"Room : %1.f", nViewCount);
	#endif
	for (auto &pShader : m_vRenderShader)
	{
		if (m_bMouseClick)
		{
			m_aabbShader->Render
			(
				pd3dDeviceContext
				, m_pCamera
				, pShader->GetvRenderObject()[0]->GetBoundingBox()
				, pShader->GetvRenderObject()
			);
		}

		auto iter = m_mTerrainMap.find(pShader->name);
		if (m_mTerrainMap.end() != iter)
		{
			if (!iter->second)
			{
				pShader->ClearRenderObject();
				continue;
			}
		}
		pShader->Render(pd3dDeviceContext, m_pCamera);
	}

	//Bounding BOX Rendering ~

#ifndef BOUNDING_ORIENTED_BOX
	if (m_bMouseClick)
	{
		auto pPlayer = PLAYER->GetPlayer();
		PlayerState* state = pPlayer->GetPlayerState();
		m_aabbShader->Render
		(
			pd3dDeviceContext
			, m_pCamera
			, std::vector<BoundingBox>{state->GetSmashBoundingBox(), state->GetBackBoudingBox()}
		, pPlayer
			);

		auto vOtherPlayer = PLAYER->GetOtherPlayer();
		for (auto &pOtherPlayer : vOtherPlayer)
		{
			PlayerState* otherState = (PlayerState*)pOtherPlayer->GetComponent(0);
			m_aabbShader->Render
			(
				pd3dDeviceContext
				, m_pCamera
				, std::vector<BoundingBox>{otherState->GetSmashBoundingBox(), otherState->GetBackBoudingBox()}
			, pOtherPlayer
				);
		}


		if (m_pBulletItemShader)
		{
			if (m_pBulletItemShader->GetvRenderObject().size() > 0)
			{
				m_aabbShader->Render
				(
					pd3dDeviceContext
					, m_pCamera
					, m_pBulletItemShader->GetvRenderObject()[0]->GetBoundingBox()
					, m_pBulletItemShader->GetvRenderObject()
				);
			}
		}
		if (m_pSkillThrowShader)
		{
			if (m_pSkillThrowShader->GetvRenderObject().size() > 0)
			{
				m_aabbShader->Render
				(
					pd3dDeviceContext
					, m_pCamera
					, m_pSkillThrowShader->GetvRenderObject()[0]->GetBoundingBox()
					, m_pSkillThrowShader->GetvRenderObject()
				);
			}
		}
	}
#endif
}

//Blend Shader
void CScene::RenderBlend(ID3D11DeviceContext *pd3dDeviceContext)
{
	CScene::RenderAway(pd3dDeviceContext);

	PARTICLE->Render(pd3dDeviceContext, m_pCamera);
	UIMGR->Render(pd3dDeviceContext);

	//Gauge
	if (gBulrMode.w == YT_BlurMode::BLUR_RESPAWN)
	{
		m_bRespawnVisible = true;
		m_pGauge->Start();
		m_fRespawnStartTime = TIMEMGR->GetGlobalTime();
	}

	if (m_bRespawnVisible)
	{
		if (TIMEMGR->GetGlobalTime() - m_fRespawnStartTime > m_fRespawnDuringTime)
		{
			m_bRespawnVisible = false;
			m_fRespawnStartTime = 0.0f;
		}

		m_pGauge->Render(pd3dDeviceContext);
	}
}

//Non MTR
void CScene::Render(ID3D11DeviceContext	*pd3dDeviceContext)
{
	RenderAway(pd3dDeviceContext);

}

//Picking
CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient)
{

	if (!m_pCamera) return(nullptr);

	XMMATRIX d3dxmtxView = m_pCamera->GetViewMatrix();
	XMMATRIX d3dxmtxProjection = m_pCamera->GetProjectionMatrix();
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	XMVECTOR d3dxvPickPosition;

	XMFLOAT4 f4vPickPosition;
	XMFLOAT4X4 f4x4mtxProjection;

	XMStoreFloat4x4(&f4x4mtxProjection, d3dxmtxProjection);
	f4vPickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / f4x4mtxProjection._11;
	f4vPickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / f4x4mtxProjection._22;
	f4vPickPosition.z = 1.0f;

	d3dxvPickPosition = XMLoadFloat4(&f4vPickPosition);

	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	MESHINTERSECTINFO d3dxIntersectInfo;
	CGameObject *pIntersectedObject = nullptr, *pNearestObject = nullptr;

	for (auto &p : m_ppObjects)
	{
		nIntersected = p->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = p.get();
		}
	}

	return(pNearestObject);
}

//SykBox
void CScene::OnChangeSkyBoxTextures(ID3D11Device* pd3dDevice, CMaterial *pMaterial, int nIndex)
{
	_TCHAR pstrTextureNames[128];
	_stprintf_s(pstrTextureNames, _T("../Assets/03_Image/SkyBox/SkyBox_add.dds"), nIndex, 128);
	ID3D11ShaderResourceView *pd3dsrvTexture = nullptr;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureNames, nullptr, nullptr, &pd3dsrvTexture, nullptr);
	pMaterial->m_pTexture->SetTexture(0, pd3dsrvTexture); pd3dsrvTexture->Release();
}


//총알
void CScene::AddBullet(XMFLOAT3 xmPosition, const XMVECTOR& x3Look, const int& playerId)
{
	std::shared_ptr<CGameObject> pGameObject = std::make_shared<CGameObject>();

	XMVECTOR right = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), x3Look);

	auto xm = xmPosition;
	xm.y = BLOCK_SCALE * 1.6f;
	pGameObject->SetPosition(xm);
	pGameObject->name = "StunIten";
	pGameObject->tag = TAG_BULLET;

	XMVECTOR look;
	look = x3Look + (right * 0.2f);
	std::unique_ptr<BulletComponent> bullet = std::make_unique<BulletComponent>();
	bullet->Start(pGameObject);
	bullet->SetId(playerId);
	bullet->SetLook(look);
	pGameObject->SetCompoent(move(bullet));

	auto rot = std::make_unique<CRotatingScript>();
	rot->Start(pGameObject);
	rot->SetRotationAxis(XMVectorSet(rand() % 10 * 0.1, rand() % 10 * 0.1, rand() % 10 * 0.1, 1.0f));
	rot->SetRotationSpeed(rand()%360 + 120);
	pGameObject->SetCompoent(move(rot));

	m_pBulletItemShader->AddBullet(pGameObject);
	pGameObject->SetShader(m_pBulletItemShader);
	SPACE->AddObject(pGameObject);
}

void CScene::AddThrowSkill(XMFLOAT3 xmPosition, XMVECTOR& x3Look, const int& playerId)
{
	std::cout << "Throw 제용이가 쏨 " << std::endl;
	std::shared_ptr<CGameObject> pGameObject = std::make_shared<CGameObject>();

	XMVECTOR right = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), x3Look);

	auto xm = xmPosition;
	xm.y = BLOCK_SCALE * 1.6f;
	pGameObject->SetPosition(xm);
	pGameObject->name = "FrpzemItem";
	pGameObject->tag = TAG_BULLET;
	
	//pGameObject->Rotate(x3Look, 90.0f);// , 0);

	XMVECTOR look;
	look = x3Look + (right * 0.2f);
	std::unique_ptr<BulletComponent> bullet = std::make_unique<BulletComponent>();
	bullet->Start(pGameObject);
	bullet->SetId(playerId);
	bullet->SetLook(look);
	pGameObject->SetCompoent(move(bullet));


	auto rot = std::make_unique<CRotatingScript>();
	rot->Start(pGameObject);
	rot->SetRotationAxis(XMVectorSet(0,0,1, 1.0f));
	rot->SetRotationSpeed(350.0f);
	pGameObject->SetCompoent(move(rot));

	m_pSkillThrowShader->AddBullet(pGameObject);
	pGameObject->SetShader(m_pSkillThrowShader);
	SPACE->AddObject(pGameObject);
}

void CScene::AddTrap(const XMFLOAT3& xmPosition, CPlayer* player, const int& playerId)
{
	for (auto &pObject : m_lFakeItemList)
	{
		auto f3 = pObject->GetPositionXMFLOAT3();
		if (f3.x == xmPosition.x)
		{
			if (f3.z == xmPosition.z)
			{
				SPACE->StaticDestory(pObject);
				pObject->SetActive(false);
			}
		}
	}
	m_lFakeItemList.remove_if
	(
	[](const auto& pObject)
	{
		if (false == pObject->IsActive())
		{
			pObject->Release();
			return true;
		}
		else
			return false;
	}
	);

 	auto pItemObject = std::make_shared<CGameObject>();
	pItemObject->name = "ItemBoom";
	pItemObject->tag = TAG_TYPE::TAG_END;
	pItemObject->SetPosition(xmPosition);					//포지셔닝
	pItemObject->SetShader(m_pTrapShader);					//Shader세팅
	SPACE->AddStaticObject(pItemObject);

	auto script = std::make_unique<ItemBoom>();
	//움직이지 못할 플레이어를 정해준다.
	script->SetLockPosition(player->GetPositionXMFLOAT3());
	script->Start(pItemObject);
	
#ifndef _CONNECT_SERVER_
	script->SetPlayer(player);
#endif
	pItemObject->SetCompoent(std::move(script));

}

void CScene::AddFakeInstall(const XMFLOAT3& xmPosition)
{
	auto pItemObject = std::make_shared<CGameObject>();
	pItemObject->name = "FakeItem";
	pItemObject->tag = TAG_TYPE::TAG_FAKE_ITEM;
	pItemObject->SetPosition(xmPosition);						//포지셔닝
	pItemObject->SetShader(m_vItemShader[rand() % 4]);			//Shader세팅

	auto fakeScript = std::make_unique<ItemFake>();				//페이크 스크립트 생성
	fakeScript->Start(pItemObject);
	pItemObject->SetCompoent(std::move(fakeScript));			//컴포넌트등록
	SPACE->AddStaticObject(pItemObject);						//등록

	auto pos = pItemObject->GetPositionXMFLOAT3(); pos.y += 1.0f;
	auto pCubeObject = std::make_shared<CGameObject>("BlendCube"); //큐브아이템생성
	pCubeObject->SetPosition(xmPosition);									//포지셔닝
	pCubeObject->SetShader(m_vItemShader[4]);						  //쉐이더붙이고
	pItemObject->SetChild(pCubeObject);							 //Child로 넣어주고
	SPACE->AddChlidObject(pCubeObject);							 //Child Object로 넣어준다

	auto rotSrcipt = std::make_unique<CRotatingScript>("로테이셩");
	rotSrcipt->SetRotationSpeed(-30.0f);														//회전속도
	rotSrcipt->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	rotSrcipt->Start(pCubeObject);
	pCubeObject->SetCompoent(move(rotSrcipt));

	m_lFakeItemList.push_back(pItemObject);
}

void CScene::AddFlashItem(XMFLOAT3 xmPosition, const XMVECTOR & x3Look, const int & playerId)
{
	std::shared_ptr<CGameObject> pGameObject = std::make_shared<CGameObject>();

	XMVECTOR right = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), x3Look);

	auto xm = xmPosition;
	xm.y = BLOCK_SCALE * 1.6f;
	pGameObject->SetPosition(xm);
	pGameObject->name = "FLASH";
	pGameObject->tag = TAG_BULLET;

	pGameObject->Rotate(0, 90, 0);

	XMVECTOR look;
	look = x3Look + (right * 0.2f);
	std::unique_ptr<BulletComponent> bullet = std::make_unique<BulletComponent>();
	bullet->Start(pGameObject);
	bullet->SetId(playerId);
	bullet->SetLook(look);
	pGameObject->SetCompoent(move(bullet));


	auto rot = std::make_unique<CRotatingScript>();
	rot->Start(pGameObject);
	rot->SetRotationAxis(XMVectorSet(0, 1, 0, 1.0f));
	rot->SetRotationSpeed(350);
	pGameObject->SetCompoent(move(rot));

	m_pFlashThrowShader->AddBullet(pGameObject);
	pGameObject->SetShader(m_pFlashThrowShader);
	SPACE->AddObject(pGameObject);
}

//총알

void CScene::AddMonsterHeadCube(const XMFLOAT3 & xmPosition, const int & playerId)
{
	if (playerId == 0 || playerId == 1)
		m_pMonsterHeadCubeShader[MonsterTeam_A_Team]->AddPosition(xmPosition);

	if (playerId == 2 || playerId == 2)
		m_pMonsterHeadCubeShader[MonsterTeam_B_Team]->AddPosition(xmPosition);
}

void CScene::AddHeadBangBang(const XMFLOAT3 & xmPosition, MonsterState* state)
{
	m_pHeadBangBangShader->AddHeadBangBang(xmPosition, state);
}

