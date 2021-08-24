#include "stdafx.h"
#include "GameFrameWork_2D.h"

#include "SceneGameRoom_2D.h"
#include "SceneRoomCreate_2D.h"
#include "SceneLobby_2D.h"

CSceneLobby_2D::CSceneLobby_2D(std::string name) : CGameState_2D(name)
{
}

CSceneLobby_2D::~CSceneLobby_2D()
{
	delete this;
}

void CSceneLobby_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	//m_width = nWndClientWidth;
	//m_height = nWndClientHeight;

	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;

	m_Background.Init("로비 배경화면", m_width / 2, m_height / 2);
	m_Background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);

	m_btnCreateRoom.Init("방 생성하기 버튼", (m_width / 2) + 470, (m_height / 2) + 130);
	m_btnCreateRoom.OnCreate((m_width / 2) + 470, (m_height / 2) + 130, 200, 100, 10, 1, 1);

	m_btnGameQuit.Init("게임 종료 버튼", (m_width / 2) + 470, (m_height / 2) + 250);
	m_btnGameQuit.OnCreate((m_width / 2) + 470, (m_height / 2) + 250, 200, 100, 10, 1, 1);

	

	m_Scr.initialize(Lu_Scroll::SCRType::SCR_V, m_iMaxValue, 650, 650, { 900, 10 }, 145, 20, 15);


}

void CSceneLobby_2D::exit()
{


}

void CSceneLobby_2D::pause()
{
}

void CSceneLobby_2D::resume()
{
}

//Render & Logic
void CSceneLobby_2D::OnDraw(HDC hdc)
{
	m_Background.OnDraw(hdc);
	m_btnCreateRoom.OnDraw(hdc);

	m_btnGameQuit.OnDraw(hdc);

	for (int idx = 0; idx < TCP->GetRoomInfoListSize(); ++idx)
	{
		//char -> TCHAR 변환
		TCHAR tchar_RoomName[256] = { 0, };
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, TCP->GetRoomInfo(idx).room_name, strlen(TCP->GetRoomInfo(idx).room_name), tchar_RoomName, 256);

		CSpriteObject_2D wnd;

		wnd.Init("방 리스트 표시 창", (m_width / 2) - 160, (100 + (idx * 100)) - m_nScrollPosition);
		wnd.OnCreate((m_width / 2) - 160, (100 + (idx * 100)) - m_nScrollPosition, 800, 100, 10, 1, 1);
		wnd.OnDraw(hdc);

		Draw_2D::drawText(hdc, (m_width / 2) - 500, (80 + (idx * 100) - m_nScrollPosition), Color_2D::WHITE_2D, 20, L"돋움", tchar_RoomName);


		if (TCP->GetRoomInfo(idx).game_mode == GAME_PLAY_MODE_BASIC)
			Draw_2D::drawText(hdc, (m_width / 2) - 150, (80 + (idx * 100) - m_nScrollPosition), Color_2D::WHITE_2D, 20, L"돋움", L"게임 모드 : 기본");
		else if (TCP->GetRoomInfo(idx).game_mode == GAME_PLAY_MODE_OCCUPY)
			Draw_2D::drawText(hdc, (m_width / 2) - 150, (80 + (idx * 100) - m_nScrollPosition), Color_2D::WHITE_2D, 20, L"돋움", L"게임 모드 : 점령");
		else if (TCP->GetRoomInfo(idx).game_mode == GAME_PLAY_MODE_SCORE)
			Draw_2D::drawText(hdc, (m_width / 2) - 150, (80 + (idx * 100) - m_nScrollPosition), Color_2D::WHITE_2D, 20, L"돋움", L"게임 모드 : 점수");


		if (TCP->GetRoomInfo(idx).max_player == GAME_PLAY_INDIVIDUAL)
			Draw_2D::drawText(hdc, (m_width / 2) + 50, (80 + (idx * 100) - m_nScrollPosition), Color_2D::WHITE_2D, 20, L"돋움", L"최대 인원 : 1 X 1");
		else if (TCP->GetRoomInfo(idx).max_player == GAME_PLAY_TEAMWORK)
			Draw_2D::drawText(hdc, (m_width / 2) + 50, (80 + (idx * 100) - m_nScrollPosition), Color_2D::WHITE_2D, 20, L"돋움", L"최대 인원 : 2 X 2");
	}

	m_Scr.Draw(hdc, RECT{ 0, 0, WIDTH, HEIGHT }, RGB((int)m_Scr.GetNowSrcPosition(), 0, 0));

	//char  -> TCHAR 변환
	TCHAR tchar_MyID[256] = { 0, };
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, TCP->my_info.UserID, strlen(TCP->my_info.UserID), tchar_MyID, 256);


	Draw_2D::drawText(hdc, 1020, 50, Color_2D::WHITE_2D, 35, L"돋움", tchar_MyID);




}

