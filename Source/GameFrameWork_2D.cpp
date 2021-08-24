
#include "stdafx.h"
#include "GameFrameWork_2D.h"
#include "SceneLoading_2D.h"
#include "Timer.h"
#include <time.h>

//#define _CONSOL_

CGameFrameWork_2D::CGameFrameWork_2D()
{

	m_hdc				 = nullptr;
	m_hBitmapFrameBuffer = nullptr;
	m_hBitmapSelect		 = nullptr;
	m_hInstance			 = nullptr;
	m_hWnd				 = nullptr;
	m_bBackgroundColor   = YT_Color(0,0,0,0);

}

CGameFrameWork_2D::~CGameFrameWork_2D()
{
	while (!states.empty()) {
		states.back()->exit();
		states.pop_back();
	}
}


//State

void CGameFrameWork_2D::enter(CGameState_2D *state)
{
	srand((unsigned int)time(nullptr));

	states.push_back(state);
	states.back()->enter(m_hWnd, m_hInstance, m_nWndClientWidth, m_nWndClientHeight);
}

void CGameFrameWork_2D::change_state(CGameState_2D *state)
{
	//KYT '16.05.10
	/*
	 ����ȯ�� �ڵ����� ����Ʈ �ڽ� ������ ����
	*/
	EDITBOX_2D->Clear();

	if (!states.empty())
	{
		states.back()->exit();
		states.pop_back();
	}

	states.push_back(state);

	states.back()->enter(m_hWnd, m_hInstance, m_nWndClientWidth, m_nWndClientHeight);

}

void CGameFrameWork_2D::exit()
{
	
	RENDERMGR_2D->ReleseInstance();
	//04_SoundMGR->ReleseInstance();
	//TIMEMGR->ReleseInstance();

#ifdef _CONSOL_
	FreeConsole(); // Free Console Window
#endif 
}

void CGameFrameWork_2D::pushState(CGameState_2D* state)
{
	// pause current state
	if (!states.empty()) {
		states.back()->pause();
	}
	// store and init the new state
	states.push_back(state);
	states.back()->enter(m_hWnd, m_hInstance, m_nWndClientWidth, m_nWndClientHeight);

}

void CGameFrameWork_2D::popState()
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->exit();
		states.pop_back();
	}
	// resume previous state
	if (!states.empty()) {
		states.back()->resume();
	}
}



//Render & Logic & Init

void CGameFrameWork_2D::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hWnd = hMainWnd;
	m_hInstance = hInstance;

	srand((unsigned int)time(nullptr));


	//���� ����
	TIMEMGR->Tick();						//Ÿ�̸�
	RENDERMGR_2D->LoadCImage();				//�����Ŵ���
	//04_SoundMGR->LoadSound();					//����Ŵ���
	SOUNDMGR->LoadSound();
	EDITBOX_2D->Load(hMainWnd);				//EditBox;

	m_nWndClientWidth = WIDTH;
	m_nWndClientHeight = HEIGHT;

	CGameFrameWork_2D::enter(new CSceneLoading_2D("CSceneLoading_2D"));
}

void CGameFrameWork_2D::PreDraw(DWORD dwColor)
{
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hdc, hBrush);
	::Rectangle(m_hdc, 0, 0, m_nWndClientWidth, m_nWndClientHeight);
	::SelectObject(m_hdc, hOldBrush);
	::DeleteObject(hBrush);
}

void CGameFrameWork_2D::BeginDraw()
{
	HDC hDC = ::GetDC(m_hWnd);

	if (!m_hdc)
		m_hdc = ::CreateCompatibleDC(hDC);
	if (m_hBitmapFrameBuffer)
	{
		::SelectObject(m_hdc, nullptr);
		::DeleteObject(m_hBitmapFrameBuffer);
		m_hBitmapFrameBuffer = nullptr;
	}

	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_nWndClientWidth, m_nWndClientHeight);
	::SelectObject(m_hdc, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(m_hdc, TRANSPARENT);

}

void CGameFrameWork_2D::OnDraw()
{
	this->BeginDraw();

	//drawRect
	Draw_2D::drawRect(m_hdc, 0, 0, m_nWndClientWidth, m_nWndClientHeight, BLACK, true);


	if (!states.empty())
	{
		states.back()->OnDraw(m_hdc);
	}
	// resume previous state
	else if (!states.empty()) {
		states.back()->OnDraw(m_hdc);
	}

	CGameFrameWork_2D::EndDraw();
}

void CGameFrameWork_2D::EndDraw()
{
	HDC hDC = ::GetDC(m_hWnd);

	::BitBlt(hDC, 0, 0, m_nWndClientWidth, m_nWndClientHeight, m_hdc, 0, 0, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);


}

void CGameFrameWork_2D::OnUpdate()
{
	TIMEMGR->Tick();
	m_fTimeElapsed = TIMEMGR->GetTimeElapsed();

	if (!states.empty())
	{
		states.back()->OnUpdate(m_fTimeElapsed);
	}
	// resume previous state
	else if (!states.empty()) {
		states.back()->OnUpdate(m_fTimeElapsed);
	}


	//04_SoundMGR->OnUpdate();

	//04_Sound::Manager->onUpdate();
}

void CGameFrameWork_2D::OnDestory()
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->exit();
		states.pop_back();
	}
	// resume previous state
	if (!states.empty()) {
		states.back()->exit();
	}
	RENDERMGR_2D->Release();
	CGameFrameWork_2D::exit();
	CGameFrameWork_2D::ReleseInstance();
	//SOUNDMGR->ReleseInstance();

}


//CallBack

void CGameFrameWork_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	if (!states.empty())
	{
		states.back()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	}

	// resume previous state
	else if (!states.empty()) 
	{
		states.back()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	}

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;

	case WM_MOUSEMOVE:
		break;
	}
}

void CGameFrameWork_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (nMessageID == WM_KEYUP && wParam == 'q' - 32)
	{
		if (::MessageBox(g_hWnd, L"������ ������??", L"���� �����", MB_OKCANCEL) == 1)
		{
			#ifdef _CONNECT_SERVER_
						//JJY 16'.04.16
						// �̺�Ʈ ����
						TCP->EV.DeleteEvent();

						TCP->RoomListClear();
						TCP->RoomPlayerListClear();

						// closesocket()
						closesocket(TCP->sock);

						// ���� ����
						WSACleanup();
			#endif
			::PostQuitMessage(0);
		}
	}


	if (!states.empty())
	{
		states.back()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	}

	// resume previous state
	else if (!states.empty()) {
		states.back()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	}
}

void CGameFrameWork_2D::SetWindowsSizeChange(int width, int height)
{
	m_nWndClientWidth = width;
	m_nWndClientHeight = height;

	if (!states.empty())
	{
		states.back()->SetWindowsSizeChange(m_nWndClientWidth, m_nWndClientHeight);
	}
	// resume previous state
	else if (!states.empty()) {
		states.back()->SetWindowsSizeChange(m_nWndClientWidth, m_nWndClientHeight);
	}

}

void CGameFrameWork_2D::CallMenu(WPARAM wParam)
{
	if (!states.empty())
	{
		states.back()->CallMenu(wParam);
	}
	// resume previous state
	else if (!states.empty()) {
		states.back()->CallMenu(wParam);

	}
	//nowState->callMenu(wParam);
}

LRESULT CALLBACK CGameFrameWork_2D::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��) ���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
	case WM_SIZE:
	{
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}
