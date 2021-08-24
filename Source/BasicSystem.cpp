#include "stdafx.h"
#include "BasicSystem.h"

#include "FontManager.h"
#include "TerrainPlayer.h"
#include "OtherPlayer.h"

CBasicSystem::CBasicSystem() 
	: CGameSystem()
	,  width(FRAME_BUFFER_WIDTH)
	, height(FRAME_BUFFER_HEIGHT)
{
	name = "BasicSystem";
	m_iPlayerKillCount = 0;
	m_iPlayerDeathCount = 0;
	m_iPlayerScore = 0;
}

CBasicSystem::~CBasicSystem()
{
}

void CBasicSystem::Release()
{
}

void CBasicSystem::Load(ID3D11Device* pd3dDevice)
{
	m_pd3dDvice = pd3dDevice;


	//-----------------------------------------
		m_fmsgTime.fontSize = 15;
		m_fmsgTime.x = 50;
		m_fmsgTime.y = 50;
		m_fmsgTime.color = YT_Color(0, 0, 0);
	//-----------------------------------------

	//-----------------------------------------

	//-----------------------------------------
			
	m_fMaxTime			= GAME_PLAY_TIME_SECOND_LIMIT;
	m_nRemainTime		= m_fMaxTime;
	width				= FRAME_BUFFER_WIDTH;
	height				= FRAME_BUFFER_HEIGHT;

	m_bBulletin = true;

	m_sBulletinView = "0BulletinView_TabPanel";

}

bool CBasicSystem::OnCollision()
{
	//KYT '16.07.11
	/*
		���뾲
		���Ⱑ �÷��̾�� ��ġ �޾ƿͼ� ���������� �浹üũ�ϴ°���!
	*/
	//Player���� shared_ptr�� �޴� vector�� �����.
	std::vector<std::shared_ptr<CGameObject>> vPlayers;

	//Player�� OtherPlayer�� vector�� �����Ѵ�.
	vPlayers.push_back(PLAYER->GetPlayer());
	auto vOtherPlayer = PLAYER->GetOtherPlayer();
	for (auto &pOtherPlayer : vOtherPlayer)  vPlayers.push_back(pOtherPlayer);

	int size = vPlayers.size();
	for (int i = 0; i < size; ++i)
	{
		vPlayers[i]->GetBoundingBox();

		SPACE->Update(vPlayers[i]);
		CSpaceNodeQuad* room = vPlayers[i]->GetSpaceNodeQuad();	//���� ������޾ƿ´�.

		for (auto &pObject : room->GetStaticObjectsContainer()) //StaticObject���� �浹
		{
			if (pObject == vPlayers[i]) continue;
			vPlayers[i]->IntersectsStatic(pObject);
		}

		for (auto &pObject : room->GetDynamicObjectsContainer()) //DynamicObject���� �浹
		{
			if (pObject == vPlayers[i]) continue;
			vPlayers[i]->IntersectsDynamic(pObject);
		}
	}


	#ifndef _CONNECT_SERVER_

	//KYT '16.07.11
	/*
		������ ���� 
		���Žùڽ��� ���̰�!! ��ڽ��� ���̴ϱ�
		���� ��ġ�� �ʰ� �ھƾƾƾ˸��� �浹üũ �����˴ϴ�
	*/
	#ifdef BOUNDING_ORIENTED_BOX
		DirectX::BoundingOrientedBox bbHandBox = static_cast<PlayerState*>(vPlayers[0]->GetComponent(0))->GetSmashBoundingBox();
	#else
		BoundingBox bbHandBox = static_cast<PlayerState*>(vPlayers[0]->GetComponent(0))->GetSmashBoundingBox();
	#endif
	
	//KYT '16.07.11

		// Player[i]�� �� - Player[j]�� ��
		for (int i = 1;  i < size; ++i)
		{
			if (bbHandBox.Intersects(static_cast<PlayerState*>(vPlayers[i]->GetComponent(0))->GetBackBoudingBox()))
			{
				//TODO
				//�浹üũ �ؾߵǴ� ����! ��� Ȱ��ȭ�� ��Ų��!
				vPlayers[0]->GetComponent(1)->Collision(vPlayers[i], ObjectType::PlayerBackBoundingBox);
				vPlayers[i]->GetComponent(1)->Collision(vPlayers[0], ObjectType::PlayerSmashBoundingBox);
				
				#ifdef DEBUG_MODE_UI
					FONTMGR->MessagePush( 10, 10, 50, YT_Color(255, 0, 0), L"�ε�ħ -", i);
				#endif
			}
		}

	#endif
	return true;
}

