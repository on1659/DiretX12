#include "stdafx.h"
#include "GameFrameWork.h"
#include "GameFrameWork_2D.h"

#include "SceneLogin_2D.h"
#include "SceneLobby_2D.h"

#include <algorithm>

#ifdef _MULIT_THREAD_LOADING_
	#include "ThreadProcessor.h"
	#include "ContentManager.h"
#endif

CSceneLogin_2D::CSceneLogin_2D(std::string name) : CGameState_2D(name)
{

}

CSceneLogin_2D::~CSceneLogin_2D()
{
	
	delete this;
}

void CSceneLogin_2D::enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight)
{

	//m_width = nWndClientWidth;
	//m_height = nWndClientHeight;

	m_width = nWndClientWidth;
	m_height = nWndClientHeight - 40;

	mx = my = 0;

	m_Background.Init("�α��� ���ȭ��", m_width / 2, m_height / 2);
	m_Background.OnCreate(m_width / 2, m_height / 2, m_width, m_height);

	m_btnLogin.Init("�α��� �����ϱ� ��ư", (m_width / 2) + 20, (m_height / 2) + 250);
	m_btnLogin.OnCreate((m_width / 2) + 20, (m_height / 2) + 250, 200, 100, 10, 1, 1);

	m_btnGameQuit.Init("���� â ���� ��ư", (m_width / 2) + 500, (m_height / 2) - 250);
	m_btnGameQuit.OnCreate((m_width / 2) + 500, (m_height / 2) - 250, 60, 60, 10, 1, 1);

	//JJY '16.05.11
	/*
		������ ���� ID
	*/

	//m_Setid[0] = 'T';
	//m_Setid[1] = 'E';
	//m_Setid[2] = 'S';
	//m_Setid[3] = 'T';
	//m_Setid[4] = '-';
	//
	//for (int i = 5; i < 11; ++i)
	//{
	//	m_Setid[i] = 97 + rand() % 20;
	//}
	//
	//m_Setid[11] = '\0';
	//----------------------------------------

	m_Setid->push_back('T');
	m_Setid->push_back('E');
	m_Setid->push_back('S');
	m_Setid->push_back('T');
	m_Setid->push_back('-');

	for (int i = 5; i < 11; ++i)
	{
		m_Setid->push_back(97 + rand() % 20);
	}

	//JJY '16.05.11
	/*
		����IP�ּ� ���� ����Ʈ �ڽ��� ID�Է��� ����Ʈ �ڽ�
		�⺻ ����IP�ּҴ� ������
	*/
	EDITBOX_2D->PushEditBoxCenterPosition((m_width / 2) + 20, (m_height / 2) + 35, 350, T_SERVERIP, Color_2D::WHITE_2D, Color_2D::ORANGE_2D, 150, 15);
	EDITBOX_2D->PushEditBoxCenterPosition((m_width / 2) + 20, (m_height / 2) + 155, 350, m_Setid->c_str(), Color_2D::WHITE_2D, Color_2D::ORANGE_2D, 150, 15);


	//RSH 2016.08.22
	/*
		�κ� ����� ���
	*/
	SOUNDMGR->PlayBGM("�κ�");
}

void CSceneLogin_2D::exit()
{
}

void CSceneLogin_2D::pause()
{
}

void CSceneLogin_2D::resume()
{
}

//Render & Logic
void CSceneLogin_2D::OnDraw(HDC hdc)
{
	m_Background.OnDraw(hdc);
	m_btnLogin.OnDraw(hdc);

	m_btnGameQuit.OnDraw(hdc);

	EDITBOX_2D->OnDraw(hdc);
}

void CSceneLogin_2D::OnUpdate(float fTimeElapsed)
{

	//if (m_alpha == 200)
	//	SOUNDMGR->PlayEFFECT("�ε��츮��");
	//
	//if (m_alpha < 255)
	//	FRAMEWORK_2D->change_state(new CSceneSelectMenu_2D("SceneSelect"));

		if (INPUT->OnlyKeyDown(YT_KEY::YK_C))	//XINPUT_GAMEPAD_START
		{
			GameStartFunc();
		}

		if (INPUT->OnlyKeyDown(YT_KEY::YK_G))	//XINPUT_GAMEPAD_BACK
		{

			#ifdef _CONNECT_SERVER_

						// �̺�Ʈ ����
						TCP->EV.DeleteEvent();

						TCP->RoomListClear();
						TCP->RoomPlayerListClear();

						// closesocket()
						closesocket(TCP->sock);

						// ���� ����
						WSACleanup();

			#endif

			EDITBOX_2D->Clear();

			PostQuitMessage(0);
		}
	
	//KYT '16.05.12
	/*
		���� Ű�� ������ GameStartFunc �Լ��� �����
	*/
	else if (EDITBOX_2D->IsStart())
	{
		GameStartFunc();
	}

}

