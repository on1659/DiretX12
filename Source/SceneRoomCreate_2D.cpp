#include "stdafx.h"
#include "GameFrameWork_2D.h"

#include "SceneLobby_2D.h"
#include "SceneRoomCreate_2D.h"
#include "SceneGameRoom_2D.h"

CSceneRoomCreate_2D::CSceneRoomCreate_2D(std::string name) : CGameState_2D(name)
{
}

CSceneRoomCreate_2D::~CSceneRoomCreate_2D()
{
	delete this;
}

void CSceneRoomCreate_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	//m_width = nWndClientWidth;
	//m_height = nWndClientHeight;

	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;

	m_Background.Init("방 생성 배경화면", m_width / 2, m_height / 2);
	m_Background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);



	m_btnBasicMode[0].Init("기본모드 버튼", (m_width / 2) + 370, (m_height / 2) - 40);
	m_btnBasicMode[0].OnCreate((m_width / 2) + 370, (m_height / 2) - 40, 150, 80, 10, 1, 1);

	m_btnOccupyMode[0].Init("점령모드 버튼", (m_width / 2) + 370, (m_height / 2) + 40);
	m_btnOccupyMode[0].OnCreate((m_width / 2) + 370, (m_height / 2) + 40, 150, 80, 10, 1, 1);

	m_btnScoreMode[0].Init("점수모드 버튼", (m_width / 2) + 370, (m_height / 2) + 120);
	m_btnScoreMode[0].OnCreate((m_width / 2) + 370, (m_height / 2) + 120, 150, 80, 10, 1, 1);

	m_btn1x1Mode[0].Init("1x1모드 버튼", (m_width / 2) + 80, (m_height / 2) - 40);
	m_btn1x1Mode[0].OnCreate((m_width / 2) + 80, (m_height / 2) - 40, 150, 80, 10, 1, 1);

	m_btn2x2Mode[0].Init("2x2모드 버튼", (m_width / 2) + 80, (m_height / 2) + 40);
	m_btn2x2Mode[0].OnCreate((m_width / 2) + 80, (m_height / 2) + 40, 150, 80, 10, 1, 1);

	////////////////////////////////////////////////////////////////////////////////////////

	m_btnBasicMode[1].Init("기본모드 선택 버튼", (m_width / 2) + 370, (m_height / 2) - 40);
	m_btnBasicMode[1].OnCreate((m_width / 2) + 370, (m_height / 2) - 40, 150, 80, 10, 1, 1);

	m_btnOccupyMode[1].Init("점령모드 선택 버튼", (m_width / 2) + 370, (m_height / 2) + 40);
	m_btnOccupyMode[1].OnCreate((m_width / 2) + 370, (m_height / 2) + 40, 150, 80, 10, 1, 1);

	m_btnScoreMode[1].Init("점수모드 선택 버튼", (m_width / 2) + 370, (m_height / 2) + 120);
	m_btnScoreMode[1].OnCreate((m_width / 2) + 370, (m_height / 2) + 120, 150, 80, 10, 1, 1);

	m_btn1x1Mode[1].Init("1x1모드 선택 버튼", (m_width / 2) + 80, (m_height / 2) - 40);
	m_btn1x1Mode[1].OnCreate((m_width / 2) + 80, (m_height / 2) - 40, 150, 80, 10, 1, 1);

	m_btn2x2Mode[1].Init("2x2모드 선택 버튼", (m_width / 2) + 80, (m_height / 2) + 40);
	m_btn2x2Mode[1].OnCreate((m_width / 2) + 80, (m_height / 2) + 40, 150, 80, 10, 1, 1);



	m_btnCreateRoomQuit.Init("방 생성 나가기 버튼", (m_width / 2) + 470, (m_height / 2) - 260);
	m_btnCreateRoomQuit.OnCreate((m_width / 2) + 470, (m_height / 2) -260, 60, 60, 10, 1, 1);


	m_btnCreateRoomConfirm.Init("방 생성하기 확인 버튼", (m_width / 2) + 20, (m_height / 2) + 270);
	m_btnCreateRoomConfirm.OnCreate((m_width / 2) + 20, (m_height / 2) + 270, 200, 90, 10, 1, 1);

	m_iGameMode = GAME_PLAY_MODE_BASIC;
	m_iGameTeamSize = GAME_PLAY_INDIVIDUAL;
}

void CSceneRoomCreate_2D::exit()
{


}

void CSceneRoomCreate_2D::pause()
{
}

void CSceneRoomCreate_2D::resume()
{
}