void CBasicSystem::Update(float fTimeElapsed)
{

	CGameSystem::UpdateTime(fTimeElapsed);
	if (m_nRemainTime <= 0.0f)
		m_nGameState = GAME_PLAY_STATE::GAME_PLAY_STOP;

	m_nRemainTime = m_fMaxTime - m_fGameTime;

	//KYT '16.07.29
	/*
		�ǳ� ���̰� �ϴ°���
		0BulletinPlayerIcon����
		'0'�� �Ϻη� ���ΰ���
		cuz. ���Ķ�����
	*/

	if (INPUT->KeyDown(YT_KEY::YK_TAB))
	{
		if (m_bBulletin)
		{
			//UIMGR->UseShader(m_sBulletinView);
			UIMGR->UseShader(m_sBulletinView + to_string(1));

			UIMGR->EndUseShader("3d_dpy_map");
			UIMGR->EndUseShader("3d_dpy_Bteam_occupy");
			UIMGR->EndUseShader("3d_dpy_map_view");

			//v�÷��̾� �� 
			for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
			{
				UIMGR->UseShader("0BulletinPlayer_TabIcon" + to_string(TCP->GetRoomPlayerInfo(idx).iSelectedTeam));
				std::cout << " BulletPlayerIcon  :" << idx << std::endl;
			}
			
			m_bBulletin = false;
		}

	}
	else
	{
		if (!m_bBulletin)
		{
			//UIMGR->EndUseShader(m_sBulletinView);
			UIMGR->EndUseShader(m_sBulletinView + to_string(1));

			UIMGR->UseShader("3d_dpy_map");
			UIMGR->UseShader("3d_dpy_Bteam_occupy");
			UIMGR->UseShader("3d_dpy_map_view");

			//v�÷��̾� �� 
			for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
				UIMGR->EndUseShader("0BulletinPlayer_TabIcon" + to_string(TCP->GetRoomPlayerInfo(idx).iSelectedTeam));

			m_bBulletin = true;
		}
	}
}

void CBasicSystem::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	FONTMGR->MessagePush(m_fmsgTime.fontFaimly, m_fmsgTime.x, m_fmsgTime.y, m_fmsgTime.color, m_fmsgTime.pivot, L"Time : %d", m_fGameTime);
	
	if(false == INPUT->KeyDown(YK_TAB))
		FONTMGR->MessagePush("�⺻", (width / 2) - 70, 35, 30, YT_Color(255, 162, 0), L"�����ð� : %d " ,m_nRemainTime);

#ifdef _CONNECT_SERVER_
	for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
	{
		if (TCP->GetRoomPlayerInfo(idx).iSelectedTeam == TCP->my_info.iSelectedTeam)
		{
			m_iPlayerKillCount = TCP->GetRoomPlayerInfo(idx).iKillNum;
			m_iPlayerDeathCount = TCP->GetRoomPlayerInfo(idx).iDeathNum;
			m_iPlayerScore - TCP->GetRoomPlayerInfo(idx).iScoreNum;
			break;
		}
	}

	FONTMGR->MessagePush("�⺻", width/(7.9f), 615, 20, YT_Color(255, 0, 0), L"%d", m_iPlayerKillCount);
	FONTMGR->MessagePush("�⺻", width /(6.2f), 615, 20, YT_Color(0, 0, 255), L"%d", m_iPlayerDeathCount);
	FONTMGR->MessagePush("�⺻", width / 5, 615, 20, YT_Color(255, 0, 255), L"%d", m_iPlayerScore);
#else
	FONTMGR->MessagePush("�⺻", width / (7.9f), 615, 20, YT_Color(255, 0, 0), L"X");
	FONTMGR->MessagePush("�⺻", width / (6.2f), 615, 20, YT_Color(0, 0, 255), L"X");
	FONTMGR->MessagePush("�⺻", width / 5, 615, 20, YT_Color(255, 0, 255), L"X");
#endif
	if (INPUT->KeyDown(YT_KEY::YK_TAB))  RenderTabUIFont();

}

void CBasicSystem::ChangeState()
{
	::GMessageBox(L"���� ����!", L"������ ������!)");
	::DimensionsChange(YT_GameMode::GAME_MODE_DIALOG);
}

