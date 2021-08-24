#include "stdafx.h"
#include "GameFrameWork_2D.h"

#include "SceneLobby_2D.h"
//#include "SceneResult_2D.h"
#include "SceneGameRoom_2D.h"

CSceneGameRoom_2D::CSceneGameRoom_2D(std::string name) : CGameState_2D(name)
{
}

CSceneGameRoom_2D::~CSceneGameRoom_2D()
{
	delete this;
}

void CSceneGameRoom_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	//m_width = nWndClientWidth;
	//m_height = nWndClientHeight;

	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;

	m_Background.Init("게임방 배경화면", m_width / 2, m_height / 2);
	m_Background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);

	m_btnRoomReady[0].Init("준비 버튼", (m_width / 2) + 300, (m_height / 2) + 250);
	m_btnRoomReady[0].OnCreate((m_width / 2) + 300, (m_height / 2) + 250, 200, 100, 10, 1, 1);

	m_btnRoomReady[1].Init("준비완료 버튼", (m_width / 2) + 300, (m_height / 2) + 250);
	m_btnRoomReady[1].OnCreate((m_width / 2) + 300, (m_height / 2) + 250, 200, 100, 10, 1, 1);

	m_btnRoomQuit.Init("방 나가기 버튼", (m_width / 2) + 50, (m_height / 2) + 250);
	m_btnRoomQuit.OnCreate((m_width / 2) + 50, (m_height / 2) + 250, 200, 100, 10, 1, 1);

	m_btnCharacterSelect_1.Init("캐릭터1 선택 버튼", (m_width / 2) - 180, (m_height / 2) - 50);
	m_btnCharacterSelect_1.OnCreate((m_width / 2) - 180, (m_height / 2) - 50, 200, 200, 10, 1, 1);

	m_btnCharacterSelect_2.Init("캐릭터2 선택 버튼", (m_width / 2) + 180, (m_height / 2) - 50);
	m_btnCharacterSelect_2.OnCreate((m_width / 2) + 180, (m_height / 2) - 50, 200, 200, 10, 1, 1);

	for (int i = 0; i < 4; ++i)
	{
		m_btnTeamLine[i].Init("팀 라인 선택 버튼", (m_width / 2), (m_height / 2));
		m_btnTeamLine[i].OnCreate((m_width / 2), (m_height / 2), 285, 125, 10, 1, 1);
	}


	m_bReady = FALSE;
}

void CSceneGameRoom_2D::exit()
{


}

void CSceneGameRoom_2D::pause()
{
}

void CSceneGameRoom_2D::resume()
{
}

