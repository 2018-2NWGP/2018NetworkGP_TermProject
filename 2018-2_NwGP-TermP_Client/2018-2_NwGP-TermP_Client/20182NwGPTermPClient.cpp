// 20182NwGPTermPClient.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "20182NwGPTermPClient.h"
#include "Framework.h"

#define MAX_LOADSTRING 100


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
CFramework myFramework;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_20182NWGPTERMPCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_20182NWGPTERMPCLIENT));

    MSG msg;

    // 기본 메시지 루프입니다.
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			if (msg.message == WM_QUIT) break; // esc 누르면 윈도우 종료.

			TranslateMessage(&msg);	// 메시지 이동
			DispatchMessage(&msg);	// 메시지 파견
		}
		// 여기서 프레임을 통해 프로그램을 돌려야 한다.
		// FrameAdvance를 통해 프로그램을 돌린다.
		myFramework.FrameAdvance();
	}
	
	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style =
		CS_HREDRAW		// 클라이언트의 너비를 변경하면, 전체 윈도우를 다시 그리게 한다.	WM{_SIZE를 통해 조작.
		| CS_VREDRAW	// 클라이언트의 높이를 변경하면, 전체 윈도우를 다시 그리게 한다.
	//  | CS_DBLCLKS	// 해당 윈도우에서 더블클릭을 사용해야 한다면 추가해야 한다.
		;
    wcex.lpfnWndProc    = CFramework::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_20182NWGPTERMPCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	// MAKEINTRESOURCEW(IDC_20182NWGPTERMPCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// 메인 윈도우 핸들
	HWND hWnd;

	// 윈도우 스타일
	DWORD dwStyle =
		WS_OVERLAPPED			// 디폴트 윈도우. 타이틀 바와 크기 조절이 안되는 경계선을 가진다. 아무런 스타일도 주지 않으면 이 스타일이 적용된다.
		| WS_CAPTION			// 타이틀 바를 가진 윈도우를 만들며 WS_BORDER 스타일을 포함한다.
		| WS_SYSMENU			// 시스템 메뉴를 가진 윈도우를 만든다.
		| WS_MINIMIZEBOX		// 최소화 버튼을 만든다.
		| WS_BORDER				// 단선으로 된 경계선을 만들며 크기 조정은 할 수 없다.
								//	  | WS_THICKFRAME		// 크기 조정이 가능한 두꺼운 경계선을 가진다. WS_BORDER와 같이 사용할 수 없다.
		;						// 추가로 필요한 윈도우 스타일은 http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm 참고

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	RECT getWinSize;
	GetWindowRect(GetDesktopWindow(), &getWinSize);

	// 클라이언트 사이즈
	RECT rc;
	rc.left = rc.top = 0;
	rc.right = CLIENT_WIDTH;
	rc.bottom = CLIENT_HEIGHT;
	// 윈도우 사이즈에 실제로 추가되는 (캡션, 외곽선 등) 크기를 보정.
	AdjustWindowRect(&rc, dwStyle, FALSE);

	// 클라이언트 절대 좌표(left, top)
	// 데스크톱의 중앙에 클라이언트가 위치하도록 설정
	POINT ptClientWorld;
	ptClientWorld.x = (getWinSize.right - CLIENT_WIDTH) / 2;
	ptClientWorld.y = (getWinSize.bottom - CLIENT_HEIGHT) / 2;

	hWnd = CreateWindowW(
		szWindowClass			// 윈도우 클래스 명.	레지스터클래스로 도장을 파고, 이 함수의 첫 인자로 도장을 찾는다.
		, szTitle				// 캡션 표시 문자열.	2번째 인자는 캡션으로, 타이틀 이름이다.
		, dwStyle				// 윈도우 스타일		3번째 플래그는 msdn 참조
		, ptClientWorld.x		// 부모 윈도우 기반 윈도우 시작좌표 : x		화면의 좌상단 x
		, ptClientWorld.y		// 부모 윈도우 기반 윈도우 시작좌표 : y		화면의 좌상단 y
		, rc.right - rc.left	// 윈도우 사이즈 : width		실제 만드는 창의 가로 크기
		, rc.bottom - rc.top	// 윈도우 사이즈 : height		실제 만드는 창의 세로 크기
		, nullptr				// 부모 윈도우.
		, nullptr				// 메뉴 핸들
		, hInstance				// 인스턴스 핸들	// (프로세스 주소)
		, nullptr);				// 추가 파라메타 : NULL

								// 생성 실패시 프로그램 종료   
	if (!hWnd) return FALSE;
	if (!myFramework.OnCreate(hInstance, hWnd, rc)) return FALSE;
	// 윈도우 표시
	ShowWindow(hWnd, nCmdShow); // 윈도우 창을 보여준다. 이게 없으면 윈도우 창이 안 보이기 떄문에 활성화가 안되므로 아무것도 못한다. 종료 조차도!
								// 확인 : WnbdProc의 default msg handler가 DefWindowProc
								// 함수를 반환하는가?
	UpdateWindow(hWnd);

	// 성공 반환
	return TRUE;
}

/*
//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
switch (message)
{
case WM_COMMAND:
{
int wmId = LOWORD(wParam);
// 메뉴 선택을 구문 분석합니다.
switch (wmId)
{
case IDM_ABOUT:
DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
break;
case IDM_EXIT:
DestroyWindow(hWnd);
break;
default:
return DefWindowProc(hWnd, message, wParam, lParam);
}
}
break;
case WM_PAINT:
{
PAINTSTRUCT ps;
HDC hdc = BeginPaint(hWnd, &ps);
// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
EndPaint(hWnd, &ps);
}
break;
case WM_DESTROY:
PostQuitMessage(0);
break;
default:
return DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
*/