void CBasicSystem::RenderTabUIFont()
{
#pragma region[Tab_UI_Font]
		int mIDViewX = 260;
		int mKillViewX = 450 -80 + 35; 
		int mDeathViewX = 590 -80 + 45;
		int mMonsterKillViewX = 730 -80 + 110;
		int mScoreViewX = 970 -80 + 80;

	#ifdef _CONNECT_SERVER_
		//ĳ���͸� 
		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			UINT selected_team = TCP->GetRoomPlayerInfo(idx).iSelectedTeam;
			
			std::string playerId = "TEST_0000";

			TCHAR tchar_UserName[256] = { 0, };
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, TCP->GetRoomPlayerInfo(idx).UserID, strlen(TCP->GetRoomPlayerInfo(idx).UserID), tchar_UserName, 256);

			if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mIDViewX + TapUI_RIGHTPUSH, 175 + (selected_team * 100) + TapUI_DOWNPUSH, 20, YT_Color(255, 255, 255)
					, tchar_UserName //<-���⿡ ��ġ�� �־��ָ� ��µ˴ϴ�.
				);
			}
			else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mIDViewX + TapUI_RIGHTPUSH, 395 + ((selected_team - 2) * 100) + TapUI_DOWNPUSH, 20, YT_Color(255, 255, 255)
					, tchar_UserName //<-���⿡ ��ġ�� �־��ָ� ��µ˴ϴ�.
				);
			}
		}

		//óġ ��ġ
		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			UINT selected_team = TCP->GetRoomPlayerInfo(idx).iSelectedTeam;
	
			UINT mKillCount = TCP->GetRoomPlayerInfo(idx).iKillNum;
	
			if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mKillViewX + TapUI_RIGHTPUSH, 125 + (selected_team * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", mKillCount //<-���⿡ ��ġ�� �־��ָ� ��µ˴ϴ�.
				);
			}
			else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mKillViewX + TapUI_RIGHTPUSH, 345 + ((selected_team - 2) * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", mKillCount //<-���⿡ ��ġ�� �־��ָ� ��µ˴ϴ�.
				);
			}
		}
	
		//���� ��ġ
		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			UINT selected_team = TCP->GetRoomPlayerInfo(idx).iSelectedTeam;
	
			UINT mDeathCount = TCP->GetRoomPlayerInfo(idx).iDeathNum;
	
			if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mDeathViewX, 125 + (selected_team * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", mDeathCount
				);
			}
			else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mDeathViewX, 345 + ((selected_team - 2) * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", mDeathCount
				);
			}
		}
	
		//���� óġ ��ġ
		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			UINT selected_team = TCP->GetRoomPlayerInfo(idx).iSelectedTeam;
	
			UINT mMonsterKillCount = TCP->GetRoomPlayerInfo(idx).iMonsterKillNum;
	
			if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mMonsterKillViewX, 125 + (selected_team * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", mMonsterKillCount
				);
			}
			else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mMonsterKillViewX, 345 + ((selected_team - 2) * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", mMonsterKillCount
				);
			}
		}
	
		//���� ��ġ
		for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
		{
			UINT selected_team = TCP->GetRoomPlayerInfo(idx).iSelectedTeam;
	
			UINT score = TCP->GetRoomPlayerInfo(idx).iScoreNum;
	
			if (selected_team == A_TEAM_0 || selected_team == A_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mScoreViewX, 125 + (selected_team * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", score
				);
			}
			else if (selected_team == B_TEAM_0 || selected_team == B_TEAM_1)
			{
				FONTMGR->MessagePush
				(
					"�⺻", mScoreViewX, 345 + ((selected_team - 2) * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
					, L"%d", score
				);
			}
		}
	#else
		//RSH '16.0815
		/*
			���� Ŭ���̾�Ʈ���� ���� Tab UI��ġ.
		*/

		//óġ
		FONTMGR->MessagePush("�⺻", mKillViewX, 125 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 0);
		
		FONTMGR->MessagePush("�⺻", mKillViewX, 125 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 0);
		
		FONTMGR->MessagePush("�⺻", mKillViewX, 345 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 0);
		
		FONTMGR->MessagePush("�⺻", mKillViewX, 345 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 0);
		
		
		//����
		FONTMGR->MessagePush("�⺻", mDeathViewX, 125 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 0);
		
		FONTMGR->MessagePush("�⺻", mDeathViewX, 125 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 1);
		
		FONTMGR->MessagePush("�⺻", mDeathViewX, 345 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 2);
		
		FONTMGR->MessagePush("�⺻", mDeathViewX, 345 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 3);
		
		//���� óġ
		FONTMGR->MessagePush("�⺻", mMonsterKillViewX, 125 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 0);
		
		FONTMGR->MessagePush("�⺻", mMonsterKillViewX, 125 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 1);
		
		FONTMGR->MessagePush("�⺻", mMonsterKillViewX, 345 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 2);
		
		FONTMGR->MessagePush("�⺻", mMonsterKillViewX, 345 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 3);
		
		//����
		FONTMGR->MessagePush("�⺻", mScoreViewX, 125 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 0);
		
		FONTMGR->MessagePush("�⺻", mScoreViewX, 125 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 1);
		
		FONTMGR->MessagePush("�⺻", mScoreViewX, 345 + (0 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 2);
		
		FONTMGR->MessagePush("�⺻", mScoreViewX, 345 + (1 * 90) + TapUI_DOWNPUSH, 55, YT_Color(255, 255, 255)
			, L"%d", 3);
	#endif
#pragma endregion
}