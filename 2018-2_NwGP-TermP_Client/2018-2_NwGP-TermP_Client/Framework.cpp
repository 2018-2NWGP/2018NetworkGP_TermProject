#include "stdafx.h"
#include "Framework.h"
#include "Scene1-TitleScene.h"
#include "Scene2-MainGameScene.h"
#include "Object1-PlayerObject.h"
#include <cassert>
#include <fstream>

CNetwork* m_pNetwork;

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 편집 컨트롤 출력 함수

HWND hEdit1, hEdit2; // 편집 컨트롤

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
	// 클라이언트 좌표 초기화
	m_rcClient.right -= m_rcClient.left;
	m_rcClient.bottom -= m_rcClient.top;
	m_rcClient.left = 0;
	m_rcClient.top = 0;

	FMOD_System_Create(&soundSystem); // 생성 
	FMOD_System_Init(soundSystem, 10, FMOD_INIT_NORMAL, NULL); // 초기화 

	FMOD_System_CreateSound(soundSystem, "Sound\\YSO201.Prelude to the Omen.mp3", FMOD_LOOP_NORMAL, 0, &TitleSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\Demetori-영지의 태양신앙 Nuclear Fusion.mp3", FMOD_LOOP_NORMAL, 0, &BattleSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\YSO201.Prelude to the Omen.mp3", FMOD_LOOP_NORMAL, 0, &WinSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\YSO201.Prelude to the Omen.mp3", FMOD_LOOP_NORMAL, 0, &LoseSound);

	FMOD_System_CreateSound(soundSystem, "Sound\\Effect_Slash.wav", FMOD_LOOP_OFF, 0, &Effect_SlashSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\Effect_Hit.wav", FMOD_LOOP_OFF, 0, &Effect_HitSound);
	FMOD_System_CreateSound(soundSystem, "Sound\\Effect_Death.wav", FMOD_LOOP_OFF, 0, &Effect_DeathSound);

	// 버퍼 생성
	CreatebackBuffer();

	// 클래스와 윈도우 프로시저 연결
	::SetUserDataPtr(m_hWnd, this);

	// ...

	// 캡션 변경
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
	// 타이머 초기화
//	m_LastUpdate_time = chrono::system_clock::now(); // 선언하지 않았다.
	m_current_time = std::chrono::system_clock::now();
	m_fps = 0;

	m_pNetwork->Initialize(m_hWnd);
	m_pNetwork->SetFramework(this);


	BuildPlayer();
	// 씬 생성
	//BuildScene();
	// 최초의 씬은 무엇인가?
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

	// Title Scene
	arrScene[CBaseScene::SceneTag::Title] = new TitleScene();
	if (FAILED(TitleImage.Load(TEXT("ResourceImage\\Title_1024x806.png"))))
		assert(!"타이틀 화면 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (TitleImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Title Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Title]->SetBackgroundImage(&TitleImage);
	
	if (FAILED(ButtonImage[0].Load(TEXT("ResourceImage\\Button_by_JHL.png"))))
		assert(!"버튼 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (ButtonImage[0].IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Idle Button Image Load Fail"), MB_OK);
	
	if (FAILED(ButtonImage[1].Load(TEXT("ResourceImage\\ButtonOn_by_JHL.png"))))
		assert(!"활성화된 버튼 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (ButtonImage[1].IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("OnButton Image Load Fail"), MB_OK);

	if (FAILED(ButtonImage[2].Load(TEXT("ResourceImage\\ButtonPush_by_JHL.png"))))
		assert(!"눌려진 버튼 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (ButtonImage[2].IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Pushed Button Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Title]->SetButtonImageArray(ButtonImage);

	// Main Scene
	arrScene[CBaseScene::SceneTag::Main] = new CMainScene();
	/*
	if (FAILED(BGI.Load(TEXT("ResourceImage\\TestField_1024x768.png"))))
	assert(!"테스트필드 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	*/
	/*
	if (FAILED(BGI.Load(TEXT("ResourceImage\\High_Field-GoldDragon.png"))))
	assert(!"필드 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	*/
	if (FAILED(BackGroundImage.Load(TEXT("ResourceImage\\BattleStage-Forest.png"))))
		assert(!"스테이지 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (BackGroundImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Background Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Main]->SetBackgroundImage(&BackGroundImage);
	if (FAILED(UserInterfaceWindowImage.Load(TEXT("ResourceImage\\UI_Window_1024x768.png"))))
		assert(!"UI 창 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (UserInterfaceWindowImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("UI Window Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Main]->SetUIWindowImage(&UserInterfaceWindowImage);
	if (FAILED(GaugeImage.Load(TEXT("ResourceImage\\Gauge_Bar.png"))))
		assert(!"게이지 바 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (GaugeImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("GaugeBar Image Load Fail"), MB_OK);
	arrScene[CBaseScene::SceneTag::Main]->SetGaugeImage(&GaugeImage);

	arrScene[CBaseScene::SceneTag::Main]->SetPlayer(m_ppPlayer[m_pNetwork->m_myid]);
	arrScene[CBaseScene::SceneTag::Main]->BuildObjects();
	//m_ppPlayer[m_pNetwork->m_myid]->SetID(m_pNetwork->m_myid);
}

void CFramework::BuildPlayer()
{
	if (FAILED(PlayerImage.Load(TEXT("ResourceImage\\Player001_SwordMan.png"))))
		assert(!"플레이어 이미지 파일이 제대로 로드되지 않았습니다!\n경로나 이름, 파일을 확인해주세요.");
	if (PlayerImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Player Image Load Fail"), MB_OK);
	// PlayerImage.Create(CLIENT_WIDTH, CLIENT_HEIGHT, 24);	// DC 만들어주는 버그 펑펑 터지는 함수

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
	// 플레이어를 조작하지 않는 화면에서는 해당 씬의 ProcessInput 함수를 실행
	if (GetKeyboardState(pKeysBuffer) && m_pCurrScene) bProcessedByScene = m_pCurrScene->ProcessInput(pKeysBuffer);
	// ProcessInput 함수의 결과값에 따라 들어오는 제어문
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
//							//printf("%d번 플레이어에게 공격!\n", j);
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
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("궁서체"), RGB(0, 255, 127));
				else
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("궁서체"), RGB(255, 0, 0));
			}
		}
#else
		char buf[100];
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				sprintf(buf, TEXT("ID %d"), i);
				UserInterfaceWindowImage.Draw(m_hDC, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 20, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 17, 100, 17, 0, 0, 1024, 768);
				if (i == m_pNetwork->m_myid)
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("궁서체"), RGB(0, 255, 127));
				else
					DrawFont(buf, m_ppPlayer[i]->GetPosition().x - m_ppPlayer[i]->GetWindowLB().w - m_ppPlayer[i]->GetSize().width - 10, m_ppPlayer[i]->GetPosition().y - m_ppPlayer[i]->GetWindowLB().h - m_ppPlayer[i]->GetSize().height / 2 - 15, 15, 100, TEXT("궁서체"), RGB(255, 0, 0));
			}
		}
