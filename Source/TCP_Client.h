
#include "PacketManger.h"

#include "CriticalSection.h"
#include "Event.h"


using namespace std;

// 소켓 정보 저장을 위한 구조체와 변수
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
		추가 설명 : RoomPlayerInfoList에 나 자신도 들어간다
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

	 SOCKET volatile sock; // 소켓
						  //char buf[BUFSIZE+1]; // 데이터 송수신 버퍼

	void ReadPacket(SOCKET sock);
	void ProcessPacket(char* packet_inf);
	void SendPacket(unsigned char* packet);


	// 오류 출력 함수
	void err_quit(char *msg);
	void err_display(char *msg);

	// 사용자 정의 데이터 수신 함수
	int recvn(SOCKET s, char *buf, int len, int flags);

	// 소켓 통신 스레드 함수
	static DWORD WINAPI ClientMain(LPVOID arg);

	//////
	/*
	volatile 키워드는 동시에 실행 중인 여러 스레드에 의해 필드가 수정될 수 있음을 나타냅니다.
	volatile로 선언된 필드에는 단일 스레드를 통한 액세스를 전제로 하는 컴파일러 최적화가 적용되지 않습니다.
	이렇게 하면 필드의 값을 항상 최신 상태로 유지할 수 있습니다.
	일반적으로 volatile 한정자는 액세스를 serialize할 때 lock 문을 사용하지 않고 여러 스레드에서 액세스하는 필드에 사용됩니다
	*/
	 BOOL volatile g_bStart; // 통신 시작 여부
							//static volatile BOOL g_bStart; // 통신 시작 여부

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

	// 소켓 관리 함수
	BOOL AddSocketInfo(SOCKET sock);
	void RemoveSocketInfo(int nIndex);

	/////////////////////////////////////
	/////////////////////////////////////
};