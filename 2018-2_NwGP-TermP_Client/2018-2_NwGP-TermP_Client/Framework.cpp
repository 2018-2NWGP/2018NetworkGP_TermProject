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

bool CFramework::OnCreate(HINSTANCE hInstance, HWND hWnd, const RECT & rc)
{
	srand((unsigned int)time(NULL));
	m_hInstance = hInstance;
	m_hWnd = hWnd;
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
	lstrcpy(m_CaptionTitle, TITLESTRING);

#if defined(SHOW_CAPTIONFPS)
	lstrcat(m_CaptionTitle, TEXT("("));
#endif
	m_TitleLength = lstrlen(m_CaptionTitle);
	SetWindowText(m_hWnd, m_CaptionTitle);

	// Ÿ�̸� �ʱ�ȭ
//	m_LastUpdate_time = chrono::system_clock::now(); // �������� �ʾҴ�.
	m_current_time = std::chrono::system_clock::now();
	m_fps = 0;

	BuildPlayer();
	// �� ����
	BuildScene();
	// ������ ���� �����ΰ�?
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
	if (FAILED(BGI.Load(TEXT("ResourceImage\\High_Field-GoldDragon.png"))))
		assert(!"�ʵ� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");
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
		m_pPlayer->SetPosition(randW(dre), randH(dre));
		if (FAILED(PlayerImage.Load(TEXT("ResourceImage\\Player001_SwordMan.png"))))
			assert(!"�÷��̾� �̹��� ������ ����� �ε���� �ʾҽ��ϴ�!\n��γ� �̸�, ������ Ȯ�����ּ���.");		
		if (PlayerImage.IsNull())MessageBox(m_hWnd, TEXT("Fail"), TEXT("Player Image Load Fail"), MB_OK);
		// PlayerImage.Create(CLIENT_WIDTH, CLIENT_HEIGHT, 24);	// DC ������ִ� ���� ���� ������ �Լ�
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
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	// �÷��̾ �������� �ʴ� ȭ�鿡���� �ش� ���� ProcessInput �Լ��� ����
	if (GetKeyboardState(pKeysBuffer) && m_pCurrScene) bProcessedByScene = m_pCurrScene->ProcessInput(pKeysBuffer);
	// �÷��̾ �����ϴ� ȭ�鿡���� �Ʒ��� ��� ����
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
	m_hbrBackground = ::CreateSolidBrush(m_clrBackBuffer);	// �� ����� �귯���� ������.
}

void CFramework::ClearBackgroundColor()
{
	::FillRect(m_hDC, &m_rcClient, m_hbrBackground);	// ���� ������� ��׶��忡 ���� Ŭ���̾�Ʈ��ŭ�� ������ ��� ���� �귯���� ������ ��ĥ�ض�.
}

void CFramework::PreprocessingForDraw()
{
	ClearBackgroundColor();	// �׸��� ������ ���۵Ǹ� ������ �׶��带 �����ϰ� �����.
	::SetBkColor(m_hDC, TRANSPARENT);	// ���ĺ����� ��� �����ϰ� ����
	::SetStretchBltMode(m_hDC, COLORONCOLOR);	// ���� ������ �޶� �þ�ų� �پ�� ������ �ִ� ��� �����.

	m_pCurrScene->Render(m_hDC);
	m_pPlayer->Render(m_hDC);
}

void CFramework::OnDraw(HDC hDC)
{
	// ���ڷ� �޴� hDC�� �ۿ��� ������� hDC�̴�. �� ��Ʈ�ʿ� m_hDC�� ����ۿ��� ������� �׸��� �Ѹ���.
	::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, m_hDC, 0, 0, SRCCOPY);
}

void CFramework::FrameAdvance()
{
	//BuildPlayer();
	// Get tick
	m_timeElapsed = std::chrono::system_clock::now() - m_current_time;
	if (m_timeElapsed.count() > MAX_FRAMETIME)
	{
		m_current_time = std::chrono::system_clock::now();

		Update(m_timeElapsed.count());
		PreprocessingForDraw(); // ����� �����̹Ƿ� OnDraw�� �ƴϴ�. OnDraw ������ ����ۿ� �׷��ִ� ������ �Ѵ�.
		InvalidateRect(m_hWnd, &m_rcClient, FALSE);	// False�� �ʱ�ȭ�� ���� �ʴ´ٴ� ���̴�. ������ ������ �޽����� ȣ���Ѵ�.
		
		// WM_PAINT ����, ������ �޽����� ���� �����Ƿ� ���⼭ �׳� �������Ѵ�.
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(m_hWnd, &ps);
			OnDraw(hdc);
			::EndPaint(m_hWnd, &ps);
		}
		if (m_timeElapsed.count() > 0.0)
			m_fps = 1.0 / m_timeElapsed.count();
	}
	InvalidateRect(m_hWnd, &m_rcClient, FALSE);	// False�� �ʱ�ȭ�� ���� �ʴ´ٴ� ���̴�. ������ ������ �޽����� ȣ���Ѵ�.
	// ĸ�ǿ� ���ڸ� �� ������ �����ϴ� ĸ�� ��Ʈ�� ����
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


	case WM_KEYDOWN:
	case WM_KEYUP:
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