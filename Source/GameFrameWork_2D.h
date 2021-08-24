#pragma once

class CGameFrameWork_2D :public CSingleTonBase<CGameFrameWork_2D>
{
private:
	std::vector<CGameState_2D*>		states;


	HINSTANCE						m_hInstance;
	HWND							m_hWnd;

	//Render
	HDC								m_hdc;
	HBITMAP							m_hBitmapFrameBuffer;
	HBITMAP							m_hBitmapSelect;
	DWORD							m_bBackgroundColor;


	float							m_fTimeElapsed;
	int								m_nWndClientWidth;
	int								m_nWndClientHeight;


	//다양한 해상도지원
	int								X = 0;
	int								Y = 1;


public:
	CGameFrameWork_2D();

	~CGameFrameWork_2D();

	//State 
	void enter(CGameState_2D *state);

	void change_state(CGameState_2D *state);

	void exit();

	void pushState(CGameState_2D* state);

	void popState();


	//Render & Logic & Init

	void OnCreate(HINSTANCE hInstance, HWND hMainWnd);

	void PreDraw(DWORD dwColor);

	void BeginDraw();

	void OnDraw();

	void EndDraw();

	void ClearFrameBuffer(DWORD dwColor);

	void OnUpdate();

	void OnDestory();

	//Call 

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SetWindowsSizeChange(int width, int height);

	void CallMenu(WPARAM wParam);

	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wqParam, LPARAM lParam);


	//Get
	int GetWindowWidth(){ return m_nWndClientWidth; }

	int GetWindowHeight(){ return m_nWndClientHeight - 70; }


	//Chagne 2D & 3D 
	void GameMode_2D_3DChange();
};