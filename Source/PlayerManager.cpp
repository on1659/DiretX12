#include "stdafx.h"
#include "PlayerManager.h"

//Height
#include "HeightMapTerrain.h"

//Mesh
#include "ModelParsingMesh.h"
#include "TerrainCubeMesh.h"
#include "FBXSingleMesh.h"
#include "WARP3DMesh.h"
#include "Warp3D_PlayerMesh.h"

//Object
#include "GameObject.h"
#include "TerrainPlayer.h" 
#include "OtherPlayer.h"

//Component

#include "PlayerAnimation.h"
#include "PlayerComponet.h"

#include "OtherPlayerAnimation.h"
#include "OtherPlayerComponent.h"

#include "Teleport.h"
#include "ThrowSkill.h"
#include "PlayerHeadBangBang.h"
#include "EvasionSkill.h"
#include "SpeedSkill.h"
#include "HideSkill.h"

//Shader
#include "InstancedObjectsShader.h"
#include "Warp3DPlayerShader.h"
#include "Warp3DOtherPlayerShader.h"

#include "Timer.h"

#include "Warp3DModelManager.h"
#include "ContentManager.h"
#include "ShadowMapShader.h"

CPlayerManager::CPlayerManager()
{
	m_pPlayer = nullptr;
	m_pCamera = nullptr;
}

CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::ReleseInstance()
{
	if(m_pPlayerShader) m_pPlayerShader->ReleaseObjects();
	m_pPlayerShader.reset();

	if (m_pPlayer)
	{
		m_pPlayer->Release();	
		//m_pPlayer.reset();
	}
	if (m_pCamera) m_pCamera.reset();

	for (auto &shader : m_pOtherPlayerShader)
	{
		shader->ReleaseObjects();
	}

	for (auto &obj : m_pOtherPlayer)obj->Release();

	if(m_pShadowCamera)m_pShadowCamera.reset();

}

