#pragma once


#define WIN32_LEAN_AND_MEAN  
#define INITGUID
#pragma comment(lib, "ws2_32.lib")
// include important windows stuff
#include <windows.h> 
#include <Winsock2.h>

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


//void NetworkInitialize();
//
//void StartRecv(int id);
//
//
//void SendPacket(int id, void *ptr);
//void SendPacketBySocket(SOCKET sock, void *ptr);
//
//void SendRemovePacket(int client, int object);
//
//void ProcessPacket(int id, char *packet);
//
//void DisconnectPlayer(int id);
//
//void accept_thread();	//새로 접속해 오는 클라이언트를 IOCP로 넘기는 역할
//

