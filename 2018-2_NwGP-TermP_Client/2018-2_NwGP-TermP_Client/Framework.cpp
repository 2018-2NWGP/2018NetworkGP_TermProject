#include "stdafx.h"
#include "Framework.h"
#include "Scene1-MainGameScene.h"
#include "Object1-PlayerObject.h"
#include <cassert>



template<typename T>
T GetUserDataPtr(HWND hWnd)
{
	return reinterpret_cast<T>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}
void SetUserDataPtr(HWND hWnd, LPVOID ptr)
{
	LONG_PTR result = :: SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr));
}

CFramework::CFramework()
{
	int retval;

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

	char c[BUFSIZE+1];

	// 데이터 보내기
	retval = send(sock, c, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		printf("send실패");
	}
	printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
}

CFramework::~CFramework()
{
	OnDestroy();
}

bool CFramework::OnCreate(HINSTANCE hInstance, HWND hWnd, const RECT & rc)
{
	srand((unsigned int)time(NULL));
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	m_rcClient = rc;
	// 클라이언트 좌표 초기화
	m_rcClient.right -= m_rcClient.left;
	m_rcClient.bottom -= m_rcClient.top;
	m_rcClient.left = 0;
	m_rcClient.top = 0;

	// 버퍼 생성
	CreatebackBuffer();

	// 클래스와 윈도우 프로시저 연결
	::SetUserDataPtr(m_hWnd, this);

	// ...

	// 캡션 변경
	lstrcpy(m_CaptionTitle, TITLESTRING);

#if defined(SHOW_CAPTIONFPS)
	lstrcat(m_CaptionTitle, TEXT("("));
#endif
	m_TitleLength = lstrlen(m_CaptionTitle);
	SetWindowText(m_hWnd, m_CaptionTitle);

	// 타이머 초기화
//	m_LastUpdate_time = chrono::system_clock::now(); // 선언하지 않았다.
	m_current_time = std::chrono::system_clock::now();
	m_fps = 0;

	BuildPlayer();
	// 씬 생성
	BuildScene();
	// 최초의 씬은 무엇인가?
	ChangeScene(CBaseScene::SceneTag::Main);
	

	return (m_hWnd != NULL);
}

void CFramework::CreatebackBuffer()
{
	if (m_hDC)	{
		::SelectObject(m_hDC, NULL);
		::DeleteDC(m_hDC);
	}
	if (m_hBitmapBackBuffer) ::DeleteObject(m_hBitmapBackBuffer);

	HDC hdc = ::GetDC(m_hWnd);
	m_hDC = ::CreateCompatibleDC(hdc);	// 호환을 시켜주는 함수
	// 프레임 워크 내부에 백버퍼를 만든다. hdc에 호환된 비트맵을 사용한다.
	m_hBitmapBackBuffer = ::CreateCompatibleBitmap(hdc, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDC, m_hBitmapBackBuffer);

	SetBKColor(RGB(255, 255, 255));

	ClearBackgroundColor();

	::ReleaseDC(m_hWnd, hdc);
}

void CFramework::BuildScene()
{
	// arrScene[SceneTag::Title] = new TitleScene();	// 이런 방식으로 씬을 만들 것이다.
	arrScene[CBaseScene::SceneTag::Main] = new CMainScene();
	/*
	if (FAILED(BGI.Load(TEXT("ResourceImage\\TestField_1024x768.png"))))
		assert(!"테스트필드 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");*/
	if (FAILED(BGI.Load(TEXT("ResourceImage\\High_Field-GoldDragon.png"))))
		assert(!"필드 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (BGI.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Background Image Load Fail"), MB_OK);	
	arrScene[CBaseScene::SceneTag::Main]->SetBackgroundImage(&BGI);
	arrScene[CBaseScene::SceneTag::Main]->SetPlayer(m_pPlayer);
}

void CFramework::BuildPlayer()
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution<> randW(32, 4800);
	std::uniform_int_distribution<> randH(64, 3200);

	if (!m_pPlayer) {
		m_pPlayer = new PlayerObject();
		m_pPlayer->SetPosition(800, 600);
		if (FAILED(PlayerImage.Load(TEXT("ResourceImage\\Player001_SwordMan.png"))))
			assert(!"플레이어 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");		
		if (PlayerImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Player Image Load Fail"), MB_OK);
		// PlayerImage.Create(CLIENT_WIDTH, CLIENT_HEIGHT, 24);	// DC 만들어주는 버그 펑펑 터지는 함수
		m_pPlayer->SetImage(&PlayerImage);
		m_pPlayer->SetSize(32, 64);
		m_pPlayer->SetBackgroundSize(4800, 3200);
	}
}

void CFramework::ReleaseScene()
{
}

bool CFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		return true;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::DestroyWindow(hWnd);	// ESC 키를 눌렀을 때 윈도우를 종료한다.
			break;
		}
		return true;
	}
	return false;
}

bool CFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONUP:
		break;
	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE:
		break;

	}
	return false;
}

HRESULT CFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:	// 윈도의 크기를 조절하는 순간 여기 들어온다.
		break;
	default:
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);
	}
//	return E_NOTIMPL;
	return 0;
}

