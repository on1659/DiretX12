
#include "PacketManger.h"

#include "CriticalSection.h"
#include "Event.h"


using namespace std;

// ���� ���� ������ ���� ����ü�� ����
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
};

enum PLYAER_STATE
{
	STATE_NONE = 0,

	STATE_DIE,
	STATE_BINDED,
	STATE_STUNED
};

struct PLAYER_INPUT_INFO {


	BOOL key_W_Down = FALSE;
	BOOL key_A_Down = FALSE;
	BOOL key_S_Down = FALSE;
	BOOL key_D_Down = FALSE;

	BOOL key_SPACE_Down = FALSE;
	BOOL key_SHIFT_Down = FALSE;


	float m_fDistance;
};


struct PLAYER_INFO 
{

	//UINT UserState;
	volatile UINT sock_num;
	volatile UINT cli_num;

	volatile UINT InRoomNum;
	volatile UINT InRoom_MyNum;

	//-----------------------------
	char UserID[ID_BUF + 1];

	volatile unsigned int iKillNum = 0;
	volatile unsigned int iDeathNum= 0 ;
	volatile unsigned int iMonsterKillNum= 0;
	volatile unsigned int iScoreNum = 0;
	//-----------------------------

	volatile BOOL bRoomEntranceSuccess;
	volatile BOOL bLoadingCompletion;

	volatile UINT iSelectedCharacter;
	volatile UINT iSelectedTeam;
	
	volatile BOOL ply_IsVisible;

	volatile int object_index;

	PLAYER_INPUT_INFO player_input_info;

	float pos_x;
	float pos_z;

	//PACKET_MANAGER packet_manager;
};

class TCPClient : public CSingleTonBase<TCPClient>
{
private:

	vector<ROOMINFO> RoomInfoList;

	//JJY '16.08.16
	/*
		�߰� ���� : RoomPlayerInfoList�� �� �ڽŵ� ����
	*/
	vector<PLAYER_INFO> RoomPlayerInfoList;

	UINT NextRoomNum = 0;
	//UINT NextUserNum = 0;

public:

	int nTotalSockets = 0;
	SOCKETINFO *SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];

	char ip_address[30];

	PLAYER_INFO my_info;

	Event EV;

	//queue<char*> SendRequestQueue;

	//BOOL LoginCheck = FALSE;
	//BOOL EnterRoomCheck = FALSE;

	TCPClient();
	virtual ~TCPClient();

	 SOCKET volatile sock; // ����
						  //char buf[BUFSIZE+1]; // ������ �ۼ��� ����

	void ReadPacket(SOCKET sock);
	void ProcessPacket(char* packet_inf);
	void SendPacket(unsigned char* packet);


	// ���� ��� �Լ�
	void err_quit(char *msg);
	void err_display(char *msg);

	// ����� ���� ������ ���� �Լ�
	int recvn(SOCKET s, char *buf, int len, int flags);

	// ���� ��� ������ �Լ�
	static DWORD WINAPI ClientMain(LPVOID arg);

	//////
	/*
	volatile Ű����� ���ÿ� ���� ���� ���� �����忡 ���� �ʵ尡 ������ �� ������ ��Ÿ���ϴ�.
	volatile�� ����� �ʵ忡�� ���� �����带 ���� �׼����� ������ �ϴ� �����Ϸ� ����ȭ�� ������� �ʽ��ϴ�.
	�̷��� �ϸ� �ʵ��� ���� �׻� �ֽ� ���·� ������ �� �ֽ��ϴ�.
	�Ϲ������� volatile �����ڴ� �׼����� serialize�� �� lock ���� ������� �ʰ� ���� �����忡�� �׼����ϴ� �ʵ忡 ���˴ϴ�
	*/
	 BOOL volatile g_bStart; // ��� ���� ����
							//static volatile BOOL g_bStart; // ��� ���� ����

	void RoomPlayerListInit();
	void RoomListInit();

	void RoomPlayerListClear() { RoomPlayerInfoList.clear(); }
	void RoomListClear() { RoomInfoList.clear(); }


	size_t GetRoomInfoListSize() const { return RoomInfoList.size(); }
	ROOMINFO GetRoomInfo(UINT idx) { return RoomInfoList[idx]; }

	size_t GetRoomPlayerInfoListSize() const { return RoomPlayerInfoList.size(); }
	PLAYER_INFO GetRoomPlayerInfo(UINT idx) { return RoomPlayerInfoList[idx]; }

	void RoomPlayer_And_MyInfo_Init();

	////////////////////////

	// ���� ���� �Լ�
	BOOL AddSocketInfo(SOCKET sock);
	void RemoveSocketInfo(int nIndex);

	/////////////////////////////////////
	/////////////////////////////////////
};