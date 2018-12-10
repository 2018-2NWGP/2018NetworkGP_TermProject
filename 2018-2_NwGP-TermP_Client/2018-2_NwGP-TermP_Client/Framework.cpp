#include "stdafx.h"
#include "Framework.h"
#include "Scene1-TitleScene.h"
#include "Scene2-MainGameScene.h"
#include "Object1-PlayerObject.h"
#include <cassert>
#include <fstream>

CNetwork* m_pNetwork;

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ���� ��Ʈ�� ��� �Լ�

HWND hEdit1, hEdit2; // ���� ��Ʈ��

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

	FMOD_System_Create(&soundSystem); // ���� 
	FMOD_System_Init(soundSystem, 10, FMOD_INIT_NORMAL, NULL); // �ʱ�ȭ 

	FMOD_System_CreateSound(soundSystem, "Sound\\YSO201.Prelude to the Omen.mp3", FMOD_LOOP_NORMAL, 0, &TitleSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\Demetori-������ �¾�ž� Nuclear Fusion.mp3", FMOD_LOOP_NORMAL, 0, &BattleSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\YSO201.Prelude to the Omen.mp3", FMOD_LOOP_NORMAL, 0, &WinSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\YSO201.Prelude to the Omen.mp3", FMOD_LOOP_NORMAL, 0, &LoseSound);

	FMOD_System_CreateSound(soundSystem, "Sound\\Effect_Slash.wav", FMOD_LOOP_OFF, 0, &Effect_SlashSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\Effect_Hit.wav", FMOD_LOOP_OFF, 0, &Effect_HitSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\Effect_Death.wav", FMOD_LOOP_OFF, 0, &Effect_DeathSound);

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
	ChangeScene(CBaseScene::SceneTag::Title);

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

	// Title Scene
	arrScene[CBaseScene::SceneTag::Title] = new TitleScene();
	if (FAILED(TitleImage.Load(TEXT("ResourceImage\\Title_1024x806.png"))))
		assert(!"Ÿ��Ʋ ȭ�� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (TitleImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Title Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Title]->SetBackgroundImage(&TitleImage);
	
	if (FAILED(ButtonImage[0].Load(TEXT("ResourceImage\\Button_by_JHL.png"))))
		assert(!"��ư �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (ButtonImage[0].IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Idle Button Image Load Fail"), MB_OK);
	
	if (FAILED(ButtonImage[1].Load(TEXT("ResourceImage\\ButtonOn_by_JHL.png"))))
		assert(!"Ȱ��ȭ�� ��ư �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (ButtonImage[1].IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("OnButton Image Load Fail"), MB_OK);

	if (FAILED(ButtonImage[2].Load(TEXT("ResourceImage\\ButtonPush_by_JHL.png"))))
		assert(!"������ ��ư �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (ButtonImage[2].IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Pushed Button Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Title]->SetButtonImageArray(ButtonImage);

	// Main Scene
	arrScene[CBaseScene::SceneTag::Main] = new CMainScene();
	/*
	if (FAILED(BGI.Load(TEXT("ResourceImage\\TestField_1024x768.png"))))
	assert(!"�׽�Ʈ�ʵ� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	*/
	/*
	if (FAILED(BGI.Load(TEXT("ResourceImage\\High_Field-GoldDragon.png"))))
	assert(!"�ʵ� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	*/
	if (FAILED(BackGroundImage.Load(TEXT("ResourceImage\\BattleStage-Forest.png"))))
		assert(!"�������� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (BackGroundImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Background Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Main]->SetBackgroundImage(&BackGroundImage);
	if (FAILED(UserInterfaceWindowImage.Load(TEXT("ResourceImage\\UI_Window_1024x768.png"))))
		assert(!"UI â �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (UserInterfaceWindowImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("UI Window Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Main]->SetUIWindowImage(&UserInterfaceWindowImage);
	if (FAILED(GaugeImage.Load(TEXT("ResourceImage\\Gauge_Bar.png"))))
		assert(!"������ �� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
	if (GaugeImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("GaugeBar Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Main]->SetGaugeImage(&GaugeImage);

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
			switch (i) {
			case 0:
				m_ppPlayer[i]->SetPosition(400, 400);
				break;
			case 1:
				m_ppPlayer[i]->SetPosition(2000, 2000);
				break;
			case 2:
				m_ppPlayer[i]->SetPosition(2000, 400);
				break;
			case 3:
				m_ppPlayer[i]->SetPosition(400, 2000);
				break;
			case 4:
				m_ppPlayer[i]->SetPosition(1200, 560);
				break;
			case 5:
				m_ppPlayer[i]->SetPosition(1200, 1840);
				break;
			case 6:
				m_ppPlayer[i]->SetPosition(560, 1200);
				break;
			case 7:
				m_ppPlayer[i]->SetPosition(1840, 1200);
				break;
			case 8:
				m_ppPlayer[i]->SetPosition(1200, 1200);
				break;			
			}
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
	case WM_LBUTTONDOWN:	
	case WM_RBUTTONDOWN:
		if (m_pCurrScene == arrScene[CBaseScene::SceneTag::Title]) {
			m_pCurrScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
			m_pCurrScene->UserInterface_Render(m_hDC);
		}	
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		if (m_pCurrScene == arrScene[CBaseScene::SceneTag::Title]) {
			m_pCurrScene->UserInterface_Render(m_hDC);
			if(m_pCurrScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam))
				DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);	
		}
		break;
	case WM_MOUSEMOVE:
		m_pCurrScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		
		break;
	default:
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
	/*if (checklogin == true)
	{
		ChangeScene(CBaseScene::SceneTag::Main);
		checklogin = false;
	}*/
	/*if (m_ppPlayer[m_pNetwork->m_myid]->GetIdleState())
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
	}*/
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
			if (m_ppPlayer[m_pNetwork->m_myid]->GetState() != melee_attack && m_ppPlayer[m_pNetwork->m_myid]->GetAttackFrame() == 0) {
				dwDirection = 0;
				FMOD_System_PlaySound(soundSystem, Effect_SlashSound, NULL, 0, &EffectChannel);
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
		if (dwDirection != 0 && m_ppPlayer[m_pNetwork->m_myid]->GetState() != melee_attack) {
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
#ifdef USE_CONSOLE_WINDOW
			//printf("Packet: {size : %d, type : %d, id : %d, x : %d, y : %d}\n", p.size, p.type, m_pNetwork->m_myid, p.x, p.y);
#endif
		}
		m_ppPlayer[m_pNetwork->m_myid]->SetDirectionBit(dwDirection);
	}
	for (int i = 0; i < MAX_USER; ++i) {
		if (m_ppPlayer[i]) {
			if (m_pNetwork->m_myid == i) {
				m_ppPlayer[i]->CenterPlayerScrolling();
#ifdef USE_CONSOLE_WINDOW
				//printf("Player[%d] : (x : %d, y : %d)\n", m_pNetwork->m_myid, m_ppPlayer[m_pNetwork->m_myid]->GetPosition().x, m_ppPlayer[m_pNetwork->m_myid]->GetPosition().y);
#endif
			}
			else {
				m_ppPlayer[i]->OtherScolling(m_ppPlayer[m_pNetwork->m_myid]);
			}
//			for (int j = 0; j < MAX_USER; ++j) {
//				if (m_ppPlayer[i]->GetState() == melee_attack) {
//					if (m_ppPlayer[j])
//						if (m_ppPlayer[i]->RectAttackCollide(m_ppPlayer[j])) {
//#ifdef USE_CONSOLE_WINDOW
//							//printf("%d�� �÷��̾�� ����!\n", j);
//#endif
//						}
//				}
//			}
			if(m_ppPlayer[i]->GetAttackFrame() == 0 && m_ppPlayer[i]->GetState() == melee_attack)
				if (i != m_pNetwork->m_myid)
					FMOD_System_PlaySound(soundSystem, Effect_SlashSound, NULL, 0, &EffectChannel);
			m_ppPlayer[i]->Update(fTimeElapsed);
		}
	}
	if (m_pCurrScene != arrScene[CBaseScene::SceneTag::Main])
		ZeroCurrentTime();
	//m_pCurrScene->Update(fTimeElapsed);
	//m_pNetwork->ReadPacket();
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

	if (m_pCurrScene) m_pCurrScene->Render(m_hDC);
	if (m_pCurrScene == arrScene[CBaseScene::SceneTag::Main]) {
		for (int i = MAX_USER - 1; i >= 0; --i)
			if (m_ppPlayer[i]) m_ppPlayer[i]->Render(m_hDC);
#ifdef UNICODE
		TCHAR buf[100];
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				wsprintf(buf, TEXT("ID %d"), i);
				UserInterfaceWindowImage.Draw(m_hDC, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 20, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 17, 100, 17, 0, 0, 1024, 768);
				if (i == m_pNetwork->m_myid)
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("�ü�ü"), RGB(0, 255, 127));
				else
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("�ü�ü"), RGB(255, 0, 0));
			}
		}
#else
		char buf[100];
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				sprintf(buf, TEXT("ID %d"), i);
				UserInterfaceWindowImage.Draw(m_hDC, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 20, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 17, 100, 17, 0, 0, 1024, 768);
				if (i == m_pNetwork->m_myid)
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("�ü�ü"), RGB(0, 255, 127));
				else
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("�ü�ü"), RGB(255, 0, 0));
			}
		}
