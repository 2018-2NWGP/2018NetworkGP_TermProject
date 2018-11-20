#include "stdafx.h"
#include "Network.h"

CNetwork::CNetwork()
{
}


CNetwork::~CNetwork()
{
}

bool CNetwork::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	int retval{ 0 };
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("이상 무");

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) printf("소켓생성실패");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) printf("connect 실패");
	else
		printf("connect 성공");
	//WSAAsyncSelect(m_mysocket, m_hWnd, WM_SOCKET, FD_READ);
	return true;
}

void CNetwork::Finalize()
{
	if (m_mysocket)
	{
		closesocket(m_mysocket);
		m_mysocket = NULL;
	}
}

void CNetwork::ProcessPacket(char *ptr)
{
	if (ptr[1] != 0) {
		switch (ptr[1])
		{
		case SC_PUT_PLAYER:
		{
			SC_Msg_Put_Character *my_packet = reinterpret_cast<SC_Msg_Put_Character *>(ptr);
			int id = my_packet->Character_id;
			if (m_myid == NONE) {
				m_myid = id;
			}
			break;
		}
		//case SC_POS:
		//{
		//	if (!m_gameLoaded) break;

		//	SC_Msg_Pos_Character *my_packet = reinterpret_cast<SC_Msg_Pos_Character *>(ptr);
		//	int id = my_packet->Character_id;
		//	if (id == m_myid) {
		//		if (m_ppPlayer[id]->GetUpdateTime() <= my_packet->updatetime)
		//		{
		//			m_ppPlayer[id]->SetPosition(my_packet->x, my_packet->y);
		//			//m_ppPlayer[id]->SyncAnimation((AnimationsType)my_packet->state, my_packet->frameTime);
		//			m_ppPlayer[id]->SetUpdateTime(my_packet->updatetime);
		//			m_ppPlayer[id]->SetLevel(my_packet->level, my_packet->maxexp, my_packet->exp);
		//		}
		//	}
		//	else if (id < NPC_START) {
		//		if (m_ppPlayer[id]->GetUpdateTime() <= my_packet->updatetime)
		//		{
		//			m_ppPlayer[id]->SetPosition(my_packet->x, my_packet->y);
		//			//m_ppPlayer[id]->SyncAnimation((AnimationsType)my_packet->state, my_packet->frameTime);
		//			m_ppPlayer[id]->SetUpdateTime(my_packet->updatetime);
		//			m_ppPlayer[id]->SetLevel(my_packet->level, my_packet->maxexp, my_packet->exp);
		//		}
		//	}
		//	break;
		//}
		
		default:
			printf("Unknown PACKET type [%d]\n", ptr[1]);
			break;
		}
	}

}


//
void CNetwork::ReadPacket()
{
	DWORD ioflag = 0;
	DWORD iobyte = 0;
	int errorcount = 0;

	int ret = recv(m_mysocket, m_buffer, sizeof(m_buffer), 0);
	ProcessPacket(m_buffer);
	/*BYTE *ptr = reinterpret_cast<BYTE *>(m_recv_buffer);

	while (0 != iobyte) {
		if (0 == m_in_packet_size) m_in_packet_size = ptr[0];
		if (iobyte + m_saved_packet_size >= m_in_packet_size) {
			memcpy(m_packet_buffer + m_saved_packet_size, ptr, m_in_packet_size - m_saved_packet_size);
			ProcessPacket(m_packet_buffer);
			ptr += m_in_packet_size - m_saved_packet_size;
			iobyte -= m_in_packet_size - m_saved_packet_size;
			m_in_packet_size = 0;
			m_saved_packet_size = 0;
		}
		else {
			memcpy(m_packet_buffer + m_saved_packet_size, ptr, iobyte);
			m_saved_packet_size += iobyte;
			iobyte = 0;
		}

	}*/


}

void CNetwork::SendPacket(void* ptr)
{
	char *packet = reinterpret_cast<char *>(ptr);
	
	int res = send(m_mysocket, packet, sizeof(packet), 0);

}

