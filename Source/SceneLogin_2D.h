#pragma once

class CSceneLogin_2D : public CGameState_2D
{
private:
	CGraphicObject_2D			m_Background;
	CSpriteObject_2D			m_btnLogin;

	CSpriteObject_2D			m_btnGameQuit;

	wstring						m_Setid[ID_BUF + 1];

public:

	CSceneLogin_2D(std::string name);

	~CSceneLogin_2D();


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


	//JJY '16.05.12
	/*
		게임 시작 함수
	*/
	void GameStartFunc();


};