void CPlayerManager::Load(ID3D11Device* pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	XMFLOAT3 boundboxExtents = XMFLOAT3(BLOCK_SCALE * 0.15, BLOCK_SCALE * 0.4, BLOCK_SCALE * 0.15 );

	m_fSpeed = CHARACTER_SPEED * BLOCK_SCALE;

	ID3D11DeviceContext* pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

	//�÷��̾� ��� �ٲٱ�
	m_fPlayerSize = BLOCK_SCALE * 0.8f  * 0.5f;
	//auto pPlayerMesh = std::make_unique<CCubeMeshTexturedIlluminated>(pd3dDevice, m_fPlayerSize, m_fPlayerSize * 2.0f, m_fPlayerSize);

	#ifdef _CONNECT_SERVER_

		//JJY '16.05.01
		/*
			ĳ���� ���ÿ� ���� �� ����� �ٲߴϴ�.
		*/
		std::string meshPath;
		std::string materialPath;
		std::string iconPath;

		m_pPlayer = std::make_shared<CTerrainPlayer>();

		auto select = TCP->my_info.iSelectedCharacter;

		if (select == (CHARACTER_MEI)) //Mei
		{

			meshPath = "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d";
			materialPath = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Atype.png";
			iconPath = "3d_dpy_Ateam_character_1";

			m_pPlayer->GetPlayerState()->SetCharcaterName("MEI");
			m_pPlayer->SetCompoent(std::make_unique<TeleportSkill>());
			m_pPlayer->SetCompoent(std::make_unique<EvasionSkill>());
			m_pPlayer->SetCompoent(std::make_unique<HideSkill>());
		}

		if (select == (CHARACTER_LIZZY)) //Lizzy
		{
			meshPath = "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d";
			materialPath = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Btype.png";
			iconPath = "3d_dpy_Ateam_character_2";

			m_pPlayer->GetPlayerState()->SetCharcaterName("LIZZY");
			m_pPlayer->SetCompoent(std::make_unique<ThrowSkill>());
			m_pPlayer->SetCompoent(std::make_unique<SpeedSkill>());
			m_pPlayer->SetCompoent(std::make_unique<HideSkill>());
		}

		m_pPlayerShader = CONTENTMGR->MakeDynamicShaderFactoty<CQueryChanPlayerShader>(pd3dDevice, "Player");// std::make_shared<COtherJeniShader>();

		m_pPlayer->name = "Player";
		m_pPlayer->SetShader(m_pPlayerShader);
		m_pPlayer->SetShader(nullptr);
		m_pPlayer->SetMesh(nullptr);
		m_pPlayer->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, meshPath)));


		m_pPlayer->SetMaterial(CONTENTMGR->GetMaterial(pd3dDevice, materialPath)); //�� �б�1
		m_pPlayer->ChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, 0.0f);

		//KYT '16.07.29
		/*
			�������� ���� ĳ����
			0�� ���� Ŭ�󿡼��� ���� �÷��̾ ������ 0���̱� ������
		*/
		m_pPlayer->GetPlayerState()->SetPath(iconPath); //���ϴϱ�
		HASHTABLE->PlayerLoad(iconPath, 0);

		auto pMesh = new Default_Mesh(pd3dDevice, 0.3, 0.4, 0.3);
		m_pPlayer->SetBoundingBox(BoundingBox(XMFLOAT3(0, 0, 0), boundboxExtents));


	#else

	#ifdef _FBXANIMATION_LOADING_
		//m_pPlayerShader = std::make_shared<CJunoShader>();
		m_pPlayerShader = CONTENTMGR->MakeDynamicShaderFactoty<CQueryChanPlayerShader>(pd3dDevice, "Player");// std::make_shared<COtherJeniShader>();
	#else
		m_pPlayerShader = CONTENTMGR->MakeDynamicShaderFactoty<CWarp3DSkinningShader>(pd3dDevice, "Player");
	#endif

		//JJY '16.05.01
		/*
			ĳ���� ���ÿ� ���� �� ����� �ٲߴϴ�.
		*/

		//KYT '16.08.02
		/*
		
			skill
			 190.0f - 245.0f = X
				  490.0f	 = Y
		
			Player Load
			1. PlayerObject ����
			2. �� ĳ���Ϳ� �´� path ����
			3. �� ĳ���Ϳ� �´� ������Ʈ ����
		*/


		std::string meshPath;
		std::string materialPath;
		std::string iconPath;

		int select = 0;
		//std::cout << "ĳ���͸� ��� �ּ��� ! 0 -> Mei /// 1 = Lizzy : ";
		//std::cin >> select;


		m_pPlayer = std::make_shared<CTerrainPlayer>();
		
		if (select == static_cast<int>(CHARACTER_MEI)) //Mei
		{

			meshPath = "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d";
			materialPath = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Atype.png";
			iconPath = "3d_dpy_Ateam_character_1";

			m_pPlayer->GetPlayerState()->SetCharcaterName("MEI");

			m_pPlayer->SetCompoent(std::make_unique<TeleportSkill>());
			m_pPlayer->SetCompoent(std::make_unique<EvasionSkill>());
			m_pPlayer->SetCompoent(std::make_unique<HideSkill>());
		}

		if (select == static_cast<int>(CHARACTER_LIZZY)) //Lizzy
		{
			meshPath = "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d";
			materialPath = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Btype.png";
			iconPath = "3d_dpy_Ateam_character_2";
			
			m_pPlayer->GetPlayerState()->SetCharcaterName("LIZZY");

			m_pPlayer->SetCompoent(std::make_unique<ThrowSkill>());
			m_pPlayer->SetCompoent(std::make_unique<SpeedSkill>());
			m_pPlayer->SetCompoent(std::make_unique<HideSkill>());
		}


		//m_pPlayerShader = CONTENTMGR->MakeDynamicShaderFactoty<CQueryChanPlayerShader>(pd3dDevice, "Player");// std::make_shared<COtherJeniShader>();

		m_pPlayer->name = "Player";
		m_pPlayer->SetShader(m_pPlayerShader);
		m_pPlayer->SetShader(nullptr);
		m_pPlayer->SetMesh(nullptr);
		m_pPlayer->SetMesh(std::move(std::make_unique<CWarp3D_PlayerMesh>(pd3dDevice, meshPath)));
		m_pPlayerShader->SetMesh(std::move(std::make_unique<CWarp3D_PlayerMesh>(pd3dDevice, meshPath)));
		((CQueryChanPlayerShader*)m_pPlayerShader.get())->SetMeshName("Player_Test_Shader");

		std::vector<std::string> vPath;
		vPath.push_back(materialPath);
		vPath.push_back("../Assets/02_ModelData/06_Character_Query01/tex_Query01_Atype_normal.png");


		m_pPlayer->SetMaterial(CONTENTMGR->GetMaterial(pd3dDevice, vPath)); //�� �б�1
		m_pPlayer->ChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, 0.0f);

		//KYT '16.07.29
		/*
		�������� ���� ĳ����
		0�� ���� Ŭ�󿡼��� ���� �÷��̾ ������ 0���̱� ������
		*/
		m_pPlayer->GetPlayerState()->SetPath(iconPath); //���ϴϱ�
		HASHTABLE->PlayerLoad(iconPath, 0);

		m_pPlayer->SetBoundingBox(BoundingBox(XMFLOAT3(0, 0, 0), boundboxExtents));
		

	#endif

	//'16.04.01 KYT
	/*
		�÷��̾� �ʱ� ��ġ
	*/
	m_pPlayer->SetPosition(XMVectorSet(20.0f, BLOCK_SCALE, 30.0f, 1.0f));
	m_pPlayer->CreateHeadUpCube(pd3dDevice);

	m_pPlayer->GetPlayerState()->SetID(TCP->my_info.iSelectedTeam);
	//((CQueryChanPlayerShader*)m_pPlayerShader.get())->AddPlayer(m_pPlayer);
	m_pPlayerShader->BuildObjects(pd3dDevice);
	m_pPlayerShader->AddObject(m_pPlayer);
	SPACE->AddObject(m_pPlayer);

	#pragma region[Camera]
		m_pCamera = m_pPlayer->GetCamera();
		//m_pCamera->CreateConstBuffers(pd3dDevice);
		m_pCamera->SetViewport(pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->GenerateViewMatrix();
		OrthoCameraLoad(pd3dDevice);
		m_pCamera->CreateShadowBuffer(XMFLOAT3(0.0f, 50.0f, 0.0f));
	#pragma endregion

	#ifdef _CONNECT_SERVER_

	#pragma region[Other Player]
		//std::vector<std::string> vOtherPlayerTexture;
		//vOtherPlayerTexture.push_back("../Assets/02_ModelData/01_CHARACTER_MEI/tex_Jeni_diffuse.png");
		//vOtherPlayerTexture.push_back("../Assets/02_ModelData/02_CHARACTER_LIZZY/tex_Juno_diffuse.png");


		//JJY '16.05.01
		/*
			Otherplayer �ִϸ��̼� ������Ʈ ����
			AnimateScript �ε��� �־��ֱ�
			*/
		//for (auto &p : vOtherPlayerTexture)
		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			if (TCP->GetRoomPlayerInfo(idx).object_index != -1)
			{
				//JJY '16.05.01
				/*
				ĳ���� ���ÿ� ���� �ٸ� �÷��̾��� ����� �ٲߴϴ�.
				*/

				std::string strMeshName;
				std::string strTextureName;
				std::string strComponentName;
				std::string strIconName;

				std::shared_ptr<CInstancedObjectsShader> pOtherPlayerShader;

				auto pOtherPlayer = std::make_shared<COtherPlayer>();//��ġ����
				pOtherPlayer->name = "OtherPlayer" + to_string(idx);	//��ġ����
				pOtherPlayer->GetPlayerState()->SetID(idx);
				pOtherPlayer->SetPosition(XMVectorSet(20.0f + idx * 2, BLOCK_SCALE * 1.0f, 30.0f, 1.0f));
				pOtherPlayer->GetPlayerState()->SetRoomPlayerIndex(idx);

				if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_MEI)
				{
					strMeshName = "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d";
					strTextureName = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Atype.png";
					strComponentName = "QueryChanOtherPlayerStateComp";
					strIconName = "3d_dpy_Ateam_character_1";
					pOtherPlayer->GetPlayerState()->SetCharcaterName("MEI");
				}
				else if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_LIZZY)
				{
					strMeshName = "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d";
					strTextureName = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Btype.png";
					strComponentName = "QueryChanOtherPlayerStateComp";
					strIconName = "3d_dpy_Ateam_character_2";
					pOtherPlayer->GetPlayerState()->SetCharcaterName("LIZZY");
				}

				//�������ҵǴ� ���̴��� �����
				pOtherPlayerShader = CONTENTMGR->MakeDynamicShaderFactoty<CQueryChanOtherPlayerShader>(pd3dDevice, "OtherPlayerObj" + to_string(idx));// std::make_shared<COtherJeniShader>()

				auto pShader = pOtherPlayerShader;
				pOtherPlayerShader.reset();

				//���̴��� �޽ø� �ٿ��ְ�
				pShader->SetMesh(move(std::make_unique<CWARP3DMesh>(pd3dDevice, strMeshName)));

				//���׸����� �ٿ��ش㿡
				pShader->SetMaterial(pd3dDevice, strTextureName); //�� �б�1
		

				//���̴��� �ٿ��ְ�
				pOtherPlayer->SetShader(nullptr);
				pOtherPlayer->SetBoundingBox(BoundingBox(XMFLOAT3(0, 0, 0), boundboxExtents));
				pOtherPlayer->CreateHeadUpCube(pd3dDevice);
				//���̴����� ������Ʈ�� �־��ִµ� (���� �ʿ����)
				pShader->AddObject(pOtherPlayer);

				//�׸���, �� �ĺ��� �޽� �����
				static_cast<CWarp3DOtherPlayerShader*>(pShader.get())->CreateTeamMarkMesh(pd3dDevice, TCP->GetRoomPlayerInfo(idx).iSelectedTeam);
				pOtherPlayer->GetPlayerState()->SetPath(strIconName);
				HASHTABLE->PlayerLoad(strIconName, idx);


				//'16.06.28
				/*
					Team Set
				*/
				SPACE->AddObject(pOtherPlayer);
				m_pOtherPlayerShader.push_back(pShader);
				m_pOtherPlayer.push_back(pOtherPlayer);
			}

		}
	#pragma endregion

	#endif

	//KYT '16.04.21
	/*
	Player Componet Start
	*/
	for (auto &object : m_pPlayerShader->GetvRenderObject())
		object->StartComponet(object);

	#ifdef _CONNECT_SERVER_

		for (auto &shader : m_pOtherPlayerShader)
		{
			auto vObject = shader->GetvRenderObject();
			for (auto &object : vObject)
				object->StartComponet(object);
		}

	#endif

	std::cout << "Player Loader" << std::endl;
	pd3dDeviceContext->Release();


	//KYT '16.06.28
	/*
		Test Player
	*/
	#ifdef GAME_SEVER_TEST
		#pragma region[Other Player]

		std::vector<std::string> vOtherPlayerTexture;
		vOtherPlayerTexture.push_back("../Assets/02_ModelData/01_CHARACTER_MEI/tex_Jeni_diffuse.png");
		vOtherPlayerTexture.push_back("../Assets/02_ModelData/02_CHARACTER_LIZZY/tex_Juno_diffuse.png");

		for (int idx = 1; idx < NON_SERVER_CLIENT_NUMBER; ++idx)
		{

			std::string strMeshName;
			std::string strTextureName;
			std::string strComponentName;
			std::shared_ptr<CInstancedObjectsShader> pOtherPlayerShader;

			strMeshName = "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d";
			//strTextureName = "../Assets/02_ModelData/01_CHARACTER_MEI/tex_Jeni_diffuse.png";
			if(idx == 1)
				strTextureName = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Atype.png";
			else
				strTextureName = "../Assets/02_ModelData/06_Character_Query01/tex_Query01_Btype.png";
			strComponentName = "QueryChanOtherPlayerStateComp";


			//Player�� �����ʹ� PlayerStaet�� ����!
			{
				pOtherPlayerShader = std::make_shared<CQueryChanOtherPlayerShader>();

				pOtherPlayerShader->name = "OtherPlayerObj" + to_string(idx);
				pOtherPlayerShader->CreateShader(pd3dDevice);

				//CONTENTMGR->MakeDynamicShaderFactoty<CQueryChanOtherPlayerShader>(pd3dDevice, "OtherPlayerObj" + to_string(idx));// std::make_shared<COtherJeniShader>();
				auto pShader = pOtherPlayerShader;
				pOtherPlayerShader.reset();

				//���̴��� �޽ø� �ٿ��ְ�
				pShader->SetMesh(std::make_unique<CWARP3DMesh>(pd3dDevice, "../Assets/02_ModelData/06_Character_Query01/QueryChan01_SmallHead.warp3d"));
					
				//���׸����� �ٿ��ش㿡

				
				pShader->SetMaterial(pd3dDevice, strTextureName); //�� �б�1

				auto pOtherPlayer = std::make_shared<COtherPlayer>();//��ġ����
				pOtherPlayer->name = "OtherPlayer" + to_string(idx);	//��ġ����
				pOtherPlayer->GetPlayerState()->SetID(idx);
				pOtherPlayer->GetPlayerState()->SetCharcaterName("MEI");

				//KYT '16.07.23
				/*
					�ƴ��÷��̾��� �׸��� �ε����ش�
					�������� ���� ĳ����
				*/
				std::string playerPath;
				if (rand()%2)		//Read
				{
					playerPath = "3d_dpy_Ateam_character_2";
				}
				else				//Blue
				{
					playerPath = "3d_dpy_Bteam_character_2";
				}

				pOtherPlayer->GetPlayerState()->SetPath(playerPath);
				HASHTABLE->PlayerLoad(playerPath, idx);
				std::cout << "Other Player Load " << std::endl;

				//RSH 16.07.16
				/*
					ĳ���� ���濡 ���� ĳ���� ���� ���� BLOCK_SCALE*1.6f => BLOCK_SCALE*1.0f
					7�� 17�Ͽ� ���� ������ ��ȯ��, ����.
				*/
				pOtherPlayer->SetPosition(XMVectorSet(20.0f + idx * 2, BLOCK_SCALE * 1.0f, 30.0f, 1.0f));

				//���̴��� �ٿ��ְ�
				pOtherPlayer->SetShader(nullptr);

				pShader->AddObject(pOtherPlayer);
				pOtherPlayer->SetBoundingBox(BoundingBox(XMFLOAT3(0, 0, 0), boundboxExtents));
				pOtherPlayer->CreateHeadUpCube(pd3dDevice);

				//���̴����� ������Ʈ�� �־��ִµ� (���� �ʿ����)

				//���� �׸��ڶ� Hide modeť�긦 ������ش�
				((CWarp3DOtherPlayerShader*)pShader.get())->CreateTeamMarkMesh(pd3dDevice, idx);
				
				//'16.06.28
				/*
					Team Set
				*/
				SPACE->AddObject(pOtherPlayer);
				m_pOtherPlayerShader.push_back(pShader);
				m_pOtherPlayer.push_back(pOtherPlayer);
			}
		}


		for (auto &pShader : m_pOtherPlayerShader)
		{
			pShader->BuildObjects(pd3dDevice);
			auto vObject = pShader->GetvRenderObject();
			for (auto &object : vObject)
				object->StartComponet(object);
		}


	#pragma endregion
	#endif


	//'KYT '16.07.27
	/*
		Shadow~
	*/
	//m_pShadowCamera = std::make_shared<CCamera>(nullptr);
	//m_pShadowCamera->CreateConstBuffers(m_pd3dDevice);
	//m_pShadowCamera->CreateShadowBuffer(XMFLOAT3(0.0f, 50.0f, 0.0f));
	//m_pShadowCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);

}

