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
	
}

CFramework::~CFramework()
{
	OnDestroy();
}

bool CFramework::OnCreate(HINSTANCE hInstance, HWND hWnd, const RECT & rc, CNetwork* pNetwork)
{
	srand((unsigned int)time(NULL));
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	m_pNetwork = pNetwork;
	m_rcClient = rc;
	// Ŭ���̾�Ʈ ��ǥ �ʱ�ȭ
	m_rcClient.right -= m_rcClient.left;
	m_rcClient.bottom -= m_rcClient.top;
	m_rcClient.left = 0;
	m_rcClient.top = 0;

	// ���� ����
	CreatebackBuffer();

	// Ŭ������ ������ ���ν��� ����
	::SetUserDataPtr(m_hWnd, this);

	// ...

	// ĸ�� ����
#ifdef UNICODE
	lstrcpy(m_CaptionTitle, TITLESTRING);
#if defined(SHOW_CAPTIONFPS)
	lstrcat(m_CaptionTitle, TEXT("("));
#endif
	m_TitleLength = lstrlen(m_CaptionTitle);
	SetWindowText(m_hWnd, m_CaptionTitle);
#else
	strcpy(m_CaptionTitle, TITLESTRING);
#if defined(SHOW_CAPTIONFPS)
	strcat(m_CaptionTitle, TEXT("("));
#endif
	m_TitleLength = strlen(m_CaptionTitle);
	SetWindowText(m_hWnd, m_CaptionTitle)
#endif
	// Ÿ�̸� �ʱ�ȭ
//	m_LastUpdate_time = chrono::system_clock::now(); // �������� �ʾҴ�.
	m_current_time = std::chrono::system_clock::now();
	m_fps = 0;

	m_pNetwork->Initialize(m_hWnd);
	m_pNetwork->SetFramework(this);


	BuildPlayer();
	// �� ����
	//BuildScene();
	// ������ ���� �����ΰ�?
	//ChangeScene(CBaseScene::SceneTag::Main);
	
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
	m_hDC = ::CreateCompatibleDC(hdc);	// ȣȯ�� �����ִ� �Լ�
	// ������ ��ũ ���ο� ����۸� �����. hdc�� ȣȯ�� ��Ʈ���� ����Ѵ�.
	m_hBitmapBackBuffer = ::CreateCompatibleBitmap(hdc, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDC, m_hBitmapBackBuffer);

	SetBKColor(RGB(255, 255, 255));

	ClearBackgroundColor();

	::ReleaseDC(m_hWnd, hdc);
}

void CFramework::BuildScene()
{
	// arrScene[SceneTag::Title] = new TitleScene();	// �̷� ������� ���� ���� ���̴�.
	arrScene[CBaseScene::SceneTag::Main] = new CMainScene();
	/*
	if (FAILED(BGI.Load(TEXT("ResourceImage\\TestField_1024x768.png"))))
	assert(!"�׽�Ʈ�ʵ� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");*/
	/*
	if (FAILED(BGI.Load(TEXT("ResourceImage\\High_Field-GoldDragon.png"))))
	assert(!"�ʵ� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");*/
	if (FAILED(BGI.Load(TEXT("ResourceImage\\BattleStage-Forest.png"))))
		assert(!"�ʵ� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (BGI.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Background Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Main]->SetBackgroundImage(&BGI);
	arrScene[CBaseScene::SceneTag::Main]->SetPlayer(m_ppPlayer[m_pNetwork->m_myid]);
	arrScene[CBaseScene::SceneTag::Main]->BuildObjects();
	//m_ppPlayer[m_pNetwork->m_myid]->SetID(m_pNetwork->m_myid);
}

void CFramework::BuildPlayer()
{
	if (FAILED(PlayerImage.Load(TEXT("ResourceImage\\Player001_SwordMan.png"))))
		assert(!"�÷��̾� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (PlayerImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Player Image Load Fail"), MB_OK);
	// PlayerImage.Create(CLIENT_WIDTH, CLIENT_HEIGHT, 24);	// DC ������ִ� ���� ���� ������ �Լ�

	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution<> randW(32, 4800);
	std::uniform_int_distribution<> randH(64, 3200);

	/*if (!m_pPlayer) {
		m_pPlayer = new PlayerObject();
		m_pPlayer->SetPosition(800, 600);	
		m_pPlayer->SetImage(&PlayerImage);
		m_pPlayer->SetSize(32, 64);
		m_pPlayer->SetBackgroundSize(4800, 3200);
		m_pNetwork->SetPlayer(m_pPlayer);
	}*/

	if (!m_ppPlayer) {
		m_ppPlayer = new PlayerObject*[MAX_USER];
		for (int i = 0; i < MAX_USER; ++i)
		{
			m_ppPlayer[i] = new PlayerObject();
			m_ppPlayer[i]->SetPosition(800+ (i * 100), 600);
			m_ppPlayer[i]->SetImage(&PlayerImage);
			m_ppPlayer[i]->SetSize(32, 64);
			m_ppPlayer[i]->SetBackgroundSize(2400, 2400);		
		}	
		m_pNetwork->SetPlayers(m_ppPlayer);
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
			::DestroyWindow(hWnd);	// ESC Ű�� ������ �� �����츦 �����Ѵ�.
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
	case WM_SIZE:	// ������ ũ�⸦ �����ϴ� ���� ���� ���´�.
		break;
	default:
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);
	}
