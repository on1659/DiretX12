#include "stdafx.h"
#include "GameFrameWork_2D.h"
#include "GameFramework.h"

#include "SceneLobby_2D.h"
#include "SceneResult_2D.h"

CSceneResult_2D::CSceneResult_2D(std::string name) : CGameState_2D(name)
{
}

CSceneResult_2D::~CSceneResult_2D()
{
	delete this;
}

void CSceneResult_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	//m_width = nWndClientWidth;
	//m_height Å³
	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;

	m_Background.Init("°á°ú ¹è°æÈ­¸é", m_width / 2, m_height / 2);
	m_Background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);

	m_btnGoToLobby.Init("·Îºñ·Î ³ª°¡±â ¹öÆ°", (m_width / 2) + 380, (m_height / 2) + 230);
	m_btnGoToLobby.OnCreate((m_width / 2) + 380, (m_height / 2) + 230, 200, 100, 10, 1, 1);

	m_my_character1.Init("°á°ú È­¸é Ä³¸¯ÅÍ1 Ç¥½Ã", (m_width / 2) + 370, (m_height / 2) - 50);
	m_my_character1.OnCreate((m_width / 2) + 370, (m_height / 2) - 50, 300, 480, 10, 1, 1);

	m_my_character2.Init("°á°ú È­¸é Ä³¸¯ÅÍ2 Ç¥½Ã", (m_width / 2) + 370, (m_height / 2) - 50);
	m_my_character2.OnCreate((m_width / 2) + 370, (m_height / 2) - 50, 300, 480, 10, 1, 1);



	//½ÂÆÐ °è»ê
	for (int i = 0; i < TCP->GetRoomPlayerInfoListSize(); ++i)
	{
		if (TCP->my_info.iSelectedTeam == A_TEAM_0 || TCP->my_info.iSelectedTeam == A_TEAM_1)
		{
			if (TCP->GetRoomPlayerInfo(i).iSelectedTeam == A_TEAM_0 || TCP->GetRoomPlayerInfo(i).iSelectedTeam == A_TEAM_1){
				my_team_score += 1;
			}
			else{
				other_team_score += 1;
			}
		}
		else
		{
			if (TCP->GetRoomPlayerInfo(i).iSelectedTeam == B_TEAM_0 || TCP->GetRoomPlayerInfo(i).iSelectedTeam == B_TEAM_1){
				my_team_score += 1;
			}
			else{
				other_team_score += 1;
			}
		}
	}

	if (my_team_score == other_team_score)
		result = 0;
	else if (my_team_score > other_team_score)
		result = 1;
	else if (my_team_score < other_team_score)
		result = 2;
}

void CSceneResult_2D::exit()
{


}

void CSceneResult_2D::pause()
{
}

void CSceneResult_2D::resume()
{
}

