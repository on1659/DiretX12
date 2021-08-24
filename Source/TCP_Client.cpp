
#include "stdafx.h"

#include "GameFrameWork_2D.h"
#include "SceneMatchStandby_2D.h"
#include "GameFramework.h"

#include "ThreadProcessor.h"

#include "PlayerState.h"
////////////////////////

WSABUF	send_wsabuf;
char 	send_buffer[BUFSIZE];
WSABUF	recv_wsabuf;
char	recv_buffer[BUFSIZE];
char	packet_buffer[BUFSIZE];
DWORD		in_packet_size = 0;
int		saved_packet_size = 0;
int		g_myid;

////////////////////////


TCPClient::TCPClient()
{
	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUFSIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUFSIZE;

	RoomPlayer_And_MyInfo_Init();

	////더미 방
	//ROOMINFO lounge_room;
	//strcpy(lounge_room.room_name, "LOUNGE");
	//lounge_room.max_player = -1;
	//lounge_room.game_mode = -1;
	//
	//RoomInfoList.push_back(lounge_room);
	//
	//++NextRoomNum;
}
TCPClient::~TCPClient()
{

}

// TCP 클라이언트 시작 부분
DWORD WINAPI TCPClient::ClientMain(LPVOID arg)
{
	TCPClient *tcpClient = (TCPClient *)arg;

	int retval;

	// 윈속 초기화
	//WSADATA wsa;
	//if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	//	return 1;

	//// socket()
	//tcpClient->sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);
	//if (tcpClient->sock == INVALID_SOCKET) tcpClient->err_quit("socket()");


	//// connect()
	//SOCKADDR_IN serveraddr;
	//ZeroMemory(&serveraddr, sizeof(serveraddr));
	//serveraddr.sin_family = AF_INET;
	//serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	//serveraddr.sin_port = htons(SERVERPORT);
	//retval = connect(tcpClient->sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	//if (retval == SOCKET_ERROR) tcpClient->err_quit("connect()");


	///////////////////////////

	// 소켓 정보 추가 & WSAEventSelect()
	tcpClient->AddSocketInfo(tcpClient->sock);
	retval = WSAEventSelect(tcpClient->sock, tcpClient->EventArray[tcpClient->nTotalSockets - 1],
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (retval == SOCKET_ERROR) tcpClient->err_quit("WSAEventSelect()");

	// 데이터 통신에 사용할 변수
	WSANETWORKEVENTS NetworkEvents;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int index, addrlen;
	index = 0;


	while (1) {

		// 이벤트 객체 관찰하기
		index = WSAWaitForMultipleEvents(tcpClient->nTotalSockets, tcpClient->EventArray,
			FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED) continue;
		index -= WSA_WAIT_EVENT_0;

		// 구체적인 네트워크 이벤트 알아내기
		retval = WSAEnumNetworkEvents(tcpClient->sock,
			tcpClient->EventArray[index], &NetworkEvents);
		if (retval == SOCKET_ERROR) continue;


		//switch (NetworkEvents.lNetworkEvents)
		//{
		//case FD_CONNECT:
		//	break;
		//
		//case FD_WRITE:
		//	break;
		//
		//case FD_READ:
		//	break;
		//
		//case FD_CLOSE:
		//	break;
		//}

		// FD_READ & FD_WRITE 이벤트 처리
		if (NetworkEvents.lNetworkEvents & FD_READ
			|| NetworkEvents.lNetworkEvents & FD_WRITE)
		{
			if (NetworkEvents.lNetworkEvents & FD_READ
				&& NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				tcpClient->err_display((char*)&NetworkEvents.iErrorCode[FD_READ_BIT]);
				continue;
			}
			if (NetworkEvents.lNetworkEvents & FD_WRITE
				&& NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				tcpClient->err_display((char*)&NetworkEvents.iErrorCode[FD_WRITE_BIT]);
				continue;
			}

			if (NetworkEvents.lNetworkEvents & FD_READ)
			{

				tcpClient->ReadPacket(tcpClient->sock);
			}
		}

		// FD_CLOSE 이벤트 처리
		if (NetworkEvents.lNetworkEvents & FD_CLOSE) {
			if (NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
				tcpClient->err_display((char*)&NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
			tcpClient->RemoveSocketInfo(index);
		}
	}

	closesocket(tcpClient->sock);

	return 0;
}


// 소켓 정보 추가
BOOL TCPClient::AddSocketInfo(SOCKET sock)
{
	SOCKETINFO *ptr = new SOCKETINFO;
	if (ptr == nullptr) {
		printf("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT) {
		err_display("WSACreateEvent()");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	SocketInfoArray[nTotalSockets] = ptr;
	EventArray[nTotalSockets] = hEvent;
	++nTotalSockets;

	return TRUE;
}

// 소켓 정보 삭제
void TCPClient::RemoveSocketInfo(int nIndex)
{
	SOCKETINFO *ptr = SocketInfoArray[nIndex];

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;
	WSACloseEvent(EventArray[nIndex]);

	if (nIndex != (nTotalSockets - 1)) {
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
		EventArray[nIndex] = EventArray[nTotalSockets - 1];
	}
	--nTotalSockets;
}


// 소켓 함수 오류 출력 후 종료
void TCPClient::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, nullptr);
	MessageBox(g_hWnd, (LPCTSTR)lpMsgBuf, (TCHAR*)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}


// 사용자 정의 데이터 수신 함수
int TCPClient::recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

void TCPClient::ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;


	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, nullptr, nullptr);
	if (ret) {
		int err_code = WSAGetLastError();
		printf("Recv Error [%d]\n", err_code);
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}


void TCPClient::ProcessPacket(char* packet_inf)
{
	UINT packet_type = packet_inf[1];
	//

	ROOMINFO tmp_roominfo;
	ROOMINFO tmp_new_roominfo;

	PLAYER_INFO tmp_playerinfo;
	PLAYER_INFO tmp_new_playerinfo;

	UINT NextRoomNum = 0;
	UINT NextUserNum = 0;
	int pos = 0;

	switch (packet_type)
	{
	case SC_LOGIN_REQUEST_RETURN:
	{
		P_SC_LOGIN_REQUEST_RETURN* recv_packet = reinterpret_cast<P_SC_LOGIN_REQUEST_RETURN *>(packet_inf);

		P_CS_LOBBY_IN cs_lobby_in;
		cs_lobby_in.size = sizeof(P_CS_LOBBY_IN);
		cs_lobby_in.type = CS_LOBBY_IN;
		SendPacket((unsigned char*)&cs_lobby_in);

		break;
	};

	case SC_LOBBY_ROOMLIST_SIZE:
	{
		P_SC_LOBBY_ROOMLIST_SIZE* recv_packet = reinterpret_cast<P_SC_LOBBY_ROOMLIST_SIZE *>(packet_inf);

		RoomListClear();

		for (int i = 0; i < recv_packet->roomlist_size; ++i)
			RoomInfoList.push_back(tmp_roominfo);

		P_CS_LOBBY_REQUEST cs_lobby_request;
		cs_lobby_request.size = sizeof(P_CS_LOBBY_REQUEST);
		cs_lobby_request.type = CS_LOBBY_REQUEST;
		SendPacket((unsigned char*)&cs_lobby_request);

		//WSAEventSelect(TCP->sock, TCP->EventArray[TCP->nTotalSockets - 1], FD_WRITE);

		break;
	};
	case SC_LOBBY_ROOMLIST_INFO:
	{
		P_SC_LOBBY_ROOMLIST_INFO* recv_packet = reinterpret_cast<P_SC_LOBBY_ROOMLIST_INFO *>(packet_inf);

		strcpy(RoomInfoList[recv_packet->room_info.room_num].room_name, recv_packet->room_info.room_name);
		RoomInfoList[recv_packet->room_info.room_num].max_player = recv_packet->room_info.max_player;
		RoomInfoList[recv_packet->room_info.room_num].game_mode = recv_packet->room_info.game_mode;

		if (recv_packet->room_info.room_num == RoomInfoList.size() - 1)
			SetEvent(EV.hGetRoomInfoEvent); // 전체 방 정보 얻어오기 완료 알리기
											//	DialogBox(m_Inst, MAKEINTRESOURCE(IDD_DIALOG2), nullptr, LobbyDlgProc);
											//LobbyPopUp();

		break;
	};

	case SC_LOBBY_ROOMLIST_INFO_UPDATE:
	{
		P_SC_LOBBY_ROOMLIST_INFO_UPDATE* recv_packet = reinterpret_cast<P_SC_LOBBY_ROOMLIST_INFO_UPDATE *>(packet_inf);

		//RoomInfoList의 사이즈가 다음 넘버링이 된다.
		NextRoomNum = RoomInfoList.size();


		RoomInfoList.push_back(tmp_new_roominfo);

		strcpy(RoomInfoList[NextRoomNum].room_name, recv_packet->room_info.room_name);
		RoomInfoList[NextRoomNum].max_player = recv_packet->room_info.max_player;
		RoomInfoList[NextRoomNum].game_mode = recv_packet->room_info.game_mode;

		break;
	};

	case SC_ROOM_USERLIST_SIZE:
	{
		P_SC_ROOM_USERLIST_SIZE* recv_packet = reinterpret_cast<P_SC_ROOM_USERLIST_SIZE *>(packet_inf);

		RoomPlayerListClear();

		//최대 인원수 이상이면 입장 불가
		if (recv_packet->bRoomEntranceApprove == FALSE)
		{
			my_info.bRoomEntranceSuccess = FALSE;

			GMessageBox(L"입장 불가", L"방 인원이 다 찼거나 이미 게임이 시작되었습니다.");

			SetEvent(EV.hGetUserInfoEvent);
		}
		else
		{
			my_info.bRoomEntranceSuccess = TRUE;

			for (int i = 0; i < recv_packet->userlist_size; ++i)
				RoomPlayerInfoList.push_back(tmp_playerinfo);

			P_CS_ROOM_USER_INFO_REQUEST cs_room_user_info_request;
			cs_room_user_info_request.size = sizeof(P_CS_ROOM_USER_INFO_REQUEST);
			cs_room_user_info_request.type = CS_ROOM_USER_INFO_REQUEST;
			SendPacket((unsigned char*)&cs_room_user_info_request);
		}

		break;
	};

	case SC_ROOM_USERLIST_INFO:
	{
		P_SC_ROOM_USERLIST_INFO* recv_packet = reinterpret_cast<P_SC_ROOM_USERLIST_INFO *>(packet_inf);

		strcpy(RoomPlayerInfoList[recv_packet->UserNum].UserID, recv_packet->str_id);
		RoomPlayerInfoList[recv_packet->UserNum].InRoom_MyNum = recv_packet->UserNum;

		//초기 정보 입히기
		RoomPlayerInfoList[recv_packet->UserNum].iSelectedCharacter = recv_packet->iSelectedCharacter;
		RoomPlayerInfoList[recv_packet->UserNum].iSelectedTeam = recv_packet->iSelectedTeam;


		if (recv_packet->UserNum == RoomPlayerInfoList.size() - 1)
		{
			SetEvent(EV.hGetUserInfoEvent); // 쓰기 완료 알리기
		}

		break;
	};

	case SC_ROOM_USERLIST_INFO_UPDATE:
	{
		P_SC_ROOM_USERLIST_INFO_UPDATE* recv_packet = reinterpret_cast<P_SC_ROOM_USERLIST_INFO_UPDATE *>(packet_inf);

		if (recv_packet->WhatChange == PLAYER_ENTRANCE)
		{
			//UserInfoList의 사이즈가 다음 넘버링이 된다.
			NextUserNum = RoomPlayerInfoList.size();

			tmp_new_playerinfo.InRoom_MyNum = NextUserNum;

			RoomPlayerInfoList.push_back(tmp_new_playerinfo);

			strcpy(RoomPlayerInfoList[NextUserNum].UserID, recv_packet->str_id);

			//새로 온 유저 정보 입히기
			RoomPlayerInfoList[NextUserNum].iSelectedCharacter = recv_packet->iSelectedCharacter;
			RoomPlayerInfoList[NextUserNum].iSelectedTeam = recv_packet->iSelectedTeam;

			if (my_info.InRoom_MyNum == NextUserNum)
			{
				my_info.iSelectedCharacter = recv_packet->iSelectedCharacter;
				my_info.iSelectedTeam = recv_packet->iSelectedTeam;
			}

			SetEvent(EV.hGetUserInfoEvent); // 전체 방의 유저 정보 얻어오기 완료 알리기
		}
		else if (recv_packet->WhatChange == PLAYER_QUIT)
		{
			//방 유저 리스트에서 정보 삭제
			for (int i = 0; i < RoomPlayerInfoList.size(); i++)
			{
				if (RoomPlayerInfoList[i].InRoom_MyNum == recv_packet->InRoom_UserNum)
				{
					RoomPlayerInfoList.erase(RoomPlayerInfoList.begin() + i);
					break;
				}
			}

			//if (my_info.InRoom_MyNum >= tmp_sc_room_userlist_info_update->InRoom_UserNum)
			//	my_info.InRoom_MyNum--;

		}
		else if (recv_packet->WhatChange == PLAYER_CHANGE_TEAM_LINE)
		{
			//for (int i = 0; i < RoomPlayerInfoList.size(); i++)
			//{
			//	if (RoomPlayerInfoList[i].InRoom_MyNum == tmp_sc_room_userlist_info_update->InRoom_UserNum)
			//	{
			//		RoomPlayerInfoList[i].iSelectedTeam = tmp_sc_room_userlist_info_update->iSelectedTeam;
			//		break;
			//	}
			//}


			RoomPlayerInfoList[recv_packet->InRoom_UserNum].iSelectedTeam = recv_packet->iSelectedTeam;
		}
		else if (recv_packet->WhatChange == PLAYER_CHANGE_CHARACTER)
		{
			//for (int i = 0; i < RoomPlayerInfoList.size(); i++)
			//{
			//	if (RoomPlayerInfoList[i].InRoom_MyNum == tmp_sc_room_userlist_info_update->InRoom_UserNum)
			//	{
			//		RoomPlayerInfoList[i].iSelectedCharacter = tmp_sc_room_userlist_info_update->iSelectedCharacter;
			//		break;
			//	}
			//}

			RoomPlayerInfoList[recv_packet->InRoom_UserNum].iSelectedCharacter = recv_packet->iSelectedCharacter;
		}
		else if (recv_packet->WhatChange == PLAYER_LOADING_COMPLETION)
		{
			RoomPlayerInfoList[recv_packet->InRoom_UserNum].bLoadingCompletion = TRUE;
			//for (int i = 0; i < RoomPlayerInfoList.size(); i++)
			//{
			//	if (RoomPlayerInfoList[i].InRoom_MyNum == tmp_sc_room_userlist_info_update->InRoom_UserNum)
			//	{
			//		RoomPlayerInfoList[i].bLoadingCompletion = TRUE;
			//		break;
			//	}
			//}

			if (recv_packet->bAllPlayerLoadingCompletion == TRUE)
			{

				//KYT '16.07.13
				/*
				영태추가
				*/
				::FinalManagerLoading(nullptr);

				//게임 시작한다.
				DimensionsChange(YT_GameMode::GAME_MODE_INGAME, 0);
			}
		}
		else if (recv_packet->WhatChange == PLAYER_READY)
		{
			if (recv_packet->bAllPlayerReady == TRUE)
			{
				//매치 대기 상태로 들어간다.
				FRAMEWORK_2D->change_state(new SceneMatchStandby_2D("SceneMatchStandby"));

				::PrepareManagerLoading(nullptr);

				P_CS_ROOM_SEND_MY_INFO cs_room_send_my_info;
				cs_room_send_my_info.size = sizeof(P_CS_ROOM_SEND_MY_INFO);
				cs_room_send_my_info.type = CS_ROOM_SEND_MY_INFO;

				cs_room_send_my_info.WhatChange = PLAYER_LOADING_COMPLETION;
				SendPacket((unsigned char*)&cs_room_send_my_info);
			}
		}
		else if (recv_packet->WhatChange == PLAYER_UN_READY)
		{

		}

		UINT object_count = 0;

		for (int j = 0; j < RoomPlayerInfoList.size(); j++)
		{
			RoomPlayerInfoList[j].InRoom_MyNum = j;
			RoomPlayerInfoList[j].object_index = -1;

			if (strcmp(RoomPlayerInfoList[j].UserID, my_info.UserID) == 0)
			{
				my_info.InRoom_MyNum = j;
				my_info.object_index = -1;
			}
			else
			{
				RoomPlayerInfoList[j].object_index = object_count;
				object_count++;
			}
		}

		//LoginCheck = FALSE;
		break;
	};

	case SC_ROOM_CHAT:
	{
		break;
	};

	case SC_INGAME_RETURN_MOVE_SYNC:
	{
		P_SC_INGAME_RETURN_MOVE_SYNC* recv_packet = reinterpret_cast<P_SC_INGAME_RETURN_MOVE_SYNC *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}

		RoomPlayerInfoList[target_idx].pos_x = recv_packet->pos_x;
		RoomPlayerInfoList[target_idx].pos_z = recv_packet->pos_z;

		CPlayerManager::GetInstance()->ServerReturn_Pos_Move(RoomPlayerInfoList[target_idx].object_index, recv_packet->pos_x, recv_packet->pos_z);

		break;
	};

	case SC_INGAME_MOVE_INPUT_RETURN:
	{
		P_SC_INGAME_MOVE_INPUT_RETURN* recv_packet = reinterpret_cast<P_SC_INGAME_MOVE_INPUT_RETURN *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}


		if (recv_packet->keytype == YK_W || recv_packet->keytype == YK_UP)
		{
			RoomPlayerInfoList[target_idx].player_input_info.key_W_Down = recv_packet->is_keydown;
		}
		else if (recv_packet->keytype == YK_A || recv_packet->keytype == YK_LEFT)
		{
			RoomPlayerInfoList[target_idx].player_input_info.key_A_Down = recv_packet->is_keydown;
		}
		else if (recv_packet->keytype == YK_S || recv_packet->keytype == YK_DOWN)
		{
			RoomPlayerInfoList[target_idx].player_input_info.key_S_Down = recv_packet->is_keydown;
		}
		else if (recv_packet->keytype == YK_D || recv_packet->keytype == YK_RIGHT)
		{
			RoomPlayerInfoList[target_idx].player_input_info.key_D_Down = recv_packet->is_keydown;
		}

		RoomPlayerInfoList[target_idx].player_input_info.m_fDistance = recv_packet->fDistance;

		break;
	};
	case SC_INGAME_ROTATE_RETURN:
	{
		P_SC_INGAME_ROTATE_RETURN* recv_packet = reinterpret_cast<P_SC_INGAME_ROTATE_RETURN *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}

		CPlayerManager::GetInstance()->ServerReturn_Rotate(RoomPlayerInfoList[target_idx].object_index, recv_packet->cxDelta, recv_packet->cyDelta);

		break;
	};

	case SC_INGAME_VISIBLE_PLAYER_SET:
	{
		P_SC_INGAME_VISIBLE_PLAYER_SET* recv_packet = reinterpret_cast<P_SC_INGAME_VISIBLE_PLAYER_SET *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}

		//std::cout << "SC_INGAME_VISIBLE_PLAYER_SET 함" << std::endl;

		RoomPlayerInfoList[target_idx].ply_IsVisible = TRUE;

		//
		RoomPlayerInfoList[target_idx].player_input_info.key_W_Down = recv_packet->W_Iskeydown;
		RoomPlayerInfoList[target_idx].player_input_info.key_A_Down = recv_packet->A_Iskeydown;
		RoomPlayerInfoList[target_idx].player_input_info.key_S_Down = recv_packet->S_Iskeydown;
		RoomPlayerInfoList[target_idx].player_input_info.key_D_Down = recv_packet->D_Iskeydown;


		RoomPlayerInfoList[target_idx].pos_x = recv_packet->pos_x;
		RoomPlayerInfoList[target_idx].pos_z = recv_packet->pos_z;

		//
		//CPlayerManager::GetInstance()->ServerReturn_Rotate(RoomPlayerInfoList[target_idx].object_index, recv_packet->cxDelta, recv_packet->cyDelta);
		CPlayerManager::GetInstance()->ServerReturn_Pos_Move(RoomPlayerInfoList[target_idx].object_index, recv_packet->pos_x, recv_packet->pos_z);

		break;
	};

	case SC_INGAME_INVISIBLE_PLAYER_SET:
	{
		P_SC_INGAME_INVISIBLE_PLAYER_SET* recv_packet = reinterpret_cast<P_SC_INGAME_INVISIBLE_PLAYER_SET *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}

		//std::cout << "SC_INGAME_INVISIBLE_PLAYER_SET 함" << std::endl;

		if (recv_packet->IsHide == true)
		{
			//cout << " 파티클 터지던가" << RoomPlayerInfoList[target_idx].pos_x << ", " << RoomPlayerInfoList[target_idx].pos_z << endl;
			PARTICLE->StartParticle("하이드", RoomPlayerInfoList[target_idx].pos_x, 1.6f, RoomPlayerInfoList[target_idx].pos_z);
			PARTICLE->StartSprtieAnimation("보라불꽃", RoomPlayerInfoList[target_idx].pos_x, 1.6f, RoomPlayerInfoList[target_idx].pos_z, 1);

			//RSH '16.08.22
			/*
			은신 효과음.(자기 자신)
			*/
			SOUNDMGR->Play("은신", XMFLOAT3(RoomPlayerInfoList[target_idx].pos_x,1, RoomPlayerInfoList[target_idx].pos_z));
		}

		RoomPlayerInfoList[target_idx].ply_IsVisible = FALSE;


		//
		RoomPlayerInfoList[target_idx].player_input_info.key_W_Down = FALSE;
		RoomPlayerInfoList[target_idx].player_input_info.key_A_Down = FALSE;
		RoomPlayerInfoList[target_idx].player_input_info.key_S_Down = FALSE;
		RoomPlayerInfoList[target_idx].player_input_info.key_D_Down = FALSE;

		//
		RoomPlayerInfoList[target_idx].player_input_info.m_fDistance = 0.0f;

		break;
	};

	case SC_INGAME_OTHER_INPUT_RETURN:
	{
		P_SC_INGAME_OTHER_INPUT_RETURN* recv_packet = reinterpret_cast<P_SC_INGAME_OTHER_INPUT_RETURN *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}


		if (recv_packet->keytype == YK_SPACE)
		{
			//cout << "등짝잼" << endl;
			RoomPlayerInfoList[target_idx].player_input_info.key_SPACE_Down = recv_packet->is_keydown;
		}

		break;
	};

	//case SC_INGAME_HIDE_RETURN:
	//{
	//	P_SC_INGAME_HIDE_RETURN* recv_packet = reinterpret_cast<P_SC_INGAME_HIDE_RETURN *>(packet_inf);
	//
	//	UINT target_idx = 0;
	//
	//	for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
	//	{
	//		if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
	//			target_idx = i;
	//	}
	//
	//	//if(recv_packet->is_mousedown == TRUE)
	//	//	RoomPlayerInfoList[target_idx].ply_IsVisible = FALSE;
	//	//else
	//	//	RoomPlayerInfoList[target_idx].ply_IsVisible = TRUE;
	//
	//	break;
	//};

	case SC_INGAME_MONSTER_INFO:
	{
		P_SC_INGAME_MONSTER_INFO* recv_packet = reinterpret_cast<P_SC_INGAME_MONSTER_INFO *>(packet_inf);
		
		//cout << "몬스터 위치 " << recv_packet->pos_x << " , " << recv_packet->pos_z << endl;

		auto v = SPACE->GetDynamicObjectVector();

		if (v.size() == MONSTER_MAXNUM)
			v[recv_packet->monster_idx]->SetPosition(recv_packet->pos_x, 1.0f, recv_packet->pos_z);

		break;
	};
	case SC_INGAME_MATCH_END:
	{
		P_SC_INGAME_MATCH_END* recv_packet = reinterpret_cast<P_SC_INGAME_MATCH_END*>(packet_inf);

		//JJY '16.08.02
		/*
			게임이 종료되었으니, 결과화면으로 간다.
		*/
		CScene::GetInstance()->SetGamePlaying(false);
		break;
	};

	case SC_INGAME_PLAYER_STATE_INFO:
	{
		P_SC_INGAME_PLAYER_STATE_INFO* recv_packet = reinterpret_cast<P_SC_INGAME_PLAYER_STATE_INFO *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}

		if (recv_packet->state == PLYAER_STATE::STATE_DIE)
		{
			if (my_info.iSelectedTeam == recv_packet->team_line)
			{
				PLAYER->GetPlayer()->GetPlayerState()->SetHp(0);
				SOUNDMGR->Play("맞음", PLAYER->GetPlayer()->GetPositionXMFLOAT3());
				//KYT '16.08.20
				/*
					Start Respwan!
				*/
				::GBulrChange(YT_BlurMode::BLUR_RESPAWN);
			}
			else
			{

				PLAYER->GetOtherPlayer(RoomPlayerInfoList[target_idx].object_index)->GetPlayerState()->SetHp(0);
				SOUNDMGR->Play("맞음", PLAYER->GetOtherPlayer(RoomPlayerInfoList[target_idx].object_index)->GetPositionXMFLOAT3());
			}
		}
		else if (recv_packet->state == PLYAER_STATE::STATE_NONE)
		{
			if (my_info.iSelectedTeam == recv_packet->team_line)
			{
				PLAYER->GetPlayer()->Respawn(); //여기서 부르는걸로
			}
			else
			{
				PLAYER->GetOtherPlayer(RoomPlayerInfoList[target_idx].object_index)->Respawn();
			}
		}


		//static_cast<PlayerAnimation*>(PLAYER->GetPlayer()->GetComponent(2))->Respawn();
		break;
	};

	case SC_INGAME_ITEM_INFO:
	{
		P_SC_INGAME_ITEM_INFO* recv_packet = reinterpret_cast<P_SC_INGAME_ITEM_INFO *>(packet_inf);


		UINT nOtherPlayerID = 0;
		std::list<std::shared_ptr<CGameObject>> vContainer;


		UINT target_idx = -1;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->get_player_team_line)
				target_idx = i;
		}


		nOtherPlayerID = RoomPlayerInfoList[target_idx].object_index;

		std::shared_ptr<CPlayer> player = nullptr;
		if (my_info.iSelectedTeam == recv_packet->get_player_team_line)
		{

			player = PLAYER->GetPlayer();
		}
		else
			player = PLAYER->GetOtherPlayer(nOtherPlayerID);


		SPACE->Update(player);

		if (player->GetSpaceNodeQuad() != nullptr)
			vContainer = player->GetSpaceNodeQuad()->GetStaticObjectsContainer();

		bool del = false;
		for (auto &pItemObject : vContainer)
		{
			if (pItemObject->nGID == recv_packet->instance_id)
			{
				if (my_info.iSelectedTeam == recv_packet->get_player_team_line)
				{
					player->SetCompoent(move(pItemObject->MoveCompoent(player, TAG_TYPE::TAG_ITEM)));
					SOUNDMGR->Play("코인", player->GetPositionXMFLOAT3());
				}

				pItemObject->StaticDestory(pItemObject);
				SPACE->StaticDestory(pItemObject);
				del = true;
				break;
			}
		}

		if (!del)
		{
			#pragma region[주변찾기]
						bool stop = false;
						auto vRoomCount = Helper::IsRenderCountVector(SPACE->FindSpaceNum(player->GetPosition()));
						auto vRoom = SPACE->GetRoomContainer();

						for (auto &n : vRoomCount)
						{
							if (n < 0 || n > 63)continue;

							auto vObject = vRoom[n]->GetStaticObjectsContainer();

							for (auto &pItemObject : vObject)
							{
								if (stop)break;

								if (pItemObject->nGID == recv_packet->instance_id)
								{
									if (my_info.iSelectedTeam == recv_packet->get_player_team_line)
									{
										//cout << "내가 아이템 먹었다" << endl;
										player->SetCompoent(move(pItemObject->MoveCompoent(player, TAG_TYPE::TAG_ITEM)));
										SOUNDMGR->Play("코인", player->GetPositionXMFLOAT3());
									}

									pItemObject->StaticDestory(pItemObject);
									SPACE->StaticDestory(pItemObject);
									stop = true;
									del = true;
									break;
								}
							}
						}

			#pragma endregion
		}

		if (!del)
		{
			#pragma region[모든 방 찾기]
						bool stop = false;
						auto vRoom = SPACE->GetRoomContainer();
						for (auto &room : vRoom)
						{
							if (stop)break;

							auto vObject = room->GetStaticObjectsContainer();
							for (auto &pItemObject : vObject)
							{
								if (stop)break;

								if (pItemObject->nGID == recv_packet->instance_id)
								{
									if (my_info.iSelectedTeam == recv_packet->get_player_team_line)
									{
										//cout << "내가 아이템 먹었다" << endl;
										player->SetCompoent(move(pItemObject->MoveCompoent(player, TAG_TYPE::TAG_ITEM)));
										SOUNDMGR->Play("코인", player->GetPositionXMFLOAT3());
									}

									pItemObject->StaticDestory(pItemObject);
									SPACE->StaticDestory(pItemObject);
									stop = true;
									break;
								}
							}
						}
			#pragma endregion
		}
		player.reset();
		break;
	};
	case SC_INGAME_THROWN_OBJ_INFO:
	{
		P_SC_INGAME_THROWN_OBJ_INFO* recv_packet = reinterpret_cast<P_SC_INGAME_THROWN_OBJ_INFO *>(packet_inf);


		if (recv_packet->obj_type == THROWN_OBJ_TYPE::OBJ_STUN_OBJECT)
		{
			CScene::GetInstance()->AddBullet(recv_packet->pos, recv_packet->look, recv_packet->team_line);
		}
		else if (recv_packet->obj_type == THROWN_OBJ_TYPE::OBJ_BIND_OBJECT)
		{
			//cout << "바인드" << endl;
			CScene::GetInstance()->AddThrowSkill(recv_packet->pos, recv_packet->look, recv_packet->team_line);
		}
		else if (recv_packet->obj_type == THROWN_OBJ_TYPE::OBJ_FLASH_BANG)
		{
			CScene::GetInstance()->AddFlashItem(recv_packet->pos, recv_packet->look, recv_packet->team_line);
		}
		else if (recv_packet->obj_type == THROWN_OBJ_TYPE::OBJ_FAKE_ITEM)
		{
			CScene::GetInstance()->AddFakeInstall(recv_packet->pos);
			//cout << "페이크 아이템 " << "포지션 " << recv_packet->pos.x << "," << recv_packet->pos.z << endl;
		}
		else if (recv_packet->obj_type == THROWN_OBJ_TYPE::OBJ_TRAP)
		{
			//cout << "트랩트랩 변신" << endl;

			UINT target_idx = -1;
		
			for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
			{
				if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
					target_idx = i;
			}
		
			std::shared_ptr<CPlayer> get_player = nullptr;
		
			if (my_info.iSelectedTeam == recv_packet->team_line)
				get_player = PLAYER->GetPlayer();
			else
				get_player = PLAYER->GetOtherPlayer(RoomPlayerInfoList[target_idx].object_index);
		
			CScene::GetInstance()->AddTrap(recv_packet->pos, get_player.get(), recv_packet->team_line);
		
			get_player.reset();
		}

		break;
	};

	case SC_INGAME_BULLETIN_BOARD_UPDATE:
	{
		P_SC_INGAME_BULLETIN_BOARD_UPDATE* recv_packet = reinterpret_cast<P_SC_INGAME_BULLETIN_BOARD_UPDATE *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}

		if (recv_packet->WhatUpdate == BB_KILL)
		{
			RoomPlayerInfoList[target_idx].iKillNum = recv_packet->UpdateContent;
		}
		else if (recv_packet->WhatUpdate == BB_DEATH)
		{
			RoomPlayerInfoList[target_idx].iDeathNum = recv_packet->UpdateContent;
		}
		else if (recv_packet->WhatUpdate == BB_MONSTER_KILL)
		{
			RoomPlayerInfoList[target_idx].iMonsterKillNum = recv_packet->UpdateContent;
		}
		else if (recv_packet->WhatUpdate == BB_SCORE)
		{
			RoomPlayerInfoList[target_idx].iScoreNum = recv_packet->UpdateContent;
		}


		break;
	};

	case SC_INGAME_SPECIAL_EFFECT_INFO:
	{
		P_SC_INGAME_SPECIAL_EFFECT_INFO* recv_packet = reinterpret_cast<P_SC_INGAME_SPECIAL_EFFECT_INFO *>(packet_inf);

		UINT target_idx = 0;

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			if (RoomPlayerInfoList[i].iSelectedTeam == recv_packet->team_line)
				target_idx = i;
		}

		if (recv_packet->effect_type == SPECIAL_EFFECT_TYPE::SE_MISS)
		{
			PARTICLE->StartSprtieAnimation("미스", RoomPlayerInfoList[target_idx].pos_x, 1.6f, RoomPlayerInfoList[target_idx].pos_z, 1);
			SOUNDMGR->Play("회피", XMFLOAT3(RoomPlayerInfoList[target_idx].pos_x, 1.6f, RoomPlayerInfoList[target_idx].pos_z));
		}

		break;
	};

	default:
		cout << "Unknown Server Packet!" << endl;
		break;
	}
}