//	return E_NOTIMPL;
	return 0;
}

void CFramework::Update(float fTimeElapsed)
{
	if (m_bUpdateActiveTrigger) {
		if (m_ppPlayer[m_pNetwork->m_myid]->GetIdleState())
		{
			m_ppPlayer[m_pNetwork->m_myid]->SetDirectionBit(0);
			int protocol;
			CS_Msg_Change_State p;
			p.size = sizeof(p);
			p.Character_id = m_pNetwork->m_myid;
			p.State = (BYTE)idle;
			p.type = CS_CHANGE_STATE;
			protocol = p.type;
			send(m_pNetwork->m_mysocket, (char*)&protocol, sizeof(protocol), 0);
			m_pNetwork->SendPacket(&p);
			m_ppPlayer[m_pNetwork->m_myid]->SetIdleState();
		}
		static UCHAR pKeysBuffer[256];
		bool bProcessedByScene = false;
		// �÷��̾ �������� �ʴ� ȭ�鿡���� �ش� ���� ProcessInput �Լ��� ����
		if (GetKeyboardState(pKeysBuffer) && m_pCurrScene) bProcessedByScene = m_pCurrScene->ProcessInput(pKeysBuffer);
		// ProcessInput �Լ��� ������� ���� ������ ���
		if (!bProcessedByScene)
		{
			DWORD dwDirection = 0;
			//m_pPlayer->SetDirection(dwDirection);

			if (pKeysBuffer[VK_UP] & 0xF0) { dwDirection |= DIR_UP; }
			if (pKeysBuffer[VK_DOWN] & 0xF0) { dwDirection |= DIR_DOWN; }
			if (pKeysBuffer[VK_LEFT] & 0xF0) { dwDirection |= DIR_LEFT; }
			if (pKeysBuffer[VK_RIGHT] & 0xF0) { dwDirection |= DIR_RIGHT; }

			if ((pKeysBuffer['a'] & 0xF0) || (pKeysBuffer['A'] & 0xF0)) {
				if (m_ppPlayer[m_pNetwork->m_myid]->GetState() != melee_attack) {
					m_ppPlayer[m_pNetwork->m_myid]->SetState(melee_attack);

					int protocol;
					CS_Msg_Change_State p;
					p.size = sizeof(p);
					p.Character_id = m_pNetwork->m_myid;
					p.State = (BYTE)melee_attack;
					p.type = CS_CHANGE_STATE;
					protocol = p.type;
					send(m_pNetwork->m_mysocket, (char*)&protocol, sizeof(protocol), 0);
					m_pNetwork->SendPacket(&p);
				}
			}
			if (dwDirection != 0) {
				int protocol;
				CS_Msg_Pos_Character p;
				p.size = sizeof(p);
				p.Character_id = m_pNetwork->m_myid;
				p.x = m_ppPlayer[m_pNetwork->m_myid]->GetPosition().x;
				p.y = m_ppPlayer[m_pNetwork->m_myid]->GetPosition().y;
				p.dwDirection = dwDirection;
				p.type = CS_MOVE;
				protocol = p.type;
				send(m_pNetwork->m_mysocket, (char*)&protocol, sizeof(protocol), 0);
				m_pNetwork->SendPacket(&p);
				printf("Packet: {size : %d, type : %d, id : %d, x : %d, y : %d}\n", p.size, p.type, m_pNetwork->m_myid, p.x, p.y);
			}
			m_ppPlayer[m_pNetwork->m_myid]->SetDirectionBit(dwDirection);
		}
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				if (m_pNetwork->m_myid == i) {
					m_ppPlayer[i]->CenterPlayerScrolling();
					printf("Player[%d] : (x : %d, y : %d)\n", m_pNetwork->m_myid, m_ppPlayer[m_pNetwork->m_myid]->GetPosition().x, m_ppPlayer[m_pNetwork->m_myid]->GetPosition().y);
				}
				else {
					m_ppPlayer[i]->OtherScolling(m_ppPlayer[m_pNetwork->m_myid]);
				}
				for (int j = i + 1; j < MAX_USER; ++j) {
					if (m_ppPlayer[j])
						if (m_ppPlayer[i]->RectAttackCollide(m_ppPlayer[j]))
							printf("%d�� �÷��̾�� ����!\n", j);
				}
				m_ppPlayer[i]->Update(fTimeElapsed);
			}
		}
		m_pCurrScene->Update(fTimeElapsed);
		//m_pNetwork->ReadPacket();
	}
}

