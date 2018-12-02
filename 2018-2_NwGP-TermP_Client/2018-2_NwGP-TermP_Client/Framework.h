#pragma once

#include "Scene0-BaseScene.h"
#include "Network.h"
// 헤더파일에는 이 프레임워크에서 사용하는 인터페이스나 클래스의 부모 외에는 include하지 않도록 한다.

template<typename Enum> // enum class의 선언형을 알려주며 인자와 대응하는 값을 반환하는 함수
inline constexpr auto GetFnumValueByType(Enum enumerator) noexcept	// enum class E : int {a, b, c}; 일 때,
{																	// auto data = GetEnumValueByType(E::a);
	return static_cast<std::underlying_type_t<enum>>(enummerator);	// data의 형식은 int이고, 값은 0
}
// enum class는 enum을 class로 만든 것이다. 이렇게하면 enum이 자료형이 되어버리므로 같은 enum class 외엔 넣을 수 없다.

// #define GetSceneEnumInt(Enum) GetEnumValueType(CScene::CurrentScene::Enum)	// 아직 CScene을 선언하지 않았으므로 주석처리

class PlayerObject;	// 클래스의 존재만 알려주고 포인터 형태로 멤버변수를 선언 후, Framework.cpp에서 헤더를 선언

class CFramework
{
private:
	HINSTANCE	m_hInstance{ NULL };

	HWND		m_hWnd{ NULL };
	RECT		m_rcClient{ 0, 0, 0, 0 };	// 클라이언트 크기

	HBITMAP		m_hBitmapBackBuffer = NULL;	// 비트맵 핸들. 윈도우 API가 제공하는 버퍼다. 윈도우에 띄우는 버퍼.
	HDC			m_hDC = NULL;				// 윈도우 핸들

	COLORREF	m_clrBackBuffer = 0x00000000;	// 백버퍼 색깔. 검정으로 초기화
	HBRUSH		m_hbrBackground = NULL;			// 그리는 도구. 색칠할 떄 쓴다.

	std::chrono::system_clock::time_point m_current_time;
	std::chrono::duration<double> m_timeElapsed;	// 시간이 얼마나 지났는가?
	double m_fps;

	TCHAR m_CaptionTitle[TITLE_MX_LENGTH];	// SetWindow 함수에다가 이 타이틀을 집어넣는다.
	int m_TitleLength;

	PlayerObject * m_pPlayer = nullptr;
	PlayerObject** m_ppPlayer{ NULL };
	int m_nPlayer = MAX_USER;


	CImage PlayerImage;
	CImage BGI;

	
	CNetwork* m_pNetwork;

public:
	CFramework();
	~CFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hWnd, const RECT &rc, CNetwork* pNetwork); // rc는 윈도우크기이고, m_rcClient에 저장한다.
	void CreatebackBuffer();	// HBITMAP을 만든다.
	void BuildScene();
	void BuildPlayer();
	void ReleaseScene();	

	bool OnDestroy()
	{
		if (m_hbrBackground) ::DeleteObject(m_hbrBackground);

		::SelectObject(m_hDC, NULL);
		if (m_hBitmapBackBuffer)  ::DeleteObject(m_hBitmapBackBuffer);
		if (m_hDC) ::DeleteDC(m_hDC);
		return false;
	}	// HDC로 만든 버퍼가 있으면 릴리즈를 한다.

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	HRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void Update(float fTimeElapsed);
	
	void SetBKColor(COLORREF color);	// COLORREF 멤버변수를 바꿔주는 함수
	void ClearBackgroundColor();		// 백버퍼 초기화
	void PreprocessingForDraw();		// 설정된 백버퍼에 실질적으로 그리는것
	void OnDraw(HDC hDC);				// Paint에서 Bitblt 단 하나만 한다.

//	void ChangeScene(CScene::CurrentScene tag, bool bDestroy = false);	// 마찬가지로 CScene을 모르므로 주석처리

	void FrameAdvance();				// Update와 PreProceessingForDraw를 처리한다. InvalidateRect도 하고 캡션도 해주고...
										// 윈도우 메시지 아래에 불리는 함수가 얘다. 모든 로직이 이 안에서 처리되기 떄문이다.

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void ChangeScene(CBaseScene::SceneTag tag);

	void RecvPacket();

	void SetMyPlayer(int id) { m_ppPlayer[m_pNetwork->m_myid] = m_ppPlayer[id]; }
private:
	CBaseScene * arrScene[CBaseScene::SceneTag::Count];
	CBaseScene * m_pCurrScene;
};