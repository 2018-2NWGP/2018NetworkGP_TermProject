#include <windows.h>

#pragma once
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
};

struct SC_Msg_Pos_Character : public Packet
{
	BYTE Character_id;
	int x;
	int y;
};
struct CS_Msg_Pos_Character : public Packet
{
	BYTE Character_id;
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

//Client->Server
#define CS_MOVE_RIGHT				  201
#define CS_MOVE_UP					  202
#define CS_MOVE_DOWN				  203
#define CS_MOVE_LEFT				  204








