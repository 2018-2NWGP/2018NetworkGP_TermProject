#pragma once


#define WIN32_LEAN_AND_MEAN  
#define INITGUID
#pragma comment(lib, "ws2_32.lib")
// include important windows stuff
#include <Winsock2.h>
#include <windows.h> 


//참조 헤더
#include <array>
#include <iostream>

//사용자 정의 헤더
#include "protocol.h"
using namespace std;
struct EXOVER {
	WSAOVERLAPPED m_over;
	char m_iobuf[MAX_BUFF_SIZE];
	WSABUF m_wsabuf;
	bool is_recv;
};
class Client {
public:
	SOCKET m_s;
	bool m_isconnected;
	int m_x;
	int m_y;
	int m_state;
	int m_hp;
	char* m_buffer;
	EXOVER m_rxover;
	Client()
	{
		m_isconnected = false;
		m_x = 0;
		m_y = 0;
		ZeroMemory(&m_rxover.m_over, sizeof(WSAOVERLAPPED));
		m_rxover.m_wsabuf.buf = m_rxover.m_iobuf;
		m_rxover.m_wsabuf.len = sizeof(m_rxover.m_wsabuf.buf);
		m_rxover.is_recv = true;
	}
};


//////////////////////////////////////////////////////////////
// 전역 변수

//class CPlayerObject;
//class CMainScene;

extern array <Client, MAX_USER> g_clients;

//extern PlayerObject* g_Player;
//extern PlayerObject** g_ppPlayer;
//extern CMainScene* g_pScene;

//std::chrono::duration<double> g_timeElapsed;
//std::chrono::system_clock::time_point g_current_time;
//CRITICAL_SECTION UserDataUpdateSection;
//////////////////////////////////////////////////////////////

void SendPacket(int id, void *ptr);

int recvn(SOCKET s, char *buf, int len, int flags);

int ReadPacket(SOCKET sock, char* packet);

int ReturnTypeNumber(SOCKET& clientSock);









