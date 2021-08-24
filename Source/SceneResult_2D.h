#pragma once

class CSceneResult_2D : public CGameState_2D
{
private:
	CGraphicObject_2D         m_Background;
	CSpriteObject_2D         m_btnGoToLobby;
	CSpriteObject_2D         m_my_character1;
	CSpriteObject_2D         m_my_character2;

	/////////////


	//
	int my_team_score = 0;
	int other_team_score = 0;
	int result = 0;

public:

	CSceneResult_2D(std::string name);

	~CSceneResult_2D();


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
