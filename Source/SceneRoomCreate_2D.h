#pragma once

class CSceneRoomCreate_2D : public CGameState_2D
{
private:
	CGraphicObject_2D			m_Background;

	CSpriteObject_2D			m_btnBasicMode[2];
	CSpriteObject_2D			m_btnOccupyMode[2];
	CSpriteObject_2D			m_btnScoreMode[2];

	CSpriteObject_2D			m_btn1x1Mode[2];
	CSpriteObject_2D			m_btn2x2Mode[2];


	CSpriteObject_2D			m_btnCreateRoomConfirm;

	CSpriteObject_2D			m_btnCreateRoomQuit;


	UINT						m_iGameMode;
	UINT						m_iGameTeamSize;

	char						m_SetRoomName[ROOM_NAME_BUF + 1];

public:

	CSceneRoomCreate_2D(std::string name);

	~CSceneRoomCreate_2D();


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

