
#pragma once

#define SERVERIP   "127.0.0.1"
#define T_SERVERIP   L"127.0.0.1"

#define SERVERPORT 9000
#define BUFSIZE    512


#define ID_BUF 100
#define PW_BUF 100
#define MESSAGE_BUF 200

#define ROOM_NAME_BUF 100

#define MONSTER_MAXNUM 10

#define MATCH_START_TIME 10

#define LOUNGE_ROOM_NUM 0

enum THROWN_OBJ_TYPE
{
	OBJ_FLASH_BANG = 0,
	OBJ_FAKE_ITEM,
	OBJ_TRAP,
	OBJ_BIND_OBJECT,
	OBJ_STUN_OBJECT
};

////패킷 이벤트 타입 정의
//#define BASIC 11
//#define BROADCAST 12
//
////리스트 절차
//#define SET_ROOMLIST_SIZE 0
//#define SET_ROOMLIST_INFO 1
//#define SET_ROOMS_END 2
//
//#define SET_PLAYERLIST_SIZE 0
//#define SET_PLAYERLIST_INFO 1
//#define SET_PLAYERS_END 2
//#define SEND_MYINFO 3