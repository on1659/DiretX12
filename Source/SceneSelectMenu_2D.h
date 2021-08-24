#pragma once


class CSpriteObject_2D;
class CGraphicObject_2D;

class CSceneSelectMenu_2D : public CGameState_2D
{

private:
	CGraphicObject_2D			m_Background;
	CSpriteObject_2D			m_Button[2];
	BoundingBox_2D				m_btnStart;
	BoundingBox_2D				m_btnExit;

	CSpriteObject_2D			m_monster;

	CSpriteObject_2D			m_player;

	bool						m_bReady;
	int							m_bMouseSound;
	int							m_iMaxValue = 255;
	Lu_Scroll					m_Scr;
	int							m_nScrollPosition;
	int							m_persent = 0;

public:

	CSceneSelectMenu_2D(std::string name);

	~CSceneSelectMenu_2D();

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

