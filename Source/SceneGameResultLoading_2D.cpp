#include "stdafx.h"
#include "GameFrameWork_2D.h"

#include "SceneResult_2D.h"
#include "SceneGameResultLoading_2D.h"

//#include "SceneSelectMenu_2D.h"

CSceneGameResultLoading_2D::CSceneGameResultLoading_2D(std::string name) : CGameState_2D(name)
{
}

CSceneGameResultLoading_2D::~CSceneGameResultLoading_2D()
{
	delete this;
}

void CSceneGameResultLoading_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	//m_width = nWndClientWidth;
	//m_height = nWndClientHeight;

	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;

	m_match_end_loading_background.Init("결과화면 로딩 배경화면");
	m_match_end_loading_background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);


	m_alpha = 0;

}

void CSceneGameResultLoading_2D::exit()
{


}

void CSceneGameResultLoading_2D::pause()
{
}

void CSceneGameResultLoading_2D::resume()
{
}

//Render & Logic
void CSceneGameResultLoading_2D::OnDraw(HDC hdc)
{
	if (m_alpha == 255)
		std::cout << "앗 2D 로딩완료" << std::endl;

	if (m_alpha <= 254)
	{
		Draw_2D::drawText(hdc, L"", 800, 400, Color_2D::BLACK_2D, 100, L"a기본L");
		m_match_end_loading_background.OnDrawAlpha(hdc, m_alpha);
		m_alpha += 2;
	}
	else
	{
		m_alpha++;
	}
}

void CSceneGameResultLoading_2D::OnUpdate(float fTimeElapsed)
{
	//if (m_alpha == 200)
	//	SOUNDMGR->PlayEFFECT("로딩띠리링");

	if (m_alpha > 200)
		FRAMEWORK_2D->change_state(new CSceneResult_2D("SceneResult"));
}

//Call
void CSceneGameResultLoading_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (nMessageID == WM_MOUSEMOVE)
	{
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
	}

}

void CSceneGameResultLoading_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{


}

void CSceneGameResultLoading_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneGameResultLoading_2D::CallMenu(WPARAM wParam)
{
}
