#pragma once

class CGameState_2D	//InterFace
{

protected:
	HWND								m_hWnd;
	HINSTANCE							m_hInstance;
	std::string							m_name;
	float								DPI[2];
	int									m_width;
	int									m_height;
	float								mx;
	float								my;
	bool								m_bRbutton;
	bool								m_bLbutton;


public:
	CGameState_2D(std::string  name){ m_name = name; }

	virtual ~CGameState_2D(){}

	virtual void enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight) = 0;

	virtual void exit() = 0;

	virtual void pause() = 0;

	virtual void resume() = 0;

	//Render & Logic
	virtual void OnDraw(HDC hdc) = 0;

	virtual void OnUpdate(float fTimeElapsed) = 0;


	//Call
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	virtual void SetWindowsSizeChange(int width, int height) = 0;

	virtual void CallMenu(WPARAM wParam) = 0;

};