void TCPClient::SendPacket(unsigned char* packet)
{
	//패킷의 4번은 패킷타입
	UINT packet_type = packet[1];
	int retval = 0;

	DWORD iobyte;

	switch (packet_type)
	{
	case CS_LOGIN_REQUEST:
	{
		P_CS_LOGIN_REQUEST* tmp_packet = (P_CS_LOGIN_REQUEST*)packet;
		P_CS_LOGIN_REQUEST* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_LOGIN_REQUEST *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_LOGIN_REQUEST));

		send_packet->size = sizeof(P_CS_LOGIN_REQUEST);
		send_packet->type = CS_LOGIN_REQUEST;

		send_wsabuf.len = sizeof(P_CS_LOGIN_REQUEST);

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		break;
	};
	case CS_LOBBY_IN:
	{

		P_CS_LOBBY_IN* tmp_packet = (P_CS_LOBBY_IN*)packet;
		P_CS_LOBBY_IN* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_LOBBY_IN *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_LOBBY_IN));

		send_packet->size = sizeof(P_CS_LOBBY_IN);
		send_packet->type = CS_LOBBY_IN;

		send_wsabuf.len = sizeof(P_CS_LOBBY_IN);

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		break;
	};
	case CS_LOBBY_REQUEST:
	{
		P_CS_LOBBY_REQUEST* tmp_packet = (P_CS_LOBBY_REQUEST*)packet;
		P_CS_LOBBY_REQUEST* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_LOBBY_REQUEST *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_LOBBY_REQUEST));

		for (int idx = 0; idx < RoomInfoList.size(); ++idx)
		{
			send_packet->size = sizeof(P_CS_LOBBY_REQUEST);
			send_packet->type = CS_LOBBY_REQUEST;
			send_packet->Request_RoomNum = idx;

			send_wsabuf.len = sizeof(P_CS_LOBBY_REQUEST);

			WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);
		}

		break;
	};
	case CS_ROOM_IN:
	{
		//방안의 플레이어와 나의 정보 초기화
		//RoomPlayer_And_MyInfo_Init();
		//---------------------------------------------------

		P_CS_ROOM_IN* tmp_packet = (P_CS_ROOM_IN*)packet;
		P_CS_ROOM_IN* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_ROOM_IN *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_ROOM_IN));

		send_packet->size = sizeof(P_CS_ROOM_IN);
		send_packet->type = CS_ROOM_IN;

		send_wsabuf.len = sizeof(P_CS_ROOM_IN);

		my_info.InRoomNum = send_packet->InRoomNum;

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		break;
	};
	case CS_ROOM_USER_INFO_REQUEST:
	{
		P_CS_ROOM_USER_INFO_REQUEST* tmp_packet = (P_CS_ROOM_USER_INFO_REQUEST*)packet;
		P_CS_ROOM_USER_INFO_REQUEST* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_ROOM_USER_INFO_REQUEST *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_ROOM_USER_INFO_REQUEST));

		for (int idx = 0; idx < RoomPlayerInfoList.size(); ++idx)
		{
			send_packet->size = sizeof(P_CS_ROOM_USER_INFO_REQUEST);
			send_packet->type = CS_ROOM_USER_INFO_REQUEST;
			send_packet->Request_RoomNum = my_info.InRoomNum;
			send_packet->Request_UserNum = idx;

			send_wsabuf.len = sizeof(P_CS_ROOM_USER_INFO_REQUEST);

			WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);
		}

		if (RoomPlayerInfoList.size() == 0)
			SetEvent(EV.hGetUserInfoEvent); // 쓰기 완료 알리기


		break;
	};

	case CS_ROOM_SEND_MY_INFO:
	{
		P_CS_ROOM_SEND_MY_INFO* tmp_packet = (P_CS_ROOM_SEND_MY_INFO*)packet;
		P_CS_ROOM_SEND_MY_INFO* send_packet;
		//
		//*send_buffer = reinterpret_cast<char >(send_packet);
		send_packet = reinterpret_cast<P_CS_ROOM_SEND_MY_INFO *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_ROOM_SEND_MY_INFO));

		send_packet->size = sizeof(P_CS_ROOM_SEND_MY_INFO);
		send_packet->type = CS_ROOM_SEND_MY_INFO;

		send_wsabuf.len = sizeof(P_CS_ROOM_SEND_MY_INFO);

		send_packet->InRoomNum = my_info.InRoomNum;

		if (send_packet->WhatChange == PLAYER_ENTRANCE)
		{
			send_packet->InRoom_UserNum = RoomPlayerInfoList.size();
			my_info.InRoom_MyNum = RoomPlayerInfoList.size();


			//

			//my_info.iSelectedTeam = PLAYER_TEAM_LINE::A_TEAM_0;
			//
			//for (int idx = 0; idx < RoomPlayerInfoList.size(); ++idx)
			//{
			//	if (RoomPlayerInfoList[idx].iSelectedTeam == my_info.iSelectedTeam)
			//	{
			//		++my_info.iSelectedTeam;
			//	}
			//	else
			//		break;
			//}
			//my_info.packet_manager.cs_room_send_my_info.iSelectedTeam = my_info.iSelectedTeam;

			send_packet->bLoadingCompletion = FALSE;
			my_info.bLoadingCompletion = FALSE;

			send_packet->iSelectedCharacter = CHARACTER_MEI;
			my_info.iSelectedCharacter = CHARACTER_MEI;

		}
		else if (send_packet->WhatChange == PLAYER_CHANGE_TEAM_LINE)
		{
			//보낼 패킷의 방 안에서의 유저 넘버를 내 정보에 있는 넘버로 설정
			send_packet->InRoom_UserNum = my_info.InRoom_MyNum;
			send_packet->iSelectedTeam = my_info.iSelectedTeam;

		}
		else if (send_packet->WhatChange == PLAYER_CHANGE_CHARACTER)
		{
			send_packet->InRoom_UserNum = my_info.InRoom_MyNum;
			send_packet->iSelectedCharacter = my_info.iSelectedCharacter;

		}
		else if (send_packet->WhatChange == PLAYER_READY)
		{
			send_packet->InRoom_UserNum = my_info.InRoom_MyNum;
			//

		}
		else if (send_packet->WhatChange == PLAYER_UN_READY)
		{
			send_packet->InRoom_UserNum = my_info.InRoom_MyNum;
			//
		}
		else if (send_packet->WhatChange == PLAYER_LOADING_COMPLETION)
		{
			send_packet->InRoom_UserNum = my_info.InRoom_MyNum;

			my_info.bLoadingCompletion = TRUE;

			send_packet->bLoadingCompletion = TRUE;
		}
		else if (send_packet->WhatChange == PLAYER_QUIT)
		{
			send_packet->InRoom_UserNum = my_info.InRoom_MyNum;
		}

		strcpy(send_packet->str_id, my_info.UserID);

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		break;
	};
	case CS_ROOM_CHAT:
	{
		break;
	};
	case CS_LOBBY_ROOM_CREATE:
	{
		P_CS_LOBBY_ROOM_CREATE* tmp_packet = (P_CS_LOBBY_ROOM_CREATE*)packet;
		P_CS_LOBBY_ROOM_CREATE* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_LOBBY_ROOM_CREATE *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_LOBBY_ROOM_CREATE));

		send_packet->size = sizeof(P_CS_LOBBY_ROOM_CREATE);
		send_packet->type = CS_LOBBY_ROOM_CREATE;

		send_wsabuf.len = sizeof(P_CS_LOBBY_ROOM_CREATE);

		//현재 얻어온 방 리스트의 사이즈가 다음 넘버링이 된다.
		my_info.InRoomNum = RoomInfoList.size();

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		RoomPlayerListClear();

		//my_info.packet_manager.cs_room_in.type = CS_ROOM_IN;
		////SendRequestQueue.push((char*)&my_info.packet_manager.cs_room_in);
		//SendPacket((char*)&my_info.packet_manager.cs_room_in);

		SetEvent(EV.hGetUserInfoEvent); // 쓰기 완료 알리기

		break;
	};
	case CS_INGAME_REQUIRE_MOVE_SYNC:
	{
		P_CS_INGAME_REQUIRE_MOVE_SYNC* tmp_packet = (P_CS_INGAME_REQUIRE_MOVE_SYNC*)packet;
		P_CS_INGAME_REQUIRE_MOVE_SYNC* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_INGAME_REQUIRE_MOVE_SYNC *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_INGAME_REQUIRE_MOVE_SYNC));

		send_packet->size = sizeof(P_CS_INGAME_REQUIRE_MOVE_SYNC);
		send_packet->type = CS_INGAME_REQUIRE_MOVE_SYNC;
		send_packet->InRoomNum = my_info.InRoomNum;


		send_wsabuf.len = sizeof(P_CS_INGAME_REQUIRE_MOVE_SYNC);

		for (int i = 0; i < RoomPlayerInfoList.size(); ++i)
		{
			//if (RoomPlayerInfoList[i].ply_IsVisible == TRUE)
			{
				send_packet->team_line = RoomPlayerInfoList[i].iSelectedTeam;
				send_wsabuf.len = sizeof(P_CS_INGAME_REQUIRE_MOVE_SYNC);

				WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);
			}
		}


		break;
	};
	case CS_INGAME_MOVE_INPUT_EVENT:
	{
		P_CS_INGAME_MOVE_INPUT_EVENT* tmp_packet = (P_CS_INGAME_MOVE_INPUT_EVENT*)packet;
		P_CS_INGAME_MOVE_INPUT_EVENT* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_INGAME_MOVE_INPUT_EVENT *>(send_buffer);

		std::memcpy(send_packet, tmp_packet, sizeof(P_CS_INGAME_MOVE_INPUT_EVENT));

		float tmp_fResizeSpeed = 1.0f;

		//if (my_info.player_input_info.key_SHIFT_Down == TRUE)
		if (INPUT->KeyDown(YT_KEY::YK_LSHIFT) == TRUE)
			tmp_fResizeSpeed *= 2.0f;

		float tmp_fSpeed = CHARACTER_SPEED * BLOCK_SCALE;

		send_packet->size = sizeof(P_CS_INGAME_MOVE_INPUT_EVENT);
		send_packet->type = CS_INGAME_MOVE_INPUT_EVENT;
		send_packet->InRoomNum = my_info.InRoomNum;
		send_packet->fDistance = tmp_fResizeSpeed * tmp_fSpeed * TIMEMGR->GetTimeElapsed();


		send_wsabuf.len = sizeof(P_CS_INGAME_MOVE_INPUT_EVENT);

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		break;
	};

	case CS_INGAME_ROTATE_EVENT:
	{
		P_CS_INGAME_ROTATE_EVENT* tmp_packet = (P_CS_INGAME_ROTATE_EVENT*)packet;
		P_CS_INGAME_ROTATE_EVENT* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_INGAME_ROTATE_EVENT *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_INGAME_ROTATE_EVENT));

		send_packet->size = sizeof(P_CS_INGAME_ROTATE_EVENT);
		send_packet->type = CS_INGAME_ROTATE_EVENT;
		send_packet->InRoomNum = my_info.InRoomNum;

		send_wsabuf.len = sizeof(P_CS_INGAME_ROTATE_EVENT);

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		break;
	};
	case CS_INGAME_OTHER_INPUT_EVENT:
	{
		P_CS_INGAME_OTHER_INPUT_EVENT* tmp_packet = (P_CS_INGAME_OTHER_INPUT_EVENT*)packet;
		P_CS_INGAME_OTHER_INPUT_EVENT* send_packet;
		//
		send_packet = reinterpret_cast<P_CS_INGAME_OTHER_INPUT_EVENT *>(send_buffer);

		memcpy(send_packet, tmp_packet, sizeof(P_CS_INGAME_OTHER_INPUT_EVENT));

		send_packet->size = sizeof(P_CS_INGAME_OTHER_INPUT_EVENT);
		send_packet->type = CS_INGAME_OTHER_INPUT_EVENT;
		send_packet->InRoomNum = my_info.InRoomNum;

		send_wsabuf.len = sizeof(P_CS_INGAME_OTHER_INPUT_EVENT);

		WSASend(sock, &send_wsabuf, 1, &iobyte, 0, nullptr, nullptr);

		break;
	};

	default:
		cout << "Unknown Client Packet!" << endl;
		break;
	}

}