//Render & Logic
void CSceneRoomCreate_2D::OnDraw(HDC hdc)
{
	m_Background.OnDraw(hdc);

	if (m_iGameMode == GAME_PLAY_MODE_BASIC)
		m_btnBasicMode[1].OnDraw(hdc);
	else
		m_btnBasicMode[0].OnDraw(hdc);

	if (m_iGameMode == GAME_PLAY_MODE_OCCUPY)
		m_btnOccupyMode[1].OnDraw(hdc);
	else
		m_btnOccupyMode[0].OnDraw(hdc);

	if (m_iGameMode == GAME_PLAY_MODE_SCORE)
		m_btnScoreMode[1].OnDraw(hdc);
	else
		m_btnScoreMode[0].OnDraw(hdc);


	if (m_iGameTeamSize == GAME_PLAY_INDIVIDUAL)
		m_btn1x1Mode[1].OnDraw(hdc);
	else
		m_btn1x1Mode[0].OnDraw(hdc);

	if (m_iGameTeamSize == GAME_PLAY_TEAMWORK)
		m_btn2x2Mode[1].OnDraw(hdc);
	else
		m_btn2x2Mode[0].OnDraw(hdc);



	m_btnCreateRoomQuit.OnDraw(hdc);
	m_btnCreateRoomConfirm.OnDraw(hdc);
}

void CSceneRoomCreate_2D::OnUpdate(float fTimeElapsed)
{
	//if (m_alpha == 200)
	//	SOUNDMGR->PlayEFFECT("로딩띠리링");
	//
	//if (m_alpha < 255)
	//	FRAMEWORK_2D->change_state(new CSceneSelectMenu_2D("SceneSelect"));
}

//Call
void CSceneRoomCreate_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

		if (m_btnBasicMode[0].GetBoundingBox().Contains(mx, my))
		{
			m_iGameMode = GAME_PLAY_MODE_BASIC;
		}
		else if (m_btnOccupyMode[0].GetBoundingBox().Contains(mx, my))
		{
			m_iGameMode = GAME_PLAY_MODE_OCCUPY;
		}
		else if (m_btnScoreMode[0].GetBoundingBox().Contains(mx, my))
		{
			m_iGameMode = GAME_PLAY_MODE_SCORE;
		}


		if (m_btn1x1Mode[0].GetBoundingBox().Contains(mx, my))
		{
			m_iGameTeamSize = GAME_PLAY_INDIVIDUAL;
		}
		else if (m_btn2x2Mode[0].GetBoundingBox().Contains(mx, my))
		{
			m_iGameTeamSize = GAME_PLAY_TEAMWORK;
		}

		////////////////////////////////////////////////////////////

		if (m_btnCreateRoomQuit.GetBoundingBox().Contains(mx, my))
		{
			FRAMEWORK_2D->change_state(new CSceneLobby_2D("SceneLobby"));
		}

		if (m_btnCreateRoomConfirm.GetBoundingBox().Contains(mx, my))
		{
			strcpy(m_SetRoomName, "[방제] ");

			char tmp_name[7];

			for (int i = 0; i < 7; ++i)
			{
				tmp_name[i] = 65 + rand() % 20;
			}

			strcat(m_SetRoomName, tmp_name);

			m_SetRoomName[14] = '\0';

			P_CS_LOBBY_ROOM_CREATE cs_lobby_room_create;
			strcpy(cs_lobby_room_create.room_info.room_name, m_SetRoomName);
			cs_lobby_room_create.room_info.max_player = m_iGameTeamSize;
			cs_lobby_room_create.room_info.game_mode = m_iGameMode;

			cs_lobby_room_create.size = sizeof(P_CS_LOBBY_ROOM_CREATE);
			cs_lobby_room_create.type = CS_LOBBY_ROOM_CREATE;
			TCP->SendPacket((unsigned char*)&cs_lobby_room_create);

			WaitForSingleObject(TCP->EV.hGetUserInfoEvent, INFINITE); // 전체 방의 유저 정보 얻어오기 완료 기다리기

			P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
			cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
			cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
			cs_room_send_my_info.WhatChange = PLAYER_ENTRANCE;

			TCP->SendPacket((unsigned char*)&cs_room_send_my_info);


			FRAMEWORK_2D->change_state(new CSceneGameRoom_2D("SceneGameRoom"));
		}


		break;
	}

}

void CSceneRoomCreate_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSceneRoomCreate_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneRoomCreate_2D::CallMenu(WPARAM wParam)
{
}