#endif

		if (m_pCurrScene) m_pCurrScene->UserInterface_Render(m_hDC);

#ifdef UNICODE	
		DrawFont(TEXT("HP"), 25, 30, 15, 0, TEXT("궁서체"), RGB(255, 0, 0));
		wsprintf(buf, TEXT("%d/%d"), m_ppPlayer[m_pNetwork->m_myid]->GetHP(), MAX_HP);
		DrawFont(buf, 375, 30, 15, 0, TEXT("궁서체"), RGB(255, 0, 0));
		wsprintf(buf, TEXT("Your ID : %d"), m_pNetwork->m_myid);
		DrawFont(buf, 25, 75, 15, 0, TEXT("궁서체"), RGB(0, 255, 255));
		wsprintf(buf, TEXT("Score : %d"), m_ppPlayer[m_pNetwork->m_myid]->GetScore());
		DrawFont(buf, 250, 75, 15, 0, TEXT("궁서체"), RGB(0, 255, 0));
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				if (i == m_pNetwork->m_myid) continue;
				wsprintf(buf, TEXT("ID %d"), i);
				DrawFont(buf, 805, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("궁서체"), RGB(255, 127, 127));
				DrawFont(TEXT("Score"), 915, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("궁서체"), RGB(255, 255, 0));
				wsprintf(buf, TEXT("%d"), m_ppPlayer[i]->GetScore());
				DrawFont(buf, 915, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("궁서체"), RGB(255, 255, 0));
				if (GaugeImage) {
					GaugeImage.Draw(m_hDC, 805, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 100, 15, 9, 0, 1, 8);	// 100 == MAX_HP
					GaugeImage.Draw(m_hDC, 805, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), max(m_ppPlayer[i]->GetHP(), 1), 15, 0, 0, 9, 8);
				}
			}
		}
		//wsprintf(buf, TEXT("Time %d:%d"), static_cast<int>(m_CurrentTime) / 60, static_cast<int>(m_CurrentTime) % 60);
		//DrawFont(buf, 605, 20, 30, 0, TEXT("궁서체"), RGB(255, 255, 255));