// 소켓 함수 오류 출력
void TCPClient::err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, nullptr);



	//DisplayText_RoomChat("[%s] %s", msg,  (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void TCPClient::RoomListInit()
{

}

void TCPClient::RoomPlayerListInit()
{

}

void TCPClient::RoomPlayer_And_MyInfo_Init()
{
	RoomPlayerInfoList.clear();
	
	//Number & Process Init
	my_info.bLoadingCompletion = FALSE;
	my_info.bRoomEntranceSuccess = FALSE;
	my_info.InRoomNum = NUMBERING_INIT;
	my_info.InRoom_MyNum = NUMBERING_INIT;
	my_info.object_index = 0;

	//State Init
	my_info.iKillNum = 0;
	my_info.iMonsterKillNum = 0;
	my_info.iScoreNum = 0;
	my_info.iDeathNum = 0;
	
	//Character & Team Line Init
	my_info.iSelectedCharacter = CHARACTER_MEI;
	my_info.iSelectedTeam = A_TEAM_0;
	
	//Input Init
	my_info.player_input_info.key_A_Down = FALSE;
	my_info.player_input_info.key_D_Down = FALSE;
	my_info.player_input_info.key_SHIFT_Down = FALSE;
	my_info.player_input_info.key_SPACE_Down = FALSE;
	my_info.player_input_info.key_S_Down = FALSE;
	my_info.player_input_info.key_W_Down = FALSE;
	my_info.player_input_info.m_fDistance = 0.0f;

	//Position Init
	my_info.pos_x = 0.0f;
	my_info.pos_z = 0.0f;

	//Other Init
	my_info.ply_IsVisible = FALSE;
}


////////////////////////////////////////////////
////////////////////////////////////////////////
