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
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
#ifdef USE_CONSOLE_WINDOW
		printf("이상 무");
#endif
	}

	// socket()
	m_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
#ifdef USE_CONSOLE_WINDOW
	if (m_mysocket == INVALID_SOCKET) printf("소켓생성실패");
#endif

	WSAAsyncSelect(m_mysocket, m_hWnd, WM_SOCKET, FD_READ);

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = WSAConnect(m_mysocket, (sockaddr *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);
	
	
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
	case SC_SET_HP_SCORE:
	{
#ifdef USE_CONSOLE_WINDOW
		printf("여기 들어오긴 하나요?\n");
#endif
		SC_Msg_Set_HP_Score *my_packet = reinterpret_cast<SC_Msg_Set_HP_Score *>(ptr);

		m_ppPlayer[my_packet->Character_id]->SetHP(my_packet->hp);
		m_ppPlayer[my_packet->HIT_id]->SetScore(my_packet->score);
#ifdef USE_CONSOLE_WINDOW
		printf("%d번 플레이어 체력 : %d %d번 플레이어 점수 : %d\n", my_packet->Character_id, my_packet->hp
			, my_packet->HIT_id, my_packet->score);
#endif
		break;
	}
	case SC_PUT_PLAYER:
	{
		SC_Msg_Put_Character *my_packet = reinterpret_cast<SC_Msg_Put_Character *>(ptr);
		int id = my_packet->Character_id;
		printf("%d\n", id);
		if (m_myid == NONE) {
			m_myid = id;
			m_idIsSet = true;
			m_pFramework->BuildScene();
			m_pFramework->ChangeScene(CBaseScene::SceneTag::Title);
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
#ifdef USE_CONSOLE_WINDOW
		printf("%d번 플레이어 체력 : %d\n", my_packet->Character_id, my_packet->hp);
#endif
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
	case CS_LOGIN_ID:
	{
		CS_Msg_Demand_LoginID *my_packet = reinterpret_cast<CS_Msg_Demand_LoginID *>(ptr);
		m_pFramework->ChangeScene(CBaseScene::SceneTag::Main);

		break;
	}
	
	
//	case SC_SYNC:
//	{
//		SC_Msg_Sync *my_packet = reinterpret_cast<SC_Msg_Sync *>(ptr);
//#ifdef USE_CONSOLE_WINDOW
//		printf("%d번 플레이어 체력 : %d\n", my_packet->Chracter_id, my_packet->hp);
//#endif
//		m_ppPlayer[my_packet->Chracter_id]->SetHP(max(my_packet->hp,0));
//		//m_ppPlayer[my_packet->Chracter_id]->SetPosition(my_packet->x, my_packet->y);
//		//m_ppPlayer[my_packet->Chracter_id]->SetState((ObjectState)my_packet->State);
//		break;
//	}
	default:
#ifdef USE_CONSOLE_WINDOW
		printf("Unknown PACKET type [%d]\n", ptr[1]);
#endif
		break;
	}
	

}

int ReturnTypeNumber(SOCKET& clientSock) {
	int type, retVal;

	retVal = recv(clientSock, (char*)&type, sizeof(type), 0);

	return type;
}

//
void CNetwork::ReadPacket(SOCKET sock)
{
	
	int ret = recv(sock, m_buffer, sizeof(m_buffer), 0);
	if (ret > 0) {
		ProcessPacket(m_buffer);
	}

}

void CNetwork::SendPacket(void* ptr)
{
	char *packet = reinterpret_cast<char *>(ptr);
	
	int res = send(m_mysocket, packet, sizeof(packet) + sizeof(int), 0);
}