//Render & Logic
void CSceneResult_2D::OnDraw(HDC hdc)
{
	m_Background.OnDraw(hdc);
	m_btnGoToLobby.OnDraw(hdc);

	if(TCP->my_info.iSelectedCharacter == CHARACTER_MEI)
		m_my_character1.OnDraw(hdc);
	else if(TCP->my_info.iSelectedCharacter == CHARACTER_LIZZY)
		m_my_character2.OnDraw(hdc);

	for (int idx = 0; idx < TCP->GetRoomPlayerInfoListSize(); ++idx)
	{
		int pos_x = 0;
		int pos_y = 0;

		UINT tmp_Team = 0;

		switch (TCP->GetRoomPlayerInfo(idx).iSelectedTeam)
		{
		case PLAYER_TEAM_LINE::A_TEAM_0:
			pos_x = -480;
			pos_y = 70;

			tmp_Team = 0;
			break;
		case PLAYER_TEAM_LINE::A_TEAM_1:
			pos_x = -480;
			pos_y = 170;

			tmp_Team = 0;
			break;
		case PLAYER_TEAM_LINE::B_TEAM_0:
			pos_x = -480;
			pos_y = 270;

			tmp_Team = 1;
			break;
		case PLAYER_TEAM_LINE::B_TEAM_1:
			pos_x = -480;
			pos_y = 370;

			tmp_Team = 1;
			break;
		default:
			break;
		}


		CSpriteObject_2D dpy_player_charater;

		if (tmp_Team == 0)
		{
			if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_MEI)
				dpy_player_charater.Init("AÆÀ Ä³¸¯ÅÍ1 Ç¥½Ã", (m_width / 2) + pos_x, 160 + pos_y);
			else if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_LIZZY)
				dpy_player_charater.Init("AÆÀ Ä³¸¯ÅÍ2 Ç¥½Ã", (m_width / 2) + pos_x, 160 + pos_y);
		}
		else
		{
			if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_MEI)
				dpy_player_charater.Init("BÆÀ Ä³¸¯ÅÍ1 Ç¥½Ã", (m_width / 2) + pos_x, 160 + pos_y);
			else if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_LIZZY)
				dpy_player_charater.Init("BÆÀ Ä³¸¯ÅÍ2 Ç¥½Ã", (m_width / 2) + pos_x, 160 + pos_y);
		}

		dpy_player_charater.OnCreate((m_width / 2) + pos_x, 150 + pos_y, 70, 70, 10, 1, 1);

		dpy_player_charater.OnDraw(hdc);


		TCHAR tchar_UserName[256] = { 0, };
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, TCP->GetRoomPlayerInfo(idx).UserID, strlen(TCP->GetRoomPlayerInfo(idx).UserID), tchar_UserName, 256);

		if(TCP->my_info.iSelectedTeam == TCP->GetRoomPlayerInfo(idx).iSelectedTeam)
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x - 70, ((m_height / 2) + pos_y) - 150, Color_2D::ORANGE_2D, 20, L"a¿¾³¯¸ñ¿åÅÁL", tchar_UserName);
		else
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x - 70, ((m_height / 2) + pos_y) - 150, Color_2D::WHITE_2D, 20, L"a¿¾³¯¸ñ¿åÅÁL", tchar_UserName);


		Draw_2D::drawText(hdc, (m_width / 2) + pos_x + 100, 130 + pos_y, Color_2D::WHITE_2D, 30, L"a¿¾³¯¸ñ¿åÅÁL", L"%d", TCP->GetRoomPlayerInfo(idx).iKillNum);

		Draw_2D::drawText(hdc, (m_width / 2) + pos_x + 200, 130 + pos_y, Color_2D::WHITE_2D, 30, L"a¿¾³¯¸ñ¿åÅÁL", L"%d", TCP->GetRoomPlayerInfo(idx).iDeathNum);

		Draw_2D::drawText(hdc, (m_width / 2) + pos_x + 340, 130 + pos_y, Color_2D::WHITE_2D, 30, L"a¿¾³¯¸ñ¿åÅÁL", L"%d", TCP->GetRoomPlayerInfo(idx).iMonsterKillNum);

		Draw_2D::drawText(hdc, (m_width / 2) + pos_x + 470, 130 + pos_y, Color_2D::WHITE_2D, 30, L"a¿¾³¯¸ñ¿åÅÁL", L"%d", TCP->GetRoomPlayerInfo(idx).iScoreNum);

	}


	if (result == 0)
		Draw_2D::drawText(hdc, (m_width / 2) + 270, (m_height / 2) - 300, Color_2D::YELLOW_2D, 70, L"a¿¾³¯¸ñ¿åÅÁL", L"DRAW");
	else if (result == 1)
		Draw_2D::drawText(hdc, (m_width / 2) + 270, (m_height / 2) - 300, Color_2D::BLUE_2D, 70, L"a¿¾³¯¸ñ¿åÅÁL" L"WIN");
	else if (result == 2)
		Draw_2D::drawText(hdc, (m_width / 2) + 270, (m_height / 2) - 300, Color_2D::RED_2D, 70, L"a¿¾³¯¸ñ¿åÅÁL", L"LOSE");

}

void CSceneResult_2D::OnUpdate(float fTimeElapsed)
{
	
	if (GetAsyncKeyState(VK_C))
	{
		auto device = FRAMEWORK->GetDevice();
		LoadingThreadLoad(device);
		PrepareManagerLoading(device);
		FinalManagerLoading(device);


	}
}

//Call
void CSceneResult_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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


		if (m_btnGoToLobby.GetBoundingBox().Contains(mx, my))
		{
			//TCP->my_info.packet_manager.cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
			//TCP->my_info.packet_manager.cs_room_send_my_info.WhatChange = PLAYER_QUIT;
			//TCP->SendPacket((char*)&TCP->my_info.packet_manager.cs_room_send_my_info);

			FRAMEWORK_2D->change_state(new CSceneLobby_2D("SceneLobby"));
		}


		break;
	}

}

void CSceneResult_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{


}

void CSceneResult_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneResult_2D::CallMenu(WPARAM wParam)
{
}