void CFramework::Update(float fTimeElapsed)
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	// 플레이어를 조작하지 않는 화면에서는 해당 씬의 ProcessInput 함수를 실행
	if (GetKeyboardState(pKeysBuffer) && m_pCurrScene) bProcessedByScene = m_pCurrScene->ProcessInput(pKeysBuffer);
	// 플레이어를 조작하는 화면에서는 아래의 제어문 진입
	if (!bProcessedByScene)
	{
		DWORD dwDirection = 0;
		if (pKeysBuffer[VK_UP] & 0xF0) dwDirection |= DIR_UP;
		if (pKeysBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_DOWN;
		if (pKeysBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeysBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if ((pKeysBuffer['a'] & 0xF0) || (pKeysBuffer['A'] & 0xF0)) m_pPlayer->SetState(melee_attack);

		m_pPlayer->SetDirection(dwDirection);
		// printf("x : %.2lf, y : %.2lf\n", m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y);
	}
	m_pPlayer->Update(fTimeElapsed);

	m_pCurrScene->Update(fTimeElapsed);
}

void CFramework::SetBKColor(COLORREF color)
{
	m_clrBackBuffer = color;
	if (m_hbrBackground) ::DeleteObject(m_hbrBackground);
	m_hbrBackground = ::CreateSolidBrush(m_clrBackBuffer);	// 이 색깔로 브러쉬를 만들어라.
}

void CFramework::ClearBackgroundColor()
{
	::FillRect(m_hDC, &m_rcClient, m_hbrBackground);	// 현재 백버퍼의 백그라운드에 현재 클라이언트만큼의 영역을 방금 만든 브러쉬의 색으로 색칠해라.
}

void CFramework::PreprocessingForDraw()
{
	ClearBackgroundColor();	// 그리기 연산이 시작되면 무조건 그라운드를 깨끗하게 만든다.
	::SetBkColor(m_hDC, TRANSPARENT);	// 알파블렌딩 허용 가능하게 설정
	::SetStretchBltMode(m_hDC, COLORONCOLOR);	// 쓰는 범위가 달라서 늘어나거나 줄어들 여지가 있는 경우 덮어쓴다.

	m_pCurrScene->Render(m_hDC);
	m_pPlayer->Render(m_hDC);
}

void CFramework::OnDraw(HDC hDC)
{
	// 인자로 받는 hDC는 밖에서 만들어진 hDC이다. 이 비트맵에 m_hDC의 백버퍼에서 만들어진 그림을 뿌린다.
	::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, m_hDC, 0, 0, SRCCOPY);
}

void CFramework::FrameAdvance()
{
	
	
	// Get tick
	m_timeElapsed = std::chrono::system_clock::now() - m_current_time;
	if (m_timeElapsed.count() > MAX_FRAMETIME)
	{
		m_current_time = std::chrono::system_clock::now();

		Update(m_timeElapsed.count());
		PreprocessingForDraw(); // 백버퍼 연산이므로 OnDraw가 아니다. OnDraw 이전에 백버퍼에 그려주는 연산을 한다.
		InvalidateRect(m_hWnd, &m_rcClient, FALSE);	// False는 초기화를 하지 않는다는 뜻이다. 강제로 윈도우 메시지를 호출한다.
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(m_hWnd, &ps);																																		
		OnDraw(hdc);
		::EndPaint(m_hWnd, &ps);

		if (m_timeElapsed.count() > 0.0)
			m_fps = 1.0 / m_timeElapsed.count();
	}
	InvalidateRect(m_hWnd, &m_rcClient, FALSE);	// False는 초기화를 하지 않는다는 뜻이다. 강제로 윈도우 메시지를 호출한다.
	// 캡션에 글자를 뭘 넣을지 연산하는 캡션 스트링 연산
	_itow_s(
		m_fps + 0.1f
		, m_CaptionTitle + m_TitleLength
		, TITLE_MX_LENGTH - m_TitleLength
		, 10);
	wcscat_s(
		m_CaptionTitle + m_TitleLength
		, TITLE_MX_LENGTH - m_TitleLength
		, TEXT("FPS )"));
	SetWindowText(m_hWnd, m_CaptionTitle);
	
}

LRESULT CFramework::WndProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CFramework* self = ::GetUserDataPtr<CFramework*>(hWnd);	// static 함수랑 연결하려고 선언한 포인터. 클래스를 정적함수에 등록하기 위한 꼼수
	if (!self)
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);	// 메시지 처리를 OS에게 넘긴다.

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:

	case WM_MOUSEMOVE:
		self->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;


	case WM_KEYDOWN:
	case WM_KEYUP:
		self->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);	// 이 디바이스 컨텍스트를 사용하겠다.
			// hdc에서 이 함수를 통해 bitmap을 붙인다.

			// CPU 단계에서 호출하는 거라 메시지 검사 부분 등이 굉장히 느리다.
			// 따라서 윈도우 메시지는 더 빠른 게임을 위해서는 쓰지 않게 된다. 하지만 알아놓기는 해야한다.
			// 픽메시지를 검사하고 트랜스메시지, 디스패치메시지에서 윈도우프로시저로 메시지를 넘겨준다.
			//self->OnDraw(hdc);
	
			::EndPaint(hWnd, &ps); // 그리고 지운다.
		}
		break;
	case WM_DESTROY:
		::SetUserDataPtr(hWnd, NULL);
		::PostQuitMessage(0);
		break;

	default:
		return self->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
	}
	return 0;
//	return LRESULT();
}

void CFramework::ChangeScene(CBaseScene::SceneTag tag)
{
	m_pCurrScene = arrScene[tag];
}