//Render & Logic
void CSceneGameRoom_2D::OnDraw(HDC hdc)
{
	m_Background.OnDraw(hdc);

	if (!m_bReady)
		m_btnRoomReady[0].OnDraw(hdc);
	else
		m_btnRoomReady[1].OnDraw(hdc);

	m_btnRoomQuit.OnDraw(hdc);


	for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
	{
		
		//char -> TCHAR 변환
		TCHAR tchar_UserName[256] = { 0, };
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, TCP->GetRoomPlayerInfo(idx).UserID, strlen(TCP->GetRoomPlayerInfo(idx).UserID), tchar_UserName, 256);
		
		int pos_x = 0;
		int pos_y = 0;

		UINT tmp_Team = 0;

		switch (TCP->GetRoomPlayerInfo(idx).iSelectedTeam)
		{
		case PLAYER_TEAM_LINE::A_TEAM_0:
			pos_x = -600;
			pos_y = 0;
			m_blankcheck[A_TEAM_0] = FALSE;

			tmp_Team = 0;
			break;
		case PLAYER_TEAM_LINE::A_TEAM_1:
			pos_x = -600;
			pos_y = 150;
			m_blankcheck[A_TEAM_1] = FALSE;

			tmp_Team = 0;
			break;
		case PLAYER_TEAM_LINE::B_TEAM_0:
			pos_x = 350;
			pos_y = 0;
			m_blankcheck[B_TEAM_0] = FALSE;

			tmp_Team = 1;
			break;
		case PLAYER_TEAM_LINE::B_TEAM_1:
			pos_x = 350;
			pos_y = 150;
			m_blankcheck[B_TEAM_1] = FALSE;

			tmp_Team = 1;
			break;
		default:
			break;
		}

		if(idx == TCP->my_info.InRoom_MyNum)
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x, 200 + pos_y, Color_2D::ORANGE_2D, 40, L"돋움", tchar_UserName);
		else
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x, 200 + pos_y, Color_2D::WHITE_2D, 40, L"돋움", tchar_UserName);


		CSpriteObject_2D dpy_player_charater;

		if (tmp_Team == 0)
		{
			if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_MEI)
				dpy_player_charater.Init("A팀 캐릭터1 표시", (m_width / 2) + pos_x, 160 + pos_y);
			else if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_LIZZY)
				dpy_player_charater.Init("A팀 캐릭터2 표시", (m_width / 2) + pos_x, 160 + pos_y);
		}
		else
		{
			if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_MEI)
				dpy_player_charater.Init("B팀 캐릭터1 표시", (m_width / 2) + pos_x, 160 + pos_y);
			else if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_LIZZY)
				dpy_player_charater.Init("B팀 캐릭터2 표시", (m_width / 2) + pos_x, 160 + pos_y);
		}

		dpy_player_charater.OnCreate((m_width / 2) + pos_x, 150 + pos_y, 70, 70, 10, 1, 1);

		dpy_player_charater.OnDraw(hdc);
	}

	for (int idx2 = 0; idx2 < 4; ++idx2)
	{
		int pos_x = 0;
		int pos_y = 0;

		if (m_blankcheck[idx2] == TRUE)
		{
			switch (idx2)
			{
			case PLAYER_TEAM_LINE::A_TEAM_0:
				pos_x = -482;
				pos_y = 54;
				break;
			case PLAYER_TEAM_LINE::A_TEAM_1:
				pos_x = -482;
				pos_y = 204;
				break;
			case PLAYER_TEAM_LINE::B_TEAM_0:
				pos_x = 478;
				pos_y = 54;
				break;
			case PLAYER_TEAM_LINE::B_TEAM_1:
				pos_x = 478;
				pos_y = 204;
				break;
			default:
				break;
			}

			m_btnTeamLine[idx2].SetPosition((m_width / 2) + pos_x, 150 + pos_y);
			m_btnTeamLine[idx2].OnDraw(hdc);
		}
		else if (m_blankcheck[idx2] == FALSE)
		{
			//
			////
			m_btnTeamLine[idx2].SetPosition(-500, -500);
		}

		m_blankcheck[idx2] = TRUE;
	}

	//char -> TCHAR 변환
	TCHAR tchar_RoomName[256] = { 0, };
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, TCP->GetRoomInfo(TCP->my_info.InRoomNum).room_name, 
		strlen(TCP->GetRoomInfo(TCP->my_info.InRoomNum).room_name), tchar_RoomName, 256);

	Draw_2D::drawText(hdc, 80 , 480, Color_2D::WHITE_2D, 25, L"돋움", tchar_RoomName);

	if (TCP->GetRoomInfo(TCP->my_info.InRoomNum).max_player == GAME_PLAY_INDIVIDUAL)
		Draw_2D::drawText(hdc, 80, 530, Color_2D::WHITE_2D, 25, L"돋움", L"인원 : 1 X 1");
	else if (TCP->GetRoomInfo(TCP->my_info.InRoomNum).max_player == GAME_PLAY_TEAMWORK)
		Draw_2D::drawText(hdc, 80, 530, Color_2D::WHITE_2D, 25, L"돋움", L"인원 : 2 X 2");

	if (TCP->GetRoomInfo(TCP->my_info.InRoomNum).game_mode == GAME_PLAY_MODE_BASIC)
		Draw_2D::drawText(hdc, 80, 580, Color_2D::WHITE_2D, 25, L"돋움", L"모드 : 기본");
	else if (TCP->GetRoomInfo(TCP->my_info.InRoomNum).game_mode == GAME_PLAY_MODE_OCCUPY)
		Draw_2D::drawText(hdc, 80, 580, Color_2D::WHITE_2D, 25, L"돋움", L"모드 : 점령");
	else if (TCP->GetRoomInfo(TCP->my_info.InRoomNum).game_mode == GAME_PLAY_MODE_SCORE)
		Draw_2D::drawText(hdc, 80, 580, Color_2D::WHITE_2D, 25, L"돋움", L"모드 : 점수");

	//////////////////////////////

	m_btnCharacterSelect_1.OnDraw(hdc);
		
	m_btnCharacterSelect_2.OnDraw(hdc);

}