void CPlayerManager::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pPlayer->Render(pd3dDeviceContext, m_pCamera);

	#ifdef _CONNECT_SERVER_

		//for (auto &pShader : m_pOtherPlayerShader)
		//{
		//	pShader->RenderPlayer(pd3dDeviceContext, m_pCamera);
		//}

		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			if (TCP->GetRoomPlayerInfo(idx).InRoom_MyNum == TCP->my_info.InRoom_MyNum)
				continue;


			if (TCP->GetRoomPlayerInfo(idx).ply_IsVisible == TRUE)
				m_pOtherPlayerShader[TCP->GetRoomPlayerInfo(idx).object_index]->RenderPlayer(pd3dDeviceContext, m_pCamera);

		}

	#endif


	#ifdef GAME_SEVER_TEST
		for(auto &pPlayer : m_pOtherPlayerShader)
		{
			pPlayer->RenderPlayer(pd3dDeviceContext, m_pCamera);
		}
	#endif 

	if (INPUT->OnlyKeyBoardDown(YT_KEY::YK_6))
	{

		auto pos = Helper::RuntimeDataModify(XMFLOAT3(), "xf3_OtherA");
		m_pOtherPlayer[0]->SetPosition(XMFLOAT3(pos.x, 1, pos.y));
		m_pOtherPlayer[0]->SetRotation(0, pos.z, 0);
	
		pos = Helper::RuntimeDataModify(XMFLOAT3(), "xf3_OtherB");
		m_pOtherPlayer[1]->SetPosition(XMFLOAT3(pos.x, 1, pos.y));
		m_pOtherPlayer[1]->SetRotation(0, pos.z, 0);

		pos = Helper::RuntimeDataModify(XMFLOAT3(), "xf3_OtherC");
		m_pOtherPlayer[2]->SetPosition(XMFLOAT3(pos.x, 1, pos.y));
		m_pOtherPlayer[2]->Rotate(0, pos.z, 0);

	}

	//	::gotoxy(0, 0);
	//	std::cout << "Position (" << m_pPlayer->GetPositionXMFLOAT3().x << "," << m_pPlayer->GetPositionXMFLOAT3().y << ","<< m_pPlayer->GetPositionXMFLOAT3().z << ")" << std::endl;

}

