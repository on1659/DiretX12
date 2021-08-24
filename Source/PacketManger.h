
/*
패킷 type define은
어느방향_어디상태_요청내용
으로 규정
*/

#ifndef _DIRECTX_HEADER_
#define _DIRECTX_HEADER_

//#include <d3d11.h>
//#include <d3dx11.h>
#include <D3DX10Math.h>
//#include <d3dcompiler.h>

#include <DirectXMath.h>
//#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#endif

#include "CommClientDefine.h"

////////////////////////////

#define NUMBERING_INIT 99

enum GAME_STATE {
	S_STANDBY = 0,
	S_LOGIN,
	S_LOBBY,
	S_ROOM_CREATE,
	S_ROOM,
	S_MATCH_STANDBY,
	S_INGAME,
	S_RESULT
};

struct PushMessage
{
	char str_id[ID_BUF + 1];
	char message[MESSAGE_BUF + 1];
};

struct ROOMINFO
{
	WORD room_num;
	//WORD Current_RoomUserCount = 0;
	char room_name[ROOM_NAME_BUF + 1];

	WORD game_mode = 0;
	WORD max_player = 0;

	volatile bool bIsAlive = true;
	volatile bool bIsMatchStart = false;
	volatile unsigned int match_play_time = MATCH_START_TIME;

	volatile unsigned int thrown_object_num = 0;
};

////////////////////////////

//플레이어가 방에서 뭘 변화 했는지
enum PLAYER_WHAT_CHANGE {

	PLAYER_ENTRANCE = 0,
	PLAYER_QUIT,
	PLAYER_READY,
	PLAYER_UN_READY,
	PLAYER_LOADING_COMPLETION,
	PLAYER_CHANGE_CHARACTER,
	PLAYER_CHANGE_TEAM_LINE
};

//플레이어의 팀
enum PLAYER_TEAM_LINE {
	A_TEAM_0 = 0,
	A_TEAM_1,
	B_TEAM_0,
	B_TEAM_1
};

//플레이어의 캐릭터
enum PLAYER_CHARACTER {
	CHARACTER_MEI = 0,
	CHARACTER_LIZZY
};

//게임 모드
enum GAME_PLAY_MODE {

	GAME_PLAY_MODE_BASIC = 0,
	GAME_PLAY_MODE_OCCUPY,
	GAME_PLAY_MODE_SCORE
};

//게임 팀 사이즈
enum GAME_PLAY_TEAM_SIZE {

	GAME_PLAY_INDIVIDUAL = 2,
	GAME_PLAY_TEAMWORK = 4
};

enum OBJECT_TYPE {

	OBJ_PLAYER = 0,
	OBJ_MONSTER
};

enum BULLETIN_BOARD_WHAT_CHANGE
{
	BB_KILL = 0,
	BB_DEATH,
	BB_MONSTER_KILL,
	BB_SCORE
};

enum SPECIAL_EFFECT_TYPE
{
	SE_MISS = 0
};

//////////////////////////////////////
//			패킷 프로토콜			//
//////////////////////////////////////

enum PACKET_PROTOCOL
{
	//CLIENT

	CS_LOGIN_REQUEST = 1,
	CS_LOBBY_IN,
	CS_LOBBY_REQUEST,
	CS_LOBBY_ROOM_CREATE,
	CS_ROOM_IN,
	CS_ROOM_USER_INFO_REQUEST,
	CS_ROOM_SEND_MY_INFO,
	CS_ROOM_CHAT,
	CS_MATCH_STANDBY_LOAD_STATE,
	CS_INGAME_REQUIRE_MOVE_SYNC,
	CS_INGAME_MOVE_INPUT_EVENT,
	CS_INGAME_OTHER_INPUT_EVENT,
	CS_INGAME_ROTATE_EVENT,

	//SERVER

