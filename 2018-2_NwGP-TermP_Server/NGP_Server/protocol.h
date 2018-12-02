#pragma once

#include <windows.h>
#include <chrono>

#pragma pack(push,1)
struct Packet {

	BYTE size;
	short type;
};
//규칙
//SC -> 서버에서 클라 CS-> 클라에서 서버
//Ch -> 캐릭터 Mo -> 중립몬스터, 미니언
struct SC_Msg_Put_Character : public Packet
{
	BYTE Character_id;
	int x;
	int y;
};

struct SC_Msg_Pos_Character : public Packet
{
	BYTE Character_id;
	DWORD dwDirection;
	float timeElapsed;
	int dir;
	int x;
	int y;
};
struct CS_Msg_Pos_Character : public Packet
{
	BYTE Character_id;
	DWORD dwDirection;
	short x;
	short y;
};

struct CS_Msg_Change_State : public Packet
{
	BYTE Character_id;
	BYTE State;
};
struct SC_Msg_Change_State : public Packet
{
	BYTE Character_id;
	BYTE State;
};
struct SC_Msg_Sync : public Packet
{
	BYTE Chracter_id;
	BYTE State;
	int x;
	int y;
};

#pragma pack(pop)

#define MAX_BUFF_SIZE 4000
#define MAX_PACKET_SIZE 255

#define MAP_WIDTH 10000
#define MAP_HEIGHT 10000

#define MAX_USER 4
#define NPC_START 1000
#define NUM_OF_NPC 10000
#define VIEW_RADIUS 500

#define MY_SERVER_PORT 4000
#define NONE -1

//enum class Type : BYTE { Connect, Create, Delete, Move, Attack, Hit };
//Server-> Client
#define SC_CONNECT_PLAYER			  101
#define SC_PUT_PLAYER				  102
#define SC_REMOVE_PLAYER			  103
#define SC_POS_PLAYER				  104
#define SC_CHANGE_STATE				  105
#define SC_SYNC						  106
//Client->Server
#define CS_MOVE						  201
#define CS_CHANGE_STATE				  202