#endif

		if (m_pCurrScene) m_pCurrScene->UserInterface_Render(m_hDC);

#ifdef UNICODE	
		DrawFont(TEXT("HP"), 25, 30, 15, 0, TEXT("�ü�ü"), RGB(255, 0, 0));
		wsprintf(buf, TEXT("%d/%d"), m_ppPlayer[m_pNetwork->m_myid]->GetHP(), MAX_HP);
		DrawFont(buf, 375, 30, 15, 0, TEXT("�ü�ü"), RGB(255, 0, 0));
		wsprintf(buf, TEXT("Your ID : %d"), m_pNetwork->m_myid);
		DrawFont(buf, 25, 75, 15, 0, TEXT("�ü�ü"), RGB(0, 255, 255));
		wsprintf(buf, TEXT("Score : %d"), m_ppPlayer[m_pNetwork->m_myid]->GetScore());
		DrawFont(buf, 250, 75, 15, 0, TEXT("�ü�ü"), RGB(0, 255, 0));
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				if (i == m_pNetwork->m_myid) continue;
				wsprintf(buf, TEXT("ID %d"), i);
				DrawFont(buf, 805, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("�ü�ü"), RGB(255, 127, 127));
				DrawFont(TEXT("Score"), 915, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("�ü�ü"), RGB(255, 255, 0));
				wsprintf(buf, TEXT("%d"), m_ppPlayer[i]->GetScore());
				DrawFont(buf, 915, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("�ü�ü"), RGB(255, 255, 0));
				if (GaugeImage) {
					GaugeImage.Draw(m_hDC, 805, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 100, 15, 9, 0, 1, 8);	// 100 == MAX_HP
					GaugeImage.Draw(m_hDC, 805, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), max(m_ppPlayer[i]->GetHP(), 1), 15, 0, 0, 9, 8);
				}
			}
		}
		//wsprintf(buf, TEXT("Time %d:%d"), static_cast<int>(m_CurrentTime) / 60, static_cast<int>(m_CurrentTime) % 60);
		//DrawFont(buf, 605, 20, 30, 0, TEXT("�ü�ü"), RGB(255, 255, 255));
