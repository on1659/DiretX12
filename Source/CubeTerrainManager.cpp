#include "stdafx.h"

//Terain Mesh
#include "CubeTerrainManager.h"
#include "TerrainCubeMesh.h"
#include "TerrainCubeShader.h"

//Component
#include "RotatingScript.h"
#include "UpDownPatrol.h"
#include "CloudPatrol.h"

//GeoMetroy
#include "GeoTerrainCubeShader.h"
#include "GeoCubeMesh.h"
#include "UIMesh.h"

#include "InstancedObjectsShader.h"

#ifdef _QURD_ARRARY
#include "SpacePartitionQuad.h"
#else
#include "SpacePartition.h"
#endif

//FBX
#include "FBXSkinningShader.h"
#include "FBXSingleMesh.h"

//Warp3D
#include "WARP3DMesh.h"

//Water Wave
#include "TSCubeShader.h"
#include "TSCubeMesh.h"

#include "Timer.h"

#include "BushShader.h"
#include "BlendShader.h"

#include "QuadrangleMesh.h"
#include "FireParticleShader.h"

#include <iostream>
#include <fstream>

#include "ChochShader.h"
#include "WaterShader.h"
#include "TerrainWater.h"
#include "WaterFallMeshShader.h"
#include "BillBoardSpriteShader.h"
#include "InstancedSpriteShader.h"

CCubeTerrainManager::CCubeTerrainManager()
{
	//   m_vMtxWorld.clear();
}


CCubeTerrainManager::~CCubeTerrainManager()
{
}

void CCubeTerrainManager::ReleseInstance()
{
	m_vPlaneShader.clear();
}