//Call
void CSceneLogin_2D::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	
	switch (nMessageID)
	{
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		break;

	case WM_RBUTTONDOWN:
		m_bRbutton = true;
		break;

	case WM_RBUTTONUP:
		m_bRbutton = false;
		break;

	case WM_LBUTTONDOWN:
		m_bLbutton = true;
		break;

	case WM_LBUTTONUP:
		m_bLbutton = false;


		if (m_btnLogin.GetBoundingBox().Contains(mx, my))
		{
			GameStartFunc();
		}

		if (m_btnGameQuit.GetBoundingBox().Contains(mx, my))
		{

			#ifdef _CONNECT_SERVER_

						// �̺�Ʈ ����
						TCP->EV.DeleteEvent();

						TCP->RoomListClear();
						TCP->RoomPlayerListClear();

						// closesocket()
						closesocket(TCP->sock);

						// ���� ����
						WSACleanup();

			#endif

			EDITBOX_2D->Clear();

			PostQuitMessage(0);
		}

		break;
	}

}

void CSceneLogin_2D::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSceneLogin_2D::SetWindowsSizeChange(int width, int height)
{
}

void CSceneLogin_2D::CallMenu(WPARAM wParam)
{
}

void CSceneLogin_2D::GameStartFunc()
{

	#ifdef _CONNECT_SERVER_

		//JJY '16.05.11
		/*
		���� IP�ּ� �����̸� Connect���� �ʰ� �޽���â�� �߰� ����
		�߰� - count�� ���� ������ ���� 3�� �ƴϸ� ����(���� ������ �߸��ȰŸ� ���� ���� �����ϴµ� �����ɸ�����)
		*/

		string tmp_serverip = EDITBOX_2D->GetText(0);

		if (count(tmp_serverip.begin(), tmp_serverip.end(), '.') != 3)
			GMessageBox(L"Socket Error", L"���� ���� �Ǵ� ���� IP�ּҸ� �߸� �Է��ϼ̽��ϴ�.");
		else
		{
			// connect()
			SOCKADDR_IN serveraddr;
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(EDITBOX_2D->GetText(0).c_str());
			serveraddr.sin_port = htons(SERVERPORT);
			int retval = connect(TCP->sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

			if (retval == SOCKET_ERROR) {

				GMessageBox(L"Socket Error", L"���� ���� �Ǵ� ���� IP�ּҸ� �߸� �Է��ϼ̽��ϴ�.");
				//TCP->err_quit("connect()");

			}
			else
			{

				// ���� ��� ������ ����
				CreateThread(nullptr, 0, TCP->ClientMain, (LPVOID*)TCP, 0, nullptr);

				///////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////


				P_CS_LOGIN_REQUEST cs_login_request;
				strcpy(cs_login_request.str_id, EDITBOX_2D->GetText(1).c_str());
				strcpy(TCP->my_info.UserID, EDITBOX_2D->GetText(1).c_str());

				cs_login_request.size = sizeof(P_CS_LOGIN_REQUEST);
				cs_login_request.type = CS_LOGIN_REQUEST;
				TCP->SendPacket((unsigned char*)&cs_login_request);

				WaitForSingleObject(TCP->EV.hGetRoomInfoEvent, INFINITE); // ��ü �� ���� ������ �Ϸ� ��ٸ���
				FRAMEWORK_2D->change_state(new CSceneLobby_2D("SceneLobby"));
				ResetEvent(TCP->EV.hGetRoomInfoEvent);

				EDITBOX_2D->Clear();
			}
		}

	#else

	//KYT '16.04.17
	/*
		������ �ε� ������
	*/
#ifdef DX11_REMOVE_LOADING_
	::DimensionsChange(GAME_MODE_INGAME);
#else //DX11_REMOVE_LOADING_
	::PrepareManagerLoading(nullptr);
	::FinalManagerLoading(nullptr);
#endif //DX11_REMOVE_LOADING_
	#endif
}

