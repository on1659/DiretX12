#pragma once

class CSceneLobby_2D : public CGameState_2D
{
private:
	CGraphicObject_2D			m_Background;
	CSpriteObject_2D			m_btnCreateRoom;
	CSpriteObject_2D			m_btnGameQuit;


	bool						m_bReady;
	int							m_bMouseSound;
	int							m_iMaxValue = 255;
	Lu_Scroll					m_Scr;
	int							m_nScrollPosition;
	int							m_persent = 0;

public:

	CSceneLobby_2D(std::string name);

	~CSceneLobby_2D();


	//Sate Change
	virtual void enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight);

	virtual void exit();

	virtual void pause();

	virtual void resume();

	//Render & Logic
	virtual void OnDraw(HDC hdc);

	virtual void OnUpdate(float fTimeElapsed);


	//Call
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void SetWindowsSizeChange(int width, int height);

	virtual void CallMenu(WPARAM wParam);


};

