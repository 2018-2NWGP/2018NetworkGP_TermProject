#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include "stdafx.h"
#include "NGP_Server.h"
#include "Object1-PlayerObject.h"
#include "Scene1-MainGameScene.h"

#define SERVERPORT 9000
#define BUFSIZE    512
#define THREADCNT 3

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// 편집 컨트롤 출력 함수
void DisplayText(char *fmt, ...);
// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);
// 소켓 통신 스레드 함수
DWORD WINAPI ServerMain(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);

HINSTANCE hInst; // 인스턴스 핸들
HWND hEdit; // 편집 컨트롤
CRITICAL_SECTION cs; // 임계 영역

//////////////////////////////////////////////////////////////
// 전역 변수
array <Client, MAX_USER> g_clients;

PlayerObject* g_Player;
PlayerObject** g_ppPlayer;
CMainScene* g_pScene;

std::chrono::duration<double> g_timeElapsed;
std::chrono::system_clock::time_point g_current_time;
//////////////////////////////////////////////////////////////

void SendPacket(int id, void *ptr)
{
	char *packet = reinterpret_cast<char *>(ptr);
	int res = send(g_clients[id].m_s, packet, sizeof(packet), 0);
	if (0 != res) {
		int err_no = WSAGetLastError();
	}
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int ReadPacket(SOCKET sock, char* packet)
{

	int ret = recvn(sock, packet, sizeof(packet), 0);
	return ret;

}

int ReturnTypeNumber(SOCKET& clientSock) {
	int type, retVal;

	retVal = recv(clientSock, (char*)&type, sizeof(type), 0);
	
	return type;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	InitializeCriticalSection(&cs);

	// 윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "MyWndClass";
	if (!RegisterClass(&wndclass)) return 1;

	// 윈도우 생성
	HWND hWnd = CreateWindow("MyWndClass", "TCP 서버", WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return 1;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 소켓 통신 스레드 생성
	CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);

	// 메시지 루프
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteCriticalSection(&cs);
	return msg.wParam;
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		hEdit = CreateWindow("edit", NULL,
			WS_CHILD | WS_VISIBLE |
			 ES_MULTILINE,
			0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 편집 컨트롤 출력 함수
void DisplayText(char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char cbuf[BUFSIZE + 256];
	vsprintf(cbuf, fmt, arg);

	EnterCriticalSection(&cs);
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);

	va_end(arg);
}

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	DisplayText("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// TCP 서버 시작 부분
DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;
	g_current_time = std::chrono::system_clock::now();

	if (!g_ppPlayer) {
		g_ppPlayer = new PlayerObject*[MAX_USER];
		for (int i = 0; i < MAX_USER; ++i)
		{
			g_ppPlayer[i] = new PlayerObject();
			g_ppPlayer[i]->SetPosition(800 + (i * 100), 600);
			g_ppPlayer[i]->SetSize(32, 64);
			g_ppPlayer[i]->SetBackgroundSize(4800, 3200);
		}
	}

	g_pScene = new CMainScene();
	
	g_pScene->BuildObjects();
	g_pScene->SetPlayer(g_ppPlayer);
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread[THREADCNT]{ 0 };

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		//소켓 타임아웃 설정
		DWORD recvTimeout = 5000;  // 5초.
		setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout));

		//// 접속한 클라이언트 정보 출력
		DisplayText("\r\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\r\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		int id = -1;
		for (int i = 0; i < MAX_USER; ++i)
			if (false == g_clients[i].m_isconnected) {
				id = i;
				break;
			}
		if (-1 == id) {
			cout << "MAX USER Exceeded\n";
		}
		cout << "ID of new Client is [" << id << "]\n";
		g_clients[id].m_s = client_sock;
		//clear for reuse

		g_clients[id].m_isconnected = true;
		g_clients[id].m_x = g_ppPlayer[id]->GetPosition().x;
		g_clients[id].m_y = g_ppPlayer[id]->GetPosition().y;
		//StartRecv(id);

		SC_Msg_Put_Character p;
		p.Character_id = id;
		p.size = sizeof(p);
		p.type = SC_PUT_PLAYER;
		SendPacket(id, &p);
		//DisplayText("%d", p.type);

		//for (int i = 0; i < MAX_USER; ++i)
		//{
		//	SC_Msg_Put_Character p;
		//	p.Character_id = i;
		//	p.x = g_clients[i].m_x;
		//	p.y = g_clients[i].m_y;
		//	if(id != i)
		//		SendPacket(id, &p);
		//}

		// 스레드 생성
		for (int i = 0; i < THREADCNT; ++i)
		{
			if (!hThread[i]) {
				hThread[i] = CreateThread(NULL, 0, ProcessClient,
					(LPVOID)g_clients[id].m_s, 0, NULL);
				break;
			}
			//if (hThread == NULL) { closesocket(client_sock); }
			//else { CloseHandle(hThread); }
		}
	}

	// closesocket()
	closesocket(listen_sock);
	delete g_Player;
	// 윈속 종료
	WSACleanup();
	return 0;
}

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	DisplayText("스레드 등록\n");
	std::chrono::system_clock::time_point current_time;
	std::chrono::duration<double> timeElapsed;
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	int sync{ NONE };
	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	int recvType{ 0 };
	while (true) {
		sync++;
		//50ms마다 한번씩 위치 좌표 보내줌
		if (sync > 50) {
			SC_Msg_Sync p;
			//p.Character_id = 0;
			p.size = sizeof(p);
			p.type = SC_SYNC;
			for (int i = 0; i < MAX_USER; ++i) {
				if (g_clients[i].m_isconnected) {
					p.State = g_ppPlayer[i]->GetState();
					p.x = g_ppPlayer[i]->GetPosition().x;
					p.y = g_ppPlayer[i]->GetPosition().y;
					send(g_clients[i].m_s, (char*)&p, sizeof(p), 0);
				}
			}
			sync = 0;
		}
		current_time = std::chrono::system_clock::now();
		timeElapsed = std::chrono::system_clock::now() - current_time;
		if (timeElapsed.count() > MAX_FRAMETIME)
		{
			for(int i = 0; i<MAX_USER; ++i)
				g_ppPlayer[i]->Update(timeElapsed.count());
			g_pScene->Update(timeElapsed.count());
		}

		recvType = ReturnTypeNumber(client_sock);
		
		//ProcessPacket(recvType);
		int type, retVal{ 0 };
		int size{ 0 };
		if (recvType == CS_MOVE) {
			CS_Msg_Pos_Character temp;
			retVal = recv(client_sock, (char*)&temp, sizeof(temp)+sizeof(int), 0);
			if (retVal == SOCKET_ERROR) printf("recv() Miss!\n");
			
			DWORD dwDirection = temp.dwDirection;
			DisplayText("%d,%d,%d,%d\n", temp.Character_id, temp.type, temp.x, temp.y);
			timeElapsed = std::chrono::system_clock::now() - current_time;
			if (timeElapsed.count() > MAX_FRAMETIME)
			{
				//dwDirection |= DIR_RIGHT;
				g_ppPlayer[temp.Character_id]->SetDirection(dwDirection);
				//g_Player->Update(g_timeElapsed.count());

			}
			
			g_clients[temp.Character_id].m_x = g_ppPlayer[temp.Character_id]->GetPosition().x;
			g_clients[temp.Character_id].m_y = g_ppPlayer[temp.Character_id]->GetPosition().y;
			
			SC_Msg_Pos_Character temp2;
			temp2.Character_id = temp.Character_id;
			//temp2.dir = 6;
			temp2.x = g_ppPlayer[temp.Character_id]->GetPosition().x;
			temp2.y = g_ppPlayer[temp.Character_id]->GetPosition().y;
			temp2.timeElapsed = timeElapsed.count();
			temp2.dwDirection = dwDirection;
			temp2.size = sizeof(temp2);
			temp2.type = SC_POS_PLAYER;
			for (int i = 0; i < MAX_USER; ++i) {
				if(g_clients[i].m_isconnected)
					send(g_clients[i].m_s, (char*)&temp2, sizeof(temp2), 0);
			}
			recvType = -1;
		}
		if (recvType == CS_CHANGE_STATE)
		{
			CS_Msg_Change_State temp;
			retVal = recv(client_sock, (char*)&temp, sizeof(temp) + sizeof(int), 0);
			if (retVal == SOCKET_ERROR) printf("recv() Miss!\n");
			g_ppPlayer[temp.Character_id]->SetState((ObjectState)temp.State);
			g_clients[temp.Character_id].m_state = temp.State;

			SC_Msg_Change_State temp2;
			temp2.Character_id = temp.Character_id;
			temp2.size = sizeof(temp2);
			temp2.State = temp.State;
			temp2.type = SC_CHANGE_STATE;
			for (int i = 0; i < MAX_USER; ++i) {
				if (g_clients[i].m_isconnected)
					send(g_clients[i].m_s, (char*)&temp2, sizeof(temp2), 0);
			}
			recvType = -1;
		}
		
	}

	// closesocket()
	closesocket(client_sock);
	DisplayText("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\r\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}



