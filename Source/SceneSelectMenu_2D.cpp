#include "stdafx.h"
#include "SceneSelectMenu_2D.h"
#include "Draw_2D.h"

#include "GameFrameWork.h"
#include "SceneLoading_2D.h"
#include "GameFrameWork_2D.h"

#ifdef _MULIT_THREAD_LOADING_
	#include "ThreadProcessor.h"
#endif

CSceneSelectMenu_2D::CSceneSelectMenu_2D(std::string name) : CGameState_2D(name)
{
}


CSceneSelectMenu_2D::~CSceneSelectMenu_2D()
{
}

void CSceneSelectMenu_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	m_width = nWndClientWidth;
	m_height = nWndClientHeight;


	//상태
	m_bReady = false;


	//이미지
	m_Background.Init("메뉴배경화면", m_width / 2, m_height / 2);

	m_Button[0].Init("시작버튼");
	m_Button[0].OnCreate(m_width / 2, m_height / 2, 200, 100, 10, 1, 2);

	m_Button[1].Init("종료버튼");
	m_Button[1].OnCreate(m_width / 2, m_height / 2 + 150, 200, 100, 10, 1, 2);

	m_bMouseSound = 99;

	m_monster.Init("캐릭터");
	m_monster.OnCreate(100, 100, 48, 72, 10, 4, 6);

	m_player.Init("상대캐릭터");
	m_player.OnCreate(200, 200, 48, 72, 10, 4, 6);

	m_Scr.initialize(Lu_Scroll::SCRType::SCR_H, m_iMaxValue, 200, 200, { 220, 50 }, 145, 10, 7.5);

	//사운드
	//04_SoundMGR->PlayBGM("메뉴화면");
}

void CSceneSelectMenu_2D::exit()
{

}

void CSceneSelectMenu_2D::pause()
{
}

void CSceneSelectMenu_2D::resume()
{

}

//Render & Logic
void CSceneSelectMenu_2D::OnDraw(HDC hdc)
{
	m_Background.OnDraw(hdc);

	m_Scr.Draw(hdc, RECT{ 0, 0, WIDTH, HEIGHT }, RGB((int)m_Scr.GetNowSrcPosition(), 0, 0));
	Draw_2D::drawProgrees(hdc, m_persent, 100, 300, 150, 15, BRIGHT_RED_1, BRIGHT_BLUE_1);

	if (m_bReady)
	{
		Draw_2D::drawText(hdc, m_width / 2 - 100, 100 + m_nScrollPosition, BLACK, L"준비 상태");
	}
	else
		Draw_2D::drawText(hdc, m_width / 2 - 100, 100 + m_nScrollPosition, BLACK, L"대기 상태");

	m_monster.OnDraw(hdc);

	m_player.OnDraw(hdc);

	//버튼 그림
	for (int i = 0; i < 2; ++i) m_Button[i].OnDraw(hdc);


	int i = 5;
	Draw_2D::drawText(hdc, 50, 50, BLACK, L"안녕? %d 원입니다.", i);



}


void CSceneSelectMenu_2D::OnUpdate(float fTimeElapsed)
{
	m_nScrollPosition = m_Scr.GetNowSrcPosition();
	if (m_persent > 100)m_persent = 0;
	m_persent++;

	for (int i = 0; i < 2; ++i)
	{
		m_Button[i].OnSpriteUpdate();

		m_Button[i].SetSpriteHeight(0);

		if (m_Button[i].GetBoundingBox().Contains(mx, my))
		{
			m_Button[i].SetSpriteHeight(1);

			//if (m_bMouseSound != i)
			//	SOUNDMGR->PlayEFFECT("마우스드래그");

			m_bMouseSound = i;
		}
	}


	if (m_bRbutton)
	{
		m_player.SetPosition(20, 20);
		m_player.SetSpriteHeight(3);
	}
	else
		m_player.SetSpriteHeight(1);
}

//Call
void CSceneSelectMenu_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	m_Scr.OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

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
		if (m_Button[0].GetBoundingBox().Contains(mx, my))
		{
		
			//KYT '16.04.12
			/*
			  Loading Thread
			*/
			#ifdef _MULIT_THREAD_LOADING_
				std::cout << " Join " << std::endl;
				loadingThread->join();
				#ifdef _MULIT_THREAD_RENDER_
					FRAMEWORK->InitializeWorkerThreads();
				#endif	
			#endif

			CONTENTMGR->FinalLoad(FRAMEWORK->GetDevice());
			SPACE->FinalLoad();
			UIMGR->FinalLoad();

			::DimensionsChange(GAME_MODE_INGAME);
		}

		if (m_Button[1].GetBoundingBox().Contains(mx, my))
		{
			::PostQuitMessage(0);
			//04_SoundMGR->PlayEFFECT("마우스클릭");
		}

		break;
	}
}

void CSceneSelectMenu_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:

		break;
	case WM_KEYUP:
		if (wParam == VK_F5)
		{
			(m_bReady) ? m_bReady = false : m_bReady = true;
			//04_SoundMGR->PlayEFFECT("마우스클릭");
		}
		if (wParam == VK_RETURN)
		{
			FRAMEWORK_2D->change_state(new CSceneLoading_2D("Loading"));
		}
		break;
	}
}

void CSceneSelectMenu_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneSelectMenu_2D::CallMenu(WPARAM wParam)
{
}

