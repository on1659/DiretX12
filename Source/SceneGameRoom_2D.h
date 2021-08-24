#pragma once

class CSceneGameRoom_2D : public CGameState_2D
{
private:
	CGraphicObject_2D			m_Background;

	CSpriteObject_2D			m_btnRoomReady[2];

	CSpriteObject_2D			m_btnRoomQuit;

	CSpriteObject_2D			m_btnCharacterSelect_1;
	CSpriteObject_2D			m_btnCharacterSelect_2;
	
	BOOL						m_blankcheck[4];
	CSpriteObject_2D			m_btnTeamLine[4];


	BOOL						m_bReady;


public:

	CSceneGameRoom_2D(std::string name);

	~CSceneGameRoom_2D();


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