void CFramework::SetBKColor(COLORREF color)
{
	m_clrBackBuffer = color;
	if (m_hbrBackground) ::DeleteObject(m_hbrBackground);
	m_hbrBackground = ::CreateSolidBrush(m_clrBackBuffer);	// �� ����� �귯���� ������.
}

void CFramework::ClearBackgroundColor()
{
	::FillRect(m_hDC, &m_rcClient, m_hbrBackground);	// ���� ������� ��׶��忡 ���� Ŭ���̾�Ʈ��ŭ�� ������ ��� ���� �귯���� ������ ��ĥ�ض�.
}

void CFramework::PreprocessingForDraw()
{
	ClearBackgroundColor();	// �׸��� ������ ���۵Ǹ� ������ �׶��带 �����ϰ� �����.
	::SetBkColor(m_hDC, TRANSPARENT);	// ���ĺ��� ��� �����ϰ� ����
	::SetStretchBltMode(m_hDC, COLORONCOLOR);	// ���� ������ �޶� �þ�ų� �پ�� ������ �ִ� ��� �����.

	m_pCurrScene->Render(m_hDC);
	for(int i = 0; i<MAX_USER; ++i) m_ppPlayer[i]->Render(m_hDC);
}

void CFramework::OnDraw(HDC hDC)
{
	// ���ڷ� �޴� hDC�� �ۿ��� ������� hDC�̴�. �� ��Ʈ�ʿ� m_hDC�� ����ۿ��� ������� �׸��� �Ѹ���.
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
		PreprocessingForDraw(); // ����� �����̹Ƿ� OnDraw�� �ƴϴ�. OnDraw ������ ����ۿ� �׷��ִ� ������ �Ѵ�.
		InvalidateRect(m_hWnd, &m_rcClient, FALSE);	// False�� �ʱ�ȭ�� ���� �ʴ´ٴ� ���̴�. ������ ������ �޽����� ȣ���Ѵ�.
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(m_hWnd, &ps);																																		
		OnDraw(hdc);
		::EndPaint(m_hWnd, &ps);

		if (m_timeElapsed.count() > 0.0)
			m_fps = 1.0 / m_timeElapsed.count();
	}
	InvalidateRect(m_hWnd, &m_rcClient, FALSE);	// False�� �ʱ�ȭ�� ���� �ʴ´ٴ� ���̴�. ������ ������ �޽����� ȣ���Ѵ�.
	// ĸ�ǿ� ���ڸ� �� ������ �����ϴ� ĸ�� ��Ʈ�� ����
#ifdef UNICODE
	_itow_s(
		m_fps + 0.1f
		, m_CaptionTitle + m_TitleLength
		, TITLE_MX_LENGTH - m_TitleLength
		, 10);
	wcscat_s(
		m_CaptionTitle + m_TitleLength
		, TITLE_MX_LENGTH - m_TitleLength
		, TEXT("FPS )"));
#else
	_itoa_s(
		m_fps + 0.1f
		, m_CaptionTitle + m_TitleLength
		, TITLE_MX_LENGTH - m_TitleLength
		, 10);
	strcat_s(
		m_CaptionTitle + m_TitleLength
		, TITLE_MX_LENGTH - m_TitleLength
		, TEXT("FPS )"));
#endif
	SetWindowText(m_hWnd, m_CaptionTitle);
}

LRESULT CFramework::WndProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CFramework* self = ::GetUserDataPtr<CFramework*>(hWnd);	// static �Լ��� �����Ϸ��� ������ ������. Ŭ������ �����Լ��� ����ϱ� ���� �ļ�
	if (!self)
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);	// �޽��� ó���� OS���� �ѱ��.

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:

	case WM_MOUSEMOVE:
		self->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_KEYDOWN: case WM_KEYUP: case WM_CHAR:
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE) {
			// ��Ȱ��ȭ
			self->SetUpdateTrigger(false);
		}
		else {
			// Ȱ��ȭ
			self->SetUpdateTrigger(true);
		}
	}
		self->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);	// �� ����̽� ���ؽ�Ʈ�� ����ϰڴ�.
			// hdc���� �� �Լ��� ���� bitmap�� ���δ�.

			// CPU �ܰ迡�� ȣ���ϴ� �Ŷ� �޽��� �˻� �κ� ���� ������ ������.
			// ���� ������ �޽����� �� ���� ������ ���ؼ��� ���� �ʰ� �ȴ�. ������ �˾Ƴ���� �ؾ��Ѵ�.
			// �ȸ޽����� �˻��ϰ� Ʈ�����޽���, ����ġ�޽������� ���������ν����� �޽����� �Ѱ��ش�.
			//self->OnDraw(hdc);
	
			::EndPaint(hWnd, &ps); // �׸��� �����.
		}
		break;
	case WM_DESTROY:
		::SetUserDataPtr(hWnd, NULL);
		::PostQuitMessage(0);
		break;
	case WM_SOCKET:
		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
			self->RecvPacket();
			break;
		}
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

void CFramework::RecvPacket()
{
	m_pNetwork->ReadPacket();
}