void CCubeTerrainManager::Load(ID3D11Device *pd3dDevice)
{
	//KYT '16.04.04
	/*
		여기서 부터 CSV 폴더에 있는 파일을 읽어서 맵을 로드한다.
	*/
	std::shared_ptr<CInstancedObjectsShader> pShader;

	#pragma region [Map Model]
		std::vector <std::string>  vPath;
		vPath.resize(8);
		vPath[0] = ("01_Cupcake02_1x1");
		vPath[1] = ("02_Rubyringcandy_1x1");
		vPath[2] = ("03_Candymachine_1x1");
		vPath[3] = ("04_Chocostrawberry_3x3");
		vPath[4] = ("05_Cupcake01_3x3");
		vPath[5] = ("06_Rollcake02_3x3");
		vPath[6] = ("08_CookieHouse_1x1");
		vPath[7] = ("09_ColaBottle_1x1");

		for (auto& path : vPath)
		{
			//KYT '17.04.18
			/*
				MapOnoff를 위함
			*/
			//
			m_vMapPath.push_back(path);
			//


			std::string meshPath;
			meshPath = "../Assets/01_MapData/Map/";
			meshPath.append(path);
			//meshPath.append(".FBX");
			meshPath.append(".warp3d");

			pShader.reset();
			pShader = CONTENTMGR->MakeStaticTerrainShaderFactoty<CInstancedObjectsShader>(pd3dDevice, path);
			//pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, meshPath, DESSERT_SCALE, SH_FBXTYPE::FBX_VOXEL)));
	
			pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, meshPath)));
			//KYT '16.04.27
			/*
			여기서 CSV에 있는 x,y,z값을 뽑아옵니다.
			*/
			auto vObjectPosition = Helper::MapCsvLoader("../Assets/01_MapData/CSV/", path);
			if (vObjectPosition.size() == 0) ::GMessageBox("CSV Data Empty", path);

			for (auto &pObjectPosition : vObjectPosition)
			{
				pObjectPosition.x *= BLOCK_SCALE;
				pObjectPosition.y *= BLOCK_SCALE;
				pObjectPosition.z *= BLOCK_SCALE;

				path.append("TerrainCube");
				std::shared_ptr<CGameObject> pTerrainObject = std::make_shared<CGameObject>();
				pTerrainObject->SetPosition(pObjectPosition);
				pTerrainObject->SetShader(pShader);
				SPACE->AddStatictTerrainObject(pTerrainObject);
			}
		}
	#pragma endregion

	#pragma region[불꽃 호박 배치]
		{
			std::vector<XMFLOAT3> pumpkinPos;
			std::vector<std::shared_ptr<CGameObject>> vPumpkinObject;


			pShader.reset();
			vPumpkinObject.clear();

			pShader = CONTENTMGR->MakeStaticTerrainShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "07_Pumpkin_3x3.FBX");
			
			//KYT '17.04.18
			/*
				MapOnoff를 위함
			*/
			//
				m_vMapPath.push_back("07_Pumpkin_3x3.FBX");
			//

			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/07_Pumpkin_3x3.FBX", DESSERT_SCALE, SH_FBXTYPE::FBX_VOXEL)));
			//pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/Cloud01.FBX", CLOUD_SCALE, SH_FBXTYPE::FBX_VOXEL)));
			pShader->SetMaterial(pd3dDevice, "../Assets/Voxel/untitled.png");
			m_vPumpkin.push_back(pShader);


			//-----------------------------------------------1번------------------------------------------
			for (int i = 0; i < 23; ++i)
			{
				std::shared_ptr<CGameObject> pTerrainObject = std::make_shared<CGameObject>();
				float x = 0.0f;   float z = 0.0f;   float y = 1.0f;

				x = 10.0f * i + 20.0f;
				y = (rand() % 2) + 5;
				z = 5.0f;

				//pTerrainObject->SetPosition(XMFLOAT3(10.0f*i + 20.0f, y, 8.0f));
				pTerrainObject->SetPosition(XMFLOAT3(x, y, z));

				pumpkinPos.push_back(XMFLOAT3(x, y, z));

				pTerrainObject->Rotate(0.0f, 180.0f, 0.0f);
				pTerrainObject->SetShader(pShader);

				pTerrainObject->SetCompoent(std::make_unique<UpDownPatrol>());


				SPACE->AddStatictTerrainObject(pTerrainObject);
			}
			//-----------------------------------------------1번------------------------------------------


			//-----------------------------------------------2번------------------------------------------
			for (int i = 0; i < 23; ++i)
			{
				std::shared_ptr<CGameObject> pTerrainObject = std::make_shared<CGameObject>();
				float x = 0.0f;   float z = 0.0f;   float y = 1.0f;

				x = 10.0f * i + 20.0f;
				y = (rand() % 2) + 5;

				if (i >= 8 && i <= 14)
					z = 130.0f;
				else
					z = 155.0f;

				//pTerrainObject->SetPosition(XMFLOAT3(10.0f*i + 20.0f, y, 8.0f));
				pTerrainObject->SetPosition(XMFLOAT3(x, y, z));
				pumpkinPos.push_back(XMFLOAT3(x, y, z));
				pTerrainObject->SetShader(pShader);

				pTerrainObject->SetCompoent(std::make_unique<UpDownPatrol>());

				SPACE->AddStatictTerrainObject(pTerrainObject);
			}
			//-----------------------------------------------2번------------------------------------------


			//-----------------------------------------------3번------------------------------------------
			for (int j = 0; j < 14; ++j)
			{
				std::shared_ptr<CGameObject> pTerrainObject = std::make_shared<CGameObject>();
				float x = 0.0f;   float z = 0.0f;   float y = 1.0f;

				x = 5.0f;
				y = (rand() % 3) + 5;
				z = 10.0f * j + 20.0f;

				//pTerrainObject->SetPosition(XMFLOAT3(8.0f, y, 10.0f * j + 20.0f));
				pTerrainObject->SetPosition(XMFLOAT3(x, y, z));
				pumpkinPos.push_back(XMFLOAT3(x, y, z));
				pTerrainObject->Rotate(0.0f, -90.0f, 0.0f);
				pTerrainObject->SetShader(pShader);

				pTerrainObject->SetCompoent(std::make_unique<UpDownPatrol>());

				SPACE->AddStatictTerrainObject(pTerrainObject);
			}
			//-----------------------------------------------3번------------------------------------------


			//----------------------------------------------- 4번------------------------------------------
			for (int j = 0; j < 14; ++j)
			{
				std::shared_ptr<CGameObject> pTerrainObject = std::make_shared<CGameObject>();
				float x = 0.0f;   float z = 0.0f;   float y = 1.0f;

				x = 245.0f;
				y = (rand() % 3) + 5;
				z = 10.0f * j + 20.0f;

				//pTerrainObject->SetPosition(XMFLOAT3(8.0f, y, 10.0f * j + 20.0f));
				pTerrainObject->SetPosition(XMFLOAT3(x, y, z));
				pumpkinPos.push_back(XMFLOAT3(x, y, z));
				pTerrainObject->Rotate(0.0f, 90.0f, 0.0f);
				pTerrainObject->SetShader(pShader);

				pTerrainObject->SetCompoent(std::make_unique<UpDownPatrol>());

				SPACE->AddStatictTerrainObject(pTerrainObject);
			}
			//----------------------------------------------- 4번------------------------------------------


			//Particle
			//---------------------------------------------------------------------------------------------------------
			pShader.reset();
			pShader = CONTENTMGR->MakeStaticShaderFactoty<CFireParticleShader>(pd3dDevice, "FireShader");
			pShader->SetMesh(move(std::make_unique<CQuadrangleMesh>(pd3dDevice, 5, 5)));

			//KYT '17.04.18
			/*
				MapOnoff를 위함
			*/
			//
				m_vMapPath.push_back("FireShader");
			//

			std::vector<std::string> path;
			path.push_back("../Assets/03_Image/Particle/Fire/FireTexture.png");
			path.push_back("../Assets/03_Image/Particle/Fire/FireNoise.png");
			path.push_back("../Assets/03_Image/Particle/Fire/FireAlpha.png");

			pShader->SetMaterial(pd3dDevice, path, 2, 2, SAMPLESTATE_PARTICLE_BASE);
			m_vPumpkin.push_back(pShader);

			int i = 0;
			//vPumpkinObject.push_back(pTerrainObject);

			for (auto &pos : pumpkinPos)
			{
				auto pObject = std::make_shared<CGameObject>();
				pObject->SetPosition(pos.x, pos.y + 3, pos.z);
				pObject->SetShader(pShader);

				//auto script = std::make_unique<UpDownPatrol>();
				//script->SetPumpObject(vPumpkinObject[i]);
				pObject->SetCompoent(std::make_unique<UpDownPatrol>());

				SPACE->AddStatictTerrainObject(pObject);
			}
		}
	#pragma endregion

	#pragma region[구름 배치]

		auto pCloud01Shader = CONTENTMGR->MakeStaticTerrainShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "CloudA");
		//pCloud01Shader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/Cloud01.FBX", CLOUD_SCALE, SH_FBXTYPE::FBX_VOXEL)));
		pCloud01Shader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/CloudA.warp3d")));
		pCloud01Shader->SetMaterial(pd3dDevice, "../Assets/Voxel/untitled.png");

		auto pCloud02Shader = CONTENTMGR->MakeStaticTerrainShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "CloudB");
		//pCloud02Shader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/Cloud02.FBX", CLOUD_SCALE, SH_FBXTYPE::FBX_VOXEL)));
		pCloud02Shader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/CloudB.warp3d")));
		pCloud02Shader->SetMaterial(pd3dDevice, "../Assets/Voxel/untitled.png");

		//KYT '17.04.18
		/*
			MapOnoff를 위함
		*/
		//
			m_vMapPath.push_back("CloudA");
			m_vMapPath.push_back("CloudB");
		//

		float x = 0.0f;   float z = 0.0f;   float y = 1.0f;
		int cloudCount = 0;
		for (int i = 1; i < 23; ++i )
		{
			for (int j = 1; j < 14; ++j)
			{
				if (i % 2) continue;
				std::shared_ptr<CGameObject> pCloudObject = std::make_shared<CGameObject>();

				x = 10.0f * i + 20.0f;
				y = 10.0f;
				z = 10.0f * j + 20.0f;

				pCloudObject->SetPosition(XMFLOAT3(x, y, z));
				pCloudObject->Rotate(0.0f, 180.0f, 0.0f);

				if (cloudCount % 2)
				{
					pCloudObject->SetShader(pCloud01Shader);
				}
				else
				{
					pCloudObject->SetShader(pCloud02Shader);
				}
				if (j % 2) pCloudObject->Rotate(0.0f,180.0f, 0.0f);

				auto script = std::make_unique<CloudPatrol>();
				script->SetSpeed((cloudCount%3)+1.0f);

				pCloudObject->SetCompoent(move(script));

				SPACE->AddStatictTerrainObject(pCloudObject);
				cloudCount++;
			}
		}

	#pragma endregion

	#pragma region[Water 배치]
		vPath.clear();
		vPath.resize(3);
		vPath[0] = ("../Assets/03_Image/WaterWave/WaterDiffuse.png");
		vPath[1] = ("../Assets/03_Image/WaterWave/WaterDispalce.png");
		vPath[2] = ("../Assets/03_Image/WaterWave/WaterNormal.png");

		//JJY '16.05.15
		/*
			물 높이 수정
		*/
		//안녕하세요?.
		{
			auto pTerrainShader = std::make_shared<CTSCubeShader>();
			pTerrainShader->CreateShader(pd3dDevice);
			pTerrainShader->SetMesh(move(std::make_unique<CTSCubeMesh>(pd3dDevice, 96, 1, 200)));
			pTerrainShader->SetMaterial(pd3dDevice, vPath);
			pTerrainShader->SetBlendState("Gauge");
			//m_pPlaneShader->SetMaterial(pd3dDevice, "../Assets/01_MapData/MapVoxelTexture/Terrain.png");

			std::shared_ptr<CGameObject> pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(128.0f, -0.8f, 75.0f);
			pObject->SetShader(pTerrainShader);
			pTerrainShader->AddObject(pObject);

			pTerrainShader->BuildObjects(pd3dDevice);
			m_vPlaneShader.push_back(pTerrainShader);
		}
	#pragma endregion
	
	#pragma region[복셀 맵 배치]

		float mapscale = 1.0f;
		float mapheight = -0.5f;

			//RSH '16.05.08
			/*
				맵 위치 수정
				충돌박스를 기준으로한 맵 위치 수정
			*/

			XMFLOAT3 mapcenter = XMFLOAT3(-0.5f, 0.0f, -0.5f);

			pShader.reset();
			pShader = std::make_shared<CChochShader>();
			pShader->CreateShader(pd3dDevice);

			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/ChocoCliff.FBX", mapscale, SH_FBXTYPE::FBX_VOXEL)));
			//pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/Cliff_chocotexture.warp3d")));
			pShader->SetMaterial(pd3dDevice, "../Assets/01_MapData/MapVoxelTexture/choco_pattern.png");

			auto pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(mapcenter.x, mapcenter.y, mapcenter.z);
			pObject->SetShader(pShader);
			pShader->AddObject(pObject);
			pShader->BuildObjects(pd3dDevice);
			m_vPlaneShader.push_back(pShader);

			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader->CreateShader(pd3dDevice);
			
			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/ChocoWall.FBX", mapscale, SH_FBXTYPE::FBX_VOXEL)));
			//pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/Cliff_chocotexture.warp3d")));
			pShader->SetMaterial(pd3dDevice, "../Assets/01_MapData/MapVoxelTexture/choco_pattern.png");
			
			pObject.reset();
			pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(mapcenter.x, mapcenter.y, mapcenter.z);
			pObject->SetShader(pShader);
			pShader->AddObject(pObject);
			pShader->BuildObjects(pd3dDevice);
			m_vPlaneShader.push_back(pShader);

		//------------------------------------------------------------------------------------------------------------------------------------//

			
			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader->CreateShader(pd3dDevice);

			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/Mground_grass.FBX", mapscale, SH_FBXTYPE::FBX_VOXEL)));
			//pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Map/Mground_grass.warp3d")));
			pShader->SetMaterial(pd3dDevice, "../Assets/01_MapData/MapVoxelTexture/grasspattern.png");

			pObject.reset();
			pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(mapcenter.x, mapcenter.y, mapcenter.z);
			pObject->SetShader(pShader);
			pShader->BuildObjects(pd3dDevice);
			pShader->AddObject(pObject);
			m_vPlaneShader.push_back(pShader);

		//RSH '16.08.20
		/*
			- 새로 맵핑한 좌우 그라운드(모델 파일 상으로 분할됨) 렌더링
		*/
			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader->CreateShader(pd3dDevice);
			//pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/LRground_checker.FBX", mapscale, SH_FBXTYPE::FBX_VOXEL)));
			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/LRground_3x3mapping.FBX", mapscale, SH_FBXTYPE::FBX_VOXEL)));

			//imsi 터짐
			pShader->SetMaterial(pd3dDevice, "../Assets/01_MapData/MapVoxelTexture/ground_texture1.png");
			
			pObject.reset();
			pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(mapcenter.x, mapcenter.y, mapcenter.z);
			pObject->SetShader(pShader);
			pShader->BuildObjects(pd3dDevice);
			pShader->AddObject(pObject);
			m_vPlaneShader.push_back(pShader);

		//------------------------------------------------------------------------------------------------------------------------------------//
			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader->CreateShader(pd3dDevice);
			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/WoodBridge.FBX", mapscale, SH_FBXTYPE::FBX_VOXEL)));
			pShader->SetMaterial(pd3dDevice, "../Assets/01_MapData/MapVoxelTexture/CartoonWood_512x512.png");

			pObject.reset();
			pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(mapcenter.x, mapcenter.y, mapcenter.z);
			pObject->SetShader(pShader);
			pShader->BuildObjects(pd3dDevice);
			pShader->AddObject(pObject);
			m_vPlaneShader.push_back(pShader);

		//------------------------------------------------------------------------------------------------------------------------------------//
			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader->CreateShader(pd3dDevice);
			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/DecorateBridge.FBX", mapscale, SH_FBXTYPE::FBX_VOXEL)));
			pShader->BuildObjects(pd3dDevice);

			pObject.reset();
			pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(mapcenter.x, mapcenter.y, mapcenter.z);
			pObject->SetShader(pShader);
			pShader->AddObject(pObject);
			m_vPlaneShader.push_back(pShader);
		//--------------------------------------------------------------------------------------------------------------------------





		//--------------------------------------------------------------------------------------------------------------------------
			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader = CONTENTMGR->MakeStaticTerrainShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "../Assets/Voxel/voxel_logoRedsupport.FBX");
			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/Voxel/voxel_logoRedsupport.FBX", 0.1f, SH_FBXTYPE::FBX_VOXEL)));

			pObject.reset();
			pObject = std::make_shared<CGameObject>("점령지");
			pObject->SetPosition(126.0f, mapcenter.y, 24.0f);
			pObject->SetShader(pShader);
			SPACE->AddStatictTerrainObject(pObject);

			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader = CONTENTMGR->MakeStaticTerrainShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "../Assets/Voxel/voxel_logoBluesupport.FBX");
			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/Voxel/voxel_logoBluesupport.FBX", 0.1f, SH_FBXTYPE::FBX_VOXEL)));

			pObject.reset();
			pObject = std::make_shared<CGameObject>("점령지");
			pObject->SetPosition(126.0f, mapcenter.y, 85.0f);
			pObject->SetShader(pShader);
			SPACE->AddStatictTerrainObject(pObject);
		//--------------------------------------------------------------------------------------------------------------------------


		//--------------------------------------------------------------------------------------------------------------------------
			mapcenter.y += 3;
			pShader.reset();
			pShader = std::make_shared<CInstancedObjectsShader>();
			pShader = CONTENTMGR->MakeStaticTerrainShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "../Assets/Voxel/voxel_warplogo.FBX");
			pShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/Voxel/voxel_warplogo.FBX", 0.05f, SH_FBXTYPE::FBX_VOXEL)));


			std::unique_ptr<CRotatingScript> script = std::make_unique<CRotatingScript>("로테이셩");
			script->SetRotationSpeed(250.0f);
			script->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

			pObject.reset();
			pObject = std::make_shared<CGameObject>("WarpLogo");
			pObject->SetPosition(126.0f, mapcenter.y, 24.0f);
			pObject->SetCompoent(move(script));
			pObject->SetShader(pShader);
			SPACE->AddStatictTerrainObject(pObject);

			pObject.reset();
			pObject = std::make_shared<CGameObject>("WarpLogo");
			pObject->SetPosition(126.0f, mapcenter.y, 85.0f);
			pObject->SetShader(pShader);

			script.reset();
			script = std::make_unique<CRotatingScript>();
			script->SetRotationSpeed(150.0f);
			script->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
			pObject->SetCompoent(move(script));

			SPACE->AddStatictTerrainObject(pObject);
		//--------------------------------------------------------------------------------------------------------------------------
	#pragma endregion

	#pragma region[중앙그라운드 꽃 장식 배치]
			//KYT '17.04.18
			/*
				MapOnoff를 위함
			*/
			//
				m_vMapPath.push_back("Daissies");
				m_vMapPath.push_back("OrangePoppies");
				m_vMapPath.push_back("PurpleTulips");
				m_vMapPath.push_back("Orchids");
			//


			//RSH '16.05.15
			/*
				중앙그라운드 꽃 장식 배치
			*/

			auto pDaissieShader = CONTENTMGR->MakeStaticShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "Daissies");
			//pDaissieShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate01_Dasisies.FBX", DECORATE_SCALE, SH_FBXTYPE::FBX_VOXEL)));
			pDaissieShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate01_Dasisies.warp3d")));

			auto pOrangePoppyShader = CONTENTMGR->MakeStaticShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "OrangePoppies");
			//pOrangePoppyShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate02_OrangePoppies.FBX", DECORATE_SCALE, SH_FBXTYPE::FBX_VOXEL)));
			pOrangePoppyShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate02_OrangePoppies.warp3d")));

			auto pPurpleTulipShader = CONTENTMGR->MakeStaticShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "PurpleTulips");
			//pPurpleTulipShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate03_PurpleTulips.FBX", DECORATE_SCALE, SH_FBXTYPE::FBX_VOXEL)));
			pPurpleTulipShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate03_PurpleTulips.warp3d")));

			auto pOrchidShader = CONTENTMGR->MakeStaticShaderFactoty<CInstancedObjectsShader>(pd3dDevice, "Orchids");
			//pOrchidShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate04_Orchids.FBX", DECORATE_SCALE, SH_FBXTYPE::FBX_VOXEL)));
			pOrchidShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/01_MapData/Decorate/Decorate04_Orchids.warp3d")));

			auto vObjectPosition = Helper::MapCsvLoader("../Assets/01_MapData/CSV/", "Mground_Decorate");
			if (vObjectPosition.size() == 0) ::GMessageBox("CSV Data Empty", "Mground_Decorate");

			int cnt = 0;
			for (auto &objPos : vObjectPosition)
				{
					auto pObject = std::make_shared<CGameObject>();

					pObject->SetPosition(objPos.x, objPos.y, objPos.z);
					if (cnt % 2)pObject->Rotate(0.0f, 90.0f, 0.0f);
					switch (cnt % 4)
					{
						case 0:
							pObject->SetShader(pDaissieShader);
							break;
						case 1:
							pObject->SetShader(pOrangePoppyShader);
							break;
						case 2:
							pObject->SetShader(pPurpleTulipShader);
							break;
						case 3:
							pObject->SetShader(pOrchidShader);
							break;
					}
					cnt++;

					SPACE->AddStatictTerrainObject(pObject);
				}
		#pragma endregion

	#pragma region [WaterFall]
	
		//------------------------------------------------------------------------------------------------------------------------------------//
			//RSH '16.08.19
			/*
				- 폭포메쉬 렌더링 작업 중. 최신 쉐이더 개발 중
			*/
			
			//m_pWaterFallShader = std::make_shared<CInstancedObjectsShader>();
			m_pWaterFallShader = std::make_unique<CInstancedSpriteShader>();
			m_pWaterFallShader->CreateShader(pd3dDevice);
			m_pWaterFallShader->SetMesh(move(std::make_unique<CFBXSingleMeshTexturedIlluminated>(pd3dDevice, "../Assets/01_MapData/MapFBX/WaterFall.FBX", 1.0f, SH_FBXTYPE::FBX_VOXEL)));
			m_pWaterFallShader->SetMaterial(pd3dDevice, "../Assets/03_Image/Sprite/waterfall_sprite_190_200.png");
			m_pWaterFallShader->BuildObjects(pd3dDevice);
			
			m_pWaterFallShader->Initialize(pd3dDevice, 1, 11);
			//m_pWaterFallShader->SetBlendState("BillBoardAlpha");
			m_pWaterFallShader->SetBlendState("Gauge");

			pObject.reset();
			pObject = std::make_shared<CGameObject>();
			pObject->SetPosition(0.0f, -2.0f, -3.0f);
			//pObject->SetShader(pWaterFallShader);
			m_pWaterFallShader->AddObject(pObject);
			//m_vPlaneShader.push_back(pWaterFallShader);
		//------------------------------------------------------------------------------------------------------------------------------------//

	#pragma endregion

}

void CCubeTerrainManager::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	for (auto &pShader : m_vPlaneShader)
	{
		if (pShader == *m_vPlaneShader.begin())
		{
			pShader->AnimateObjects(TIMEMGR->GetTimeElapsed());
			pShader->Render(pd3dDeviceContext, nullptr);
		}
		else
			pShader->RenderTerrain(pd3dDeviceContext, nullptr);
	}
	//if(m_pWaterFallShader)
	//	m_pWaterFallShader->RenderNoClear(pd3dDeviceContext, nullptr);

	pd3dDeviceContext->CopyResource(nullptr, nullptr);
}

void CCubeTerrainManager::Animate(float fTimeElasped)
{
	for (auto &pShader : m_vPlaneShader)
		pShader->AnimateObjects(fTimeElasped);
}
