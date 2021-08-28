// LabProject14.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "LabProject14.h"
#include "GameFramework.h"
#include "GameFrameWork_2D.h"
#include "EditBox_2D.h"

#ifdef _MULIT_THREAD_LOADING_
	#include "ThreadProcessor.h"
#endif
#define MAX_LOADSTRING 100

TCHAR				szTitle[MAX_LOADSTRING];				// 제목 표시줄 텍스트입니다.
TCHAR				szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
	{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LABPROJECT14, szWindowClass, MAX_LOADSTRING);
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABPROJECT14));

	if (!InitInstance(hInstance, nCmdShow)) return(FALSE);

	#ifdef _CONNECT_SERVER_

		//JJY 16'.04.16
		///////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		// 윈속 초기화
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
		{
			GMessageBox(L"WSAStartup Error", L"윈속초기화 에러");
			//return 0;
		}

		// 이벤트 생성
		if (TCP->EV.InitEvent() == 1) 
		{
			GMessageBox(L"InitEvent Error", L"이벤트 생성 에러");
			//return 0;
		}

		// socket()
		TCP->sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);
		if (TCP->sock == INVALID_SOCKET) 
		{
			GMessageBox(L"CreateSocket Error", L"소켓 생성 에러");
			TCP->err_quit("socket()");
		}
		///////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////

	#endif

	MSG msg = { 0 };
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			TIMEMGR->Tick();
			INPUT->Update(TIMEMGR->GetTimeElapsed());

			if (gbGameMode == GAME_MODE_INGAME)
			{
				FRAMEWORK->FrameAdvance();
			}

			else if (gbGameMode == GAME_MODE_DIALOG)
			{
				FRAMEWORK_2D->OnUpdate();
				FRAMEWORK_2D->OnDraw();
			}
		}
	}

	FRAMEWORK->OnDestroy();
	FRAMEWORK_2D->OnDestory();

	return((int)msg.wParam);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABPROJECT14));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr/*MAKEINTRESOURCE(IDC_LABPROJECT14)*/;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassEx(&wcex)) return(FALSE);

	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };

	//JJY 16'.04.16 창 스타일 변경
	//DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	DWORD dwStyle = WS_OVERLAPPED;

	AdjustWindowRect(&rc, dwStyle, FALSE);
	HWND hMainWnd = CreateWindow(szWindowClass
		, szTitle
		, dwStyle
		, 180
		, 50
		, rc.right - rc.left
		, rc.bottom - rc.top
		, nullptr
		, nullptr
		, hInstance
		, nullptr);

	//global
	g_hWnd = hMainWnd;
	ghInstance = hInstance;
	if (!hMainWnd) return(FALSE);

	ShowWindow(hMainWnd, nCmdShow);

	FRAMEWORK_2D->OnCreate(hInstance, hMainWnd);
	FRAMEWORK_2D->OnDraw();
	if (!FRAMEWORK->OnCreate(hInstance, hMainWnd)) return(false);


	//KYT '16.04.14 
	/*
	  Loading Thread 두 개 돌리는것
	*/
	#ifdef _MULIT_THREAD_LOADING_
		loadingThread = new std::thread{ LoadingThreadLoad , FRAMEWORK->GetDevice() };
	#endif

	//LoadingThreadLoad(FRAMEWORK->GetDevice());
	::GTimeRecord();
	return(true);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(hWnd, message, wParam, lParam))return 0;

	int wmId, wmEvent;

	switch (message)
	{
	case WM_SIZE:
		break;

	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		if (gbGameMode == GAME_MODE_DIALOG)
			FRAMEWORK_2D->OnProcessingMouseMessage(hWnd, message, wParam, lParam);
		else if (gbGameMode == GAME_MODE_INGAME)
			FRAMEWORK->OnProcessingMouseMessage(hWnd, message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		if (gbGameMode == GAME_MODE_DIALOG)
			FRAMEWORK_2D->OnProcessingKeyboardMessage(hWnd, message, wParam, lParam);
		else if (gbGameMode == GAME_MODE_INGAME)
			FRAMEWORK->OnProcessingKeyboardMessage(hWnd, message, wParam, lParam);

		if (wParam == VK_F11)
			SOUNDMGR->VolumDown();

		if (wParam == VK_F12)
			SOUNDMGR->VolumUp();

		break;

	//KYT '16.05.10
	/*
		EditBox Input		
	*/
	case WM_CHAR:
		if (gbGameMode == GAME_MODE_DIALOG) EDITBOX_2D->Input(wParam);

		if (wParam == '-')
			SOUNDMGR->VolumDown();

		if (wParam == '+')
			SOUNDMGR->VolumUp();
	

		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_DESTROY:
		::ReleaseManager(nullptr);
		#ifdef _CONNECT_SERVER_
				//JJY 16'.04.16
				// 이벤트 제거
				TCP->EV.DeleteEvent();

				TCP->RoomListClear();
				TCP->RoomPlayerListClear();

				// closesocket()
				closesocket(TCP->sock);

				// 윈속 종료
				WSACleanup();
		#endif
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return(0);
}