	SC_LOGIN_REQUEST_RETURN,
	SC_LOBBY_ROOMLIST_SIZE,
	SC_LOBBY_ROOMLIST_INFO,
	SC_LOBBY_ROOMLIST_INFO_UPDATE,
	SC_ROOM_USERLIST_SIZE,
	SC_ROOM_USERLIST_INFO,
	SC_ROOM_USERLIST_INFO_UPDATE,
	SC_ROOM_CHAT,
	SC_MATCH_STANDBY_CLIENT_STATE,
	SC_INGAME_RETURN_MOVE_SYNC,

	SC_INGAME_MOVE_INPUT_RETURN,
	SC_INGAME_OTHER_INPUT_RETURN,

	SC_INGAME_ROTATE_RETURN,
	SC_INGAME_VISIBLE_PLAYER_SET,
	SC_INGAME_INVISIBLE_PLAYER_SET,
	SC_INGAME_MONSTER_INFO,
	SC_INGAME_VISIBLE_MONSTER_SET,
	SC_INGAME_INVISIBLE_MONSTER_SET,
	SC_INGAME_PLAYER_STATE_INFO,
	SC_INGAME_ITEM_INFO,
	SC_INGAME_THROWN_OBJ_INFO,

	SC_INGAME_BULLETIN_BOARD_UPDATE,
	SC_INGAME_SPECIAL_EFFECT_INFO,
	SC_INGAME_MATCH_END
};


#pragma pack (push, 1)

////////////이벤트성 패킷
//------클라이언트 송신 패킷 데이터
//Login

struct P_CS_LOGIN_REQUEST
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD check = 0;
	//float data;

	char str_id[ID_BUF + 1];
};
//Lobby

struct P_CS_LOBBY_IN
{
	BYTE size;
	PACKET_PROTOCOL type;

	//WORD check = 0;
};


struct P_CS_LOBBY_REQUEST
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD Request_RoomNum;
};


struct P_CS_LOBBY_ROOM_CREATE
{
	BYTE size;
	PACKET_PROTOCOL type;

	ROOMINFO room_info;
};


//Room

struct P_CS_ROOM_IN
{
	BYTE size;
	PACKET_PROTOCOL type;

	//WORD check = 0;
	WORD InRoomNum;
};


struct P_CS_ROOM_USER_INFO_REQUEST
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD Request_RoomNum;
	WORD Request_UserNum;
};



struct P_CS_ROOM_SEND_MY_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD WhatChange;

	BOOL bReady;
	BOOL bLoadingCompletion;

	WORD InRoomNum;
	WORD InRoom_UserNum;

	WORD iSelectedCharacter;
	WORD iSelectedTeam;

	char str_id[ID_BUF + 1];
};


struct P_CS_ROOM_CHAT
{
	BYTE size;
	PACKET_PROTOCOL type;

	bool InHere;

	WORD InRoomNum;
	PushMessage push_message;
	//string recv_message;
};

//Match_Standby


struct P_CS_MATCH_STANDBY_LOAD_STATE
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD InRoom_UserNum;
	BOOL isLoadCompletion;
};



//Ingame


struct P_CS_INGAME_REQUIRE_MOVE_SYNC
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD InRoomNum;
	WORD team_line;
};


struct P_CS_INGAME_MOVE_INPUT_EVENT
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD InRoomNum;

	WORD keytype;
	BOOL is_keydown;
	float fDistance;
};

struct P_CS_INGAME_OTHER_INPUT_EVENT
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD InRoomNum;

	WORD keytype;
	BOOL is_keydown;
};

struct P_CS_INGAME_ROTATE_EVENT
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD InRoomNum;

	float cxDelta;
	float cyDelta;
};

//Result


//------서버 송신 패킷 데이터
//Login

struct P_SC_LOGIN_REQUEST_RETURN
{
	BYTE size;
	PACKET_PROTOCOL type;

	BOOL approve;
};

//Lobby

struct P_SC_LOBBY_ROOMLIST_SIZE
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD roomlist_size;
};