void CSceneLobby_2D::OnUpdate(float fTimeElapsed)
{
	m_nScrollPosition = m_Scr.GetNowSrcPosition() * 4;
	//if (m_alpha == 200)
	//	SOUNDMGR->PlayEFFECT("로딩띠리링");
	//
	//if (m_alpha < 255)
	//	FRAMEWORK_2D->change_state(new CSceneSelectMenu_2D("SceneSelect"));
}

//Call
void CSceneLobby_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (m_bLbutton == true)
			m_Scr.OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

		break;

	case WM_RBUTTONDOWN:
		m_bRbutton = true;
		break;

	case WM_RBUTTONUP:
		m_bRbutton = false;

		break;

	case WM_LBUTTONDOWN:
		m_bLbutton = true;

		m_Scr.OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

		break;

	case WM_LBUTTONUP:
		m_bLbutton = false;



		for (int idx = 0; idx < TCP->GetRoomInfoListSize(); ++idx)
		{
			CSpriteObject_2D wnd;
			wnd.OnCreate((m_width / 2) - 160, (100 + (idx * 100)) - m_nScrollPosition, 800, 100, 10, 1, 1);

			if (wnd.GetBoundingBox().Contains(mx, my))
			{
				P_CS_ROOM_IN cs_room_in;
				cs_room_in.InRoomNum = idx;
				TCP->my_info.InRoomNum = idx;

				cs_room_in.size = sizeof(P_CS_ROOM_IN);
				cs_room_in.type = CS_ROOM_IN;
				TCP->SendPacket((unsigned char*)&cs_room_in);

				WaitForSingleObject(TCP->EV.hGetUserInfoEvent, INFINITE); // 전체 방의 유저 정보 얻어오기 완료 기다리기
				

				if (TCP->my_info.bRoomEntranceSuccess == TRUE)
				{
					P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
					cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
					cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;
					cs_room_send_my_info.WhatChange = PLAYER_ENTRANCE;

					TCP->SendPacket((unsigned char*)&cs_room_send_my_info);

					FRAMEWORK_2D->change_state(new CSceneGameRoom_2D("SceneGameRoom"));
			
				}

				ResetEvent(TCP->EV.hGetUserInfoEvent);
				break;
			}
		}


		if (m_btnCreateRoom.GetBoundingBox().Contains(mx, my))
		{
			FRAMEWORK_2D->change_state(new CSceneRoomCreate_2D("SceneRoomCreate"));
		}

		if (m_btnGameQuit.GetBoundingBox().Contains(mx, my))
		{

			#ifdef _CONNECT_SERVER_

						// 이벤트 제거
						TCP->EV.DeleteEvent();

						TCP->RoomListClear();
						TCP->RoomPlayerListClear();

						// closesocket()
						closesocket(TCP->sock);

						// 윈속 종료
						WSACleanup();

			#endif
			PostQuitMessage(0);
		}

		break;
	}

}

void CSceneLobby_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (nMessageID == WM_KEYDOWN)
	{
		if (wParam == 's' - 32)
		{

		#ifndef _CONNECT_SERVER_

		#endif

		}
	}

}

void CSceneLobby_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneLobby_2D::CallMenu(WPARAM wParam)
{
}