#else
		DrawFont(TEXT("HP"), 25, 30, 15, 0, TEXT("�ü�ü"), RGB(255, 0, 0));
		sprintf(buf, TEXT("%d/%d"), m_ppPlayer[m_pNetwork->m_myid]->GetHP(), MAX_HP);
		DrawFont(buf, 375, 30, 15, 0, TEXT("�ü�ü"), RGB(255, 0, 0));
		sprintf(buf, TEXT("Your ID : %d"), m_pNetwork->m_myid);
		DrawFont(buf, 25, 75, 15, 0, TEXT("�ü�ü"), RGB(0, 255, 255));
		sprintf(buf, TEXT("Score : %d"), m_ppPlayer[m_pNetwork->m_myid]->GetScore());
		DrawFont(buf, 250, 75, 15, 0, TEXT("�ü�ü"), RGB(0, 255, 0));
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				if (i == m_pNetwork->m_myid) continue;
				sprintf(buf, TEXT("ID %d"), i);
				DrawFont(buf, 805, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("�ü�ü"), RGB(255, 127, 127));
				DrawFont(TEXT("Score"), 915, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("�ü�ü"), RGB(255, 255, 0));
				sprintf(buf, TEXT("%d"), m_ppPlayer[i]->GetScore());
				DrawFont(buf, 915, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("�ü�ü"), RGB(255, 255, 0));
				if (GaugeImage) {
					GaugeImage.Draw(m_hDC, 805, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 100, 15, 9, 0, 1, 8);	// 100 == MAX_HP
					GaugeImage.Draw(m_hDC, 805, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), max(m_ppPlayer[i]->GetHP(), 0), 15, 0, 0, 9, 8);
				}
			}
		}