void CPlayerManager::OrthoCameraLoad(ID3D11Device* pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext)
{
	if (pd3dDeviceContext)
		m_pCamera->OrthoCameraLoad(pd3dDevice, pd3dDeviceContext);

	else
		m_pCamera->OrthoCameraLoad(pd3dDevice);
}

void CPlayerManager::Animate(float fTimeElasped) {}

void CPlayerManager::OnProcessingKeyboardMessage(UINT nMessageID, WPARAM wParam)
{
	m_pPlayer->ChangeCamera(m_pd3dDevice, DWORD(wParam - VK_F1 + 1), TIMEMGR->GetTimeElapsed());
	m_pCamera = m_pPlayer->GetCamera();
}

std::shared_ptr<CPlayer> CPlayerManager::GetOtherPlayer(int index)
{
	if (index >= m_pOtherPlayer.size())return nullptr;
	return m_pOtherPlayer[index];
}

//KYT '16.04.24
/*
	��ü�浹�� �÷��̾�� �ٸ� object�� �̷�����ٰ� ������.
	�׷��� Player ���� ���� �浹üũ�� ����� �ȴٰ� ��������.

	Step 1.  Player�� ���� ������Ʈ �մϴ�.
	2.  Player�� ���� �޾ƿɴϴ�.
	3.  Player�� Static Object üũ�� �մϴ�.
	4.  Player�� Dynamic Objectüũ�� �մϴ�.
	- �̶� ���ΰ��� �浹�� �����մϴ�.
*/
bool CPlayerManager::OnCollision()
{
	return true;
}

