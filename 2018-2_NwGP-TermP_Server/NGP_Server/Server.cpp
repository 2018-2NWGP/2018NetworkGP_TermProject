// IDI_LEGENDOFDOODLES.cpp: ���� ���α׷��� �������� �����մϴ�.
//

#include "stdafx.h"
#include "NGP_Server.h"

#define MAX_LOADSTRING 100

// ���� ����:

				// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
BOOL                InitInstance();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(nCmdShow);


	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	MSG msg{ NULL };

	try
	{
		// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
		if (!InitInstance())
		{
			return FALSE;
		}
		printf("-----------------Server Online-----------------\n");

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else if (timer.Update())
			{
				g_Framework.FrameAdvance(timer.GetTimeElapsed());
			}
			if (!g_Framework.IsRunning()) break;
		}

		g_Framework.Finalize();
		for (auto& th : w_threads) th.join();
		a_thread.join();
		time_thread.join();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"hResult Error", MB_OK);
	}



	return (int)msg.wParam;
}

BOOL InitInstance()
{
	// Framework Initialize
	g_Framework.Initialize();
	NetworkInitialize();

	return TRUE;
}
