#include "stdafx.h"
#include "GameFrameWork_2D.h"

#include "SceneLoading_2D.h"
#include "SceneLogin_2D.h"
//#include "SceneSelectMenu_2D.h"

CSceneLoading_2D::CSceneLoading_2D(std::string name) : CGameState_2D(name)
{
}

CSceneLoading_2D::~CSceneLoading_2D()
{
	delete this;
}

void CSceneLoading_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{
	//m_width = nWndClientWidth;
	//m_height = nWndClientHeight;

	 
	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;



	m_logo.Init("로고");
	m_logo.OnCreate(m_width / 2, m_height / 2, m_width, m_height);

	m_loading_background.Init("로딩 배경화면");
	m_loading_background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);

	m_alpha = 255;


	
}

void CSceneLoading_2D::exit()
{


}

void CSceneLoading_2D::pause()
{
}

void CSceneLoading_2D::resume()
{
}   

//Render & Logic
void CSceneLoading_2D::OnDraw(HDC hdc)
{
	if (m_alpha == 255)std::cout << "앗 2D 로딩완료" << std::endl;

	if (m_alpha <= 254)
	{
		Draw_2D::drawText(hdc, L"", 800, 400, Color_2D::BLACK_2D, 100, L"a기본L");
		m_logo.OnDrawAlpha(hdc, m_alpha);
		m_alpha -= 2;
	}
	else
	{
		m_loading_background.OnDraw(hdc);

		//Draw_2D::drawRect(hdc, 0, 0, WIDTH, HEIGHT, Color_2D::WHITE_2D, Color_2D::BRIGHT_GREEN_1_2D, true);
		//ChoPa(hdc);
		//Draw_2D::drawText(hdc, L"잠시만", 800, 300, Color_2D::BRIGHT_YELLOW_3_2D, 100, L"a기본L");
		//Draw_2D::drawText(hdc, L"기다려주세요 ~ !", 750, 400, Color_2D::BRIGHT_YELLOW_3_2D, 100, L"a기본L");
		m_alpha--;
	}
}

void CSceneLoading_2D::OnUpdate(float fTimeElapsed)
{
	if (m_alpha == 200 || m_alpha == 199)
		SOUNDMGR->Play("로딩");

	if (m_alpha < 20)
		FRAMEWORK_2D->change_state(new CSceneLogin_2D("SceneLogin"));
}

//Call
void CSceneLoading_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (nMessageID == WM_MOUSEMOVE)
	{
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
	}

}

void CSceneLoading_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	
}

void CSceneLoading_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneLoading_2D::CallMenu(WPARAM wParam)
{
}