void CSceneGameRoom_2D::OnUpdate(float fTimeElapsed)
{
	//if (m_alpha == 200)
	//	SOUNDMGR->PlayEFFECT("로딩띠리링");
	//
	//if (m_alpha < 255)
	//	FRAMEWORK_2D->change_state(new CSceneSelectMenu_2D("SceneSelect"));
}

//Call
void CSceneGameRoom_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	switch (nMessageID)
	{
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		break;

	case WM_RBUTTONDOWN:
		m_bRbutton = true;
		break;

	case WM_RBUTTONUP:
		m_bRbutton = false;

		break;

	case WM_LBUTTONDOWN:
		m_bLbutton = true;


		break;

	case WM_LBUTTONUP:
		m_bLbutton = false;


		if (!m_bReady)
		{
			if (m_btnCharacterSelect_1.GetBoundingBox().Contains(mx, my))
			{
				P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
				TCP->my_info.iSelectedCharacter = CHARACTER_MEI;
				cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
				cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
				cs_room_send_my_info.WhatChange = PLAYER_CHANGE_CHARACTER;
				TCP->SendPacket((unsigned char*)&cs_room_send_my_info);
			}
			else if (m_btnCharacterSelect_2.GetBoundingBox().Contains(mx, my))
			{
				P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
				TCP->my_info.iSelectedCharacter = CHARACTER_LIZZY;
				cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
				cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
				cs_room_send_my_info.WhatChange = PLAYER_CHANGE_CHARACTER;
				TCP->SendPacket((unsigned char*)&cs_room_send_my_info);
			}

			for (int line_idx = 0; line_idx < 4; ++line_idx)
			{
				if (m_btnTeamLine[line_idx].GetBoundingBox().Contains(mx, my))
				{
					P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
					TCP->my_info.iSelectedTeam = line_idx;
					cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
					cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
					cs_room_send_my_info.WhatChange = PLAYER_CHANGE_TEAM_LINE;
					TCP->SendPacket((unsigned char*)&cs_room_send_my_info);
				}
			}
			if (m_btnRoomReady[0].GetBoundingBox().Contains(mx, my))
			{
				m_bReady = TRUE;

				P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
				cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
				cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
				cs_room_send_my_info.bReady = m_bReady;
				cs_room_send_my_info.WhatChange = PLAYER_READY;
				TCP->SendPacket((unsigned char*)&cs_room_send_my_info);

				////KYT '16.04.05
				///* 3D 로딩 임시임시*/
				//HINSTANCE hinst = nullptr;
				//HWND hwnd = nullptr;
				//
				//if (!FRAMEWORK->OnCreate(hinst, hwnd))
				//{
				//	::GMessageBox(L"3D", L"Fail");
				//	return;
				//}
				//
				//
				//
				//
				//
				//
				////게임 시작한다.
				//DimensionsChange(YT_GameMode::GAME_MODE_INGAME, 0);
			}
			if (m_btnRoomQuit.GetBoundingBox().Contains(mx, my))
			{
				P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
				cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
				cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
				cs_room_send_my_info.WhatChange = PLAYER_QUIT;
				TCP->SendPacket((unsigned char*)&cs_room_send_my_info);

				FRAMEWORK_2D->change_state(new CSceneLobby_2D("SceneLobby"));
			}

		}
		else
		{
			if (m_btnRoomReady[1].GetBoundingBox().Contains(mx, my))
			{
				m_bReady = FALSE;

				P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
				cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
				cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
				cs_room_send_my_info.bReady = m_bReady;
				cs_room_send_my_info.WhatChange = PLAYER_UN_READY;
				TCP->SendPacket((unsigned char*)&cs_room_send_my_info);

			}
		}
		

		break;
	}

}

void CSceneGameRoom_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{


}

void CSceneGameRoom_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneGameRoom_2D::CallMenu(WPARAM wParam)
{
}