struct P_SC_LOBBY_ROOMLIST_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	ROOMINFO room_info;
};


struct P_SC_LOBBY_ROOMLIST_INFO_UPDATE
{
	BYTE size;
	PACKET_PROTOCOL type;

	ROOMINFO room_info;
};

//Room

struct P_SC_ROOM_USERLIST_SIZE
{
	BYTE size;
	PACKET_PROTOCOL type;

	BOOL bRoomEntranceApprove;
	WORD userlist_size;
};


struct P_SC_ROOM_USERLIST_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD UserNum;

	WORD iSelectedCharacter;
	WORD iSelectedTeam;

	char str_id[ID_BUF + 1];
};


struct P_SC_ROOM_USERLIST_INFO_UPDATE
{
	BYTE size;
	PACKET_PROTOCOL type;

	BOOL bAllPlayerReady;
	BOOL bAllPlayerLoadingCompletion;


	BOOL bLoadingCompletion;

	WORD WhatChange;
	WORD InRoom_UserNum;

	WORD iSelectedCharacter;
	WORD iSelectedTeam;

	char str_id[ID_BUF + 1];
};

struct P_SC_ROOM_CHAT
{
	BYTE size;
	PACKET_PROTOCOL type;
	//string send_message;

	PushMessage push_message;

	//char broadcast_message[MESSAGE_BUF + 1];

	//int new_state;
};

//Match_Standby


struct P_SC_MATCH_STANDBY_CLIENT_STATE
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD InRoom_UserNum;
	BOOL isLoadCompletion;
};

//Ingame


struct P_SC_INGAME_RETURN_MOVE_SYNC
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD team_line;

	float pos_x;
	float pos_z;
};


struct P_SC_INGAME_MOVE_INPUT_RETURN
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD team_line;

	WORD keytype;
	BOOL is_keydown;
	float fDistance;
};

struct P_SC_INGAME_OTHER_INPUT_RETURN
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD team_line;

	WORD keytype;
	BOOL is_keydown;
};

struct P_SC_INGAME_ROTATE_RETURN
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD team_line;

	float cxDelta;
	float cyDelta;
};

struct P_SC_INGAME_VISIBLE_PLAYER_SET
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD team_line;

	BOOL W_Iskeydown;
	BOOL A_Iskeydown;
	BOOL S_Iskeydown;
	BOOL D_Iskeydown;

	//float cxDelta;
	//float cyDelta;

	float pos_x;
	float pos_z;
};

struct P_SC_INGAME_INVISIBLE_PLAYER_SET
{
	BYTE size;
	PACKET_PROTOCOL type;

	BOOL IsHide;
	WORD team_line;
};

struct P_SC_INGAME_MONSTER_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD monster_idx;

	float pos_x;
	float pos_z;
};

struct P_SC_INGAME_VISIBLE_MONSTER_SET
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD monster_index;
};

struct P_SC_INGAME_INVISIBLE_MONSTER_SET
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD monster_index;
};

struct P_SC_INGAME_PLAYER_STATE_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD team_line;
	WORD state;
};

struct P_SC_INGAME_ITEM_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD instance_id;
	WORD get_player_team_line;
};

struct P_SC_INGAME_THROWN_OBJ_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD obj_type;
	WORD team_line;

	XMFLOAT3 pos;
	XMVECTOR look;
};

struct P_SC_INGAME_BULLETIN_BOARD_UPDATE
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD team_line;
	WORD WhatUpdate;

	WORD UpdateContent;
};

struct P_SC_INGAME_MATCH_END
{
	BYTE size;
	PACKET_PROTOCOL type;
};

struct P_SC_INGAME_SPECIAL_EFFECT_INFO
{
	BYTE size;
	PACKET_PROTOCOL type;

	WORD effect_type;
	WORD team_line;

};

//Result


////////////지속처리 패킷



//////////

#pragma pack (pop)