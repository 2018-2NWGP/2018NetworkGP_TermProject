#include "stdafx.h"
#include "Network.h"
#include "Framework.h"
#include "Object1-PlayerObject.h"

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
	m_mysocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_mysocket == INVALID_SOCKET) printf("소켓생성실패");

	WSAAsyncSelect(m_mysocket, m_hWnd, WM_SOCKET, FD_READ);

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_mysocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	
	
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
	SC_Msg_Put_Character *my_packet = reinterpret_cast<SC_Msg_Put_Character *>(ptr);
	SC_Msg_Pos_Character *my_Movepacket = reinterpret_cast<SC_Msg_Pos_Character *>(ptr);
	switch (my_packet->type)
	{
	case SC_PUT_PLAYER:
	{
		SC_Msg_Put_Character *my_packet = reinterpret_cast<SC_Msg_Put_Character *>(ptr);
		int id = my_packet->Character_id;
		printf("%d\n", id);
		if (m_myid == NONE) {
			m_myid = id;
			m_idIsSet = true;
			m_pFramework->BuildScene();
			m_pFramework->ChangeScene(CBaseScene::SceneTag::Main);
			//printf("id is set");
		}
		if (m_myid != id) {
			m_ppPlayer[id]->SetPosition(my_packet->x, my_packet->y);
		}
		break;
	}
	case SC_POS_PLAYER:
	{
		SC_Msg_Pos_Character *my_packet = reinterpret_cast<SC_Msg_Pos_Character *>(ptr);

		m_ppPlayer[my_packet->Character_id]->SetDirectionBit(my_packet->dwDirection);
		m_ppPlayer[my_packet->Character_id]->SetState(walking);
		m_ppPlayer[my_packet->Character_id]->SetPosition(my_packet->x, my_packet->y);
		m_ppPlayer[my_packet->Character_id]->Update(my_packet->timeElapsed);
		break;
	}
	case SC_CHANGE_STATE:
	{
		SC_Msg_Change_State *my_packet = reinterpret_cast<SC_Msg_Change_State *>(ptr);

		m_ppPlayer[my_packet->Character_id]->SetState((ObjectState)my_packet->State);
		break;
	}
	
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
		break;
	}
	

}


//
void CNetwork::ReadPacket()
{
	int ret = recv(m_mysocket, m_buffer, sizeof(m_buffer), 0);
	if (ret > 0) {
		ProcessPacket(m_buffer);
	}

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
	
	int res = send(m_mysocket, packet, sizeof(packet) + sizeof(int), 0);
}