#else
		DrawFont(TEXT("HP"), 25, 30, 15, 0, TEXT("궁서체"), RGB(255, 0, 0));
		sprintf(buf, TEXT("%d/%d"), m_ppPlayer[m_pNetwork->m_myid]->GetHP(), MAX_HP);
		DrawFont(buf, 375, 30, 15, 0, TEXT("궁서체"), RGB(255, 0, 0));
		sprintf(buf, TEXT("Your ID : %d"), m_pNetwork->m_myid);
		DrawFont(buf, 25, 75, 15, 0, TEXT("궁서체"), RGB(0, 255, 255));
		sprintf(buf, TEXT("Score : %d"), m_ppPlayer[m_pNetwork->m_myid]->GetScore());
		DrawFont(buf, 250, 75, 15, 0, TEXT("궁서체"), RGB(0, 255, 0));
		for (int i = 0; i < MAX_USER; ++i) {
			if (m_ppPlayer[i]) {
				if (i == m_pNetwork->m_myid) continue;
				sprintf(buf, TEXT("ID %d"), i);
				DrawFont(buf, 805, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("궁서체"), RGB(255, 127, 127));
				DrawFont(TEXT("Score"), 915, 255 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("궁서체"), RGB(255, 255, 0));
				sprintf(buf, TEXT("%d"), m_ppPlayer[i]->GetScore());
				DrawFont(buf, 915, 275 + (50 * ((i > m_pNetwork->m_myid) ? (i - 1) : i)), 10, 0, TEXT("궁서체"), RGB(255, 255, 0));
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
					DrawFont(buf, 1, 375, 75, 75, TEXT("궁서체"), RGB(255, 255, 127));
				}
	}
	else if (m_pCurrScene == arrScene[CBaseScene::SceneTag::Title]) {
#ifdef UNICODE	
		TCHAR buf[100];
		wsprintf(buf, TEXT("Lord of"));
		DrawFont(buf, 100, 75, 100, 100, TEXT("궁서체"), RGB(70, 255, 127));
		wsprintf(buf, TEXT("Slayers"));
		DrawFont(buf, 250, 275, 100, 100, TEXT("궁서체"), RGB(70, 255, 127));
		if (m_pCurrScene) m_pCurrScene->UserInterface_Render(m_hDC);
		wsprintf(buf, TEXT("LOGIN"));
		DrawFont(buf, 345, 575, 65, 50, TEXT("궁서체"), RGB(125, 100, 113));
#else
		char buf[100];
		sprintf(buf, TEXT("Lord of"));
		DrawFont(buf, 100, 75, 100, 100, TEXT("궁서체"), RGB(70, 255, 127));
		sprintf(buf, TEXT("Slayers"));
		DrawFont(buf, 300, 275, 100, 100, TEXT("궁서체"), RGB(70, 255, 127));
		if (m_pCurrScene) m_pCurrScene->UserInterface_Render(m_hDC);
		sprintf(buf, TEXT("LOGIN"));
		DrawFont(buf, 345, 575, 65, 50, TEXT("궁서체"), RGB(125, 100, 113));
#endif
	}

	
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

	case WM_KEYDOWN: case WM_KEYUP: case WM_CHAR:
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE) {
			// 비활성화
			self->SetUpdateTrigger(false);
		}
		else {
			// 활성화
			self->SetUpdateTrigger(true);
		}
	}
		self->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);	// 이 디바이스 컨텍스트를 사용하겠다.
			// hdc에서 이 함수를 통해 bitmap을 붙인다.

			// 픽메시지를 검사하고 트랜스메시지, 디스패치메시지에서 윈도우프로시저로 메시지를 넘겨준다.
			//self->OnDraw(hdc);
	
			::EndPaint(hWnd, &ps); // 그리고 지운다.
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