#endif

		for (int i = 0; i < MAX_USER; ++i)
			if (m_ppPlayer[i])
				if (m_ppPlayer[i]->GetWinTrigger()) {
#ifdef UNICODE
					wsprintf(buf, TEXT("Player %d Win!"), i);
#else
					sprintf(buf, TEXT("Player %d Win!"), i);
#endif
					DrawFont(buf, 1, 375, 75, 75, TEXT("�ü�ü"), RGB(255, 255, 127));
				}
	}
	else if (m_pCurrScene == arrScene[CBaseScene::SceneTag::Title]) {
#ifdef UNICODE	
		TCHAR buf[100];
		wsprintf(buf, TEXT("Lord of"));
		DrawFont(buf, 100, 75, 100, 100, TEXT("�ü�ü"), RGB(70, 255, 127));
		wsprintf(buf, TEXT("Slayers"));
		DrawFont(buf, 250, 275, 100, 100, TEXT("�ü�ü"), RGB(70, 255, 127));
		if (m_pCurrScene) m_pCurrScene->UserInterface_Render(m_hDC);
		wsprintf(buf, TEXT("LOGIN"));
		DrawFont(buf, 345, 575, 65, 50, TEXT("�ü�ü"), RGB(125, 100, 113));
#else
		char buf[100];
		sprintf(buf, TEXT("Lord of"));
		DrawFont(buf, 100, 75, 100, 100, TEXT("�ü�ü"), RGB(70, 255, 127));
		sprintf(buf, TEXT("Slayers"));
		DrawFont(buf, 300, 275, 100, 100, TEXT("�ü�ü"), RGB(70, 255, 127));
		if (m_pCurrScene) m_pCurrScene->UserInterface_Render(m_hDC);
		sprintf(buf, TEXT("LOGIN"));
		DrawFont(buf, 345, 575, 65, 50, TEXT("�ü�ü"), RGB(125, 100, 113));
#endif
	}

	
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

			// �ȸ޽����� �˻��ϰ� Ʈ�����޽���, ����ġ�޽������� ���������ν����� �޽����� �Ѱ��ش�.
			//self->OnDraw(hdc);
	
			::EndPaint(hWnd, &ps); // �׸��� �����.
		}
		break;
	case WM_DESTROY:
		::SetUserDataPtr(hWnd, NULL);
		::PostQuitMessage(0);
		::exit(1);
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
	FMOD_Channel_Stop(BGM_channel);
	m_pCurrScene = arrScene[tag];
	switch (tag) {
	case CBaseScene::SceneTag::Title:
		FMOD_System_PlaySound(soundSystem, TitleSound, NULL, 0, &BGM_channel);
		break;
	case CBaseScene::SceneTag::Main:
		FMOD_System_PlaySound(soundSystem, BattleSound, NULL, 0, &BGM_channel);
		break;
	}
}

void CFramework::RecvPacket()
{
	m_pNetwork->ReadPacket(m_pNetwork->m_mysocket);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	char id[255];
	int x, y, pw;
	pw = 0;
	int id_protocol, pw_protocol;
	switch (uMsg) {
	case WM_INITDIALOG:
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
		hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			x = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT1));
			y = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT2));
			GetDlgItemText(hDlg, IDC_EDIT1, (LPTSTR)id, x + 1);
			
			pw = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, FALSE);
			
			CS_Msg_Demand_LoginID lp;
			strcpy(lp.id, id);
			lp.pw = pw;
			lp.my_id = m_pNetwork->m_myid;
			lp.size = sizeof(lp);
			lp.type = CS_LOGIN_ID;
			id_protocol = lp.type;
						
			send(m_pNetwork->m_mysocket, (char*)&id_protocol, sizeof(id_protocol), 0);
			m_pNetwork->SendPacket(&lp);
			std::cout << lp.pw<<" "<<lp.my_id<<""<<lp.id<< std::endl;
			EndDialog(hDlg, IDCANCEL);		
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	}
	return 0;
}