bool CPlayerManager::OnCollision(std::shared_ptr<CGameObject> object)
{
	//BoundingBox playerBox = m_pPlayer->GetBoundingBox();
	//if (playerBox.Intersects(object->GetBoundingBox()))
	//{
	//   object->OnCollision(m_pPlayer);
	//   return true;
	//}
	return false;
}

//JJY 16'.04.24
/*
	�÷��̾� ServerReturn_Move, ServerReturn_Rotate�Լ� �߰�
*/

void CPlayerManager::Client_DeadReckoning()
{
	//float tmp_fResizeSpeed = 1.0f;

	for (auto idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
	{
		//if (TCP->GetRoomPlayerInfo(idx).ply_IsVisible == TRUE)
		{
			if (   TCP->GetRoomPlayerInfo(idx).player_input_info.key_W_Down == false
				&& TCP->GetRoomPlayerInfo(idx).player_input_info.key_A_Down == false
				&& TCP->GetRoomPlayerInfo(idx).player_input_info.key_S_Down == false
				&& TCP->GetRoomPlayerInfo(idx).player_input_info.key_D_Down == false)
				continue;

			DWORD dwDirection = 0;

			if (TCP->GetRoomPlayerInfo(idx).player_input_info.key_W_Down)    dwDirection |= DIR_FORWARD;
			if (TCP->GetRoomPlayerInfo(idx).player_input_info.key_A_Down)    dwDirection |= DIR_LEFT;
			if (TCP->GetRoomPlayerInfo(idx).player_input_info.key_S_Down)    dwDirection |= DIR_BACKWARD;
			if (TCP->GetRoomPlayerInfo(idx).player_input_info.key_D_Down)    dwDirection |= DIR_RIGHT;

			if (dwDirection)
			{
				if (TCP->my_info.iSelectedTeam == TCP->GetRoomPlayerInfo(idx).iSelectedTeam)
				{
					m_pPlayer->Move(dwDirection, TCP->GetRoomPlayerInfo(idx).player_input_info.m_fDistance, false);
					m_pPlayer->Update(0.0f);
				}
				else
				{
					if (TCP->GetRoomPlayerInfo(idx).ply_IsVisible == TRUE)
					{
						m_pOtherPlayer[TCP->GetRoomPlayerInfo(idx).object_index]->Move(dwDirection, TCP->GetRoomPlayerInfo(idx).player_input_info.m_fDistance, false);
					}
				}
			}
		}
	}
}

void CPlayerManager::ServerReturn_Pos_Move(int obj_idx, float pos_x, float pos_z)
{
	//RSH 16.07.16
	/*
		ĳ���� ���濡 ���� ĳ���� ���� ���� BLOCK_SCALE*1.6f => BLOCK_SCALE*1.0f
		7�� 17�Ͽ� ���� ������ ��ȯ��, ����.
	*/

	if (obj_idx == -1)
	{
		m_pPlayer->SetPosition(XMFLOAT3(pos_x, BLOCK_SCALE * 1.0f, pos_z));
		m_pPlayer->Update(0.0f);		//3�� �����ڵ�
	}	
	else
	{
		m_pOtherPlayer[obj_idx]->SetPosition(XMFLOAT3(pos_x, BLOCK_SCALE * 1.0f, pos_z));
	}
}

void CPlayerManager::ServerReturn_Rotate(int obj_idx, float cxDelta, float cyDelta)
{
	if (obj_idx == -1)
	{
		m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
		m_pPlayer->Update(0.0f);	//3�� �����ڵ�
	}
	else
	{
		m_pOtherPlayer[obj_idx]->Rotate(cyDelta, cxDelta, 0.0f);
	}
}
