#include "stdafx.h"
#include "GameFrameWork_2D.h"

#include "SceneMatchStandby_2D.h"



SceneMatchStandby_2D::SceneMatchStandby_2D(std::string name) : CGameState_2D(name)
{
}

SceneMatchStandby_2D::~SceneMatchStandby_2D()
{
	delete this;
}

void SceneMatchStandby_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	//m_width = nWndClientWidth;
	//m_height = nWndClientHeight;

	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;

	m_Background.Init("매치 대기 배경화면", m_width / 2, m_height / 2);
	m_Background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);

	m_dpyLoading.Init("로딩 표시", (m_width / 2) + 500, (m_height / 2) - 270);
	m_dpyLoading.OnCreate((m_width / 2) + 500, (m_height / 2) - 270, 80, 80, 10, 11, 1);


}

void SceneMatchStandby_2D::exit()
{


}

void SceneMatchStandby_2D::pause()
{
}

void SceneMatchStandby_2D::resume()
{
}

//Render & Logic
void SceneMatchStandby_2D::OnDraw(HDC hdc)
{
	m_Background.OnDraw(hdc);

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
			pos_x = -500;
			pos_y = 150;

			tmp_Team = 0;
			break;
		case PLAYER_TEAM_LINE::A_TEAM_1:
			pos_x = -300;
			pos_y = 150;

			tmp_Team = 0;
			break;
		case PLAYER_TEAM_LINE::B_TEAM_0:
			pos_x = 300;
			pos_y = 150;

			tmp_Team = 1;
			break;
		case PLAYER_TEAM_LINE::B_TEAM_1:
			pos_x = 500;
			pos_y = 150;

			tmp_Team = 1;
			break;
		default:
			break;
		}


		CSpriteObject_2D dpy_player_charater_card;

		if (tmp_Team == 0)
		{
			if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_MEI)
				dpy_player_charater_card.Init("매치 대기 A팀 캐릭터1 표시", (m_width / 2) + pos_x, 160 + pos_y);
			else if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_LIZZY)
				dpy_player_charater_card.Init("매치 대기 A팀 캐릭터2 표시", (m_width / 2) + pos_x, 160 + pos_y);
		}
		else
		{
			if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_MEI)
				dpy_player_charater_card.Init("매치 대기 B팀 캐릭터1 표시", (m_width / 2) + pos_x, 160 + pos_y);
			else if (TCP->GetRoomPlayerInfo(idx).iSelectedCharacter == CHARACTER_LIZZY)
				dpy_player_charater_card.Init("매치 대기 B팀 캐릭터2 표시", (m_width / 2) + pos_x, 160 + pos_y);
		}

		dpy_player_charater_card.OnCreate((m_width / 2) + pos_x, 150 + pos_y, 200, 400, 10, 1, 1);

		dpy_player_charater_card.OnDraw(hdc);

		if (idx == TCP->my_info.InRoom_MyNum)
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x - 60, 210 + pos_y, Color_2D::ORANGE_2D, 20, L"돋움", tchar_UserName);
		else
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x - 60, 210 + pos_y, Color_2D::WHITE_2D, 20, L"돋움", tchar_UserName);

		if (TCP->GetRoomPlayerInfo(idx).bLoadingCompletion == TRUE)
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x - 40, 260 + pos_y, Color_2D::WHITE_2D, 20, L"돋움", L"로딩완료");
		else
			Draw_2D::drawText(hdc, (m_width / 2) + pos_x - 40, 260 + pos_y, Color_2D::WHITE_2D, 20, L"돋움", L"로딩중");
	}

	m_dpyLoading.OnDraw(hdc);


}

void SceneMatchStandby_2D::OnUpdate(float fTimeElapsed)
{
	m_dpyLoading.OnSpriteUpdate();


	//if (m_alpha == 200)
	//	SOUNDMGR->PlayEFFECT("로딩띠리링");

	//if (m_alpha < 255)
	//	FRAMEWORK_2D->change_state(new CSceneLogin_2D("SceneLogin"));
}

//Call
void SceneMatchStandby_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

}

void SceneMatchStandby_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{


}

void SceneMatchStandby_2D::SetWindowsSizeChange(int width, int height)
{
}

void SceneMatchStandby_2D::CallMenu(WPARAM wParam)
{
}

