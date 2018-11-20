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

class Client {
public:
	SOCKET m_s;
	bool m_isconnected;
	float m_x;
	float m_y;
	Client()
	{
		m_isconnected = false;
		m_x = 0;
		m_y = 0;
	}
};


// 네크워크 함수

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

