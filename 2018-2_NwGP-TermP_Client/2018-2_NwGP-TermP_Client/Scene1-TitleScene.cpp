#include "stdafx.h"
#include "Scene1-TitleScene.h"


TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}

void TitleScene::BuildObjects()
{
}

void TitleScene::Render(HDC hdc)
{
	GetBackgroundImage()->Draw(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
}

void TitleScene::Update(float fTimeElapsed)
{
}

bool TitleScene::ProcessInput(unsigned char * KeyBuffer)
{
	return false;
}

void TitleScene::UserInterface_Render(HDC hdc)
{
	if (GetButtonImageArray()) {
		if (GetButtonImageArray()[m_TitleButtonStateIndex]) {
			switch (m_TitleButtonStateIndex) {
			case 0:
				GetButtonImageArray()[m_TitleButtonStateIndex].Draw(hdc, 325, 550, 375, 100, 0, 0, 1307, 598);
				break;
			case 1:
				GetButtonImageArray()[m_TitleButtonStateIndex].Draw(hdc, 310, 542, 405, 117, 0, 0, 1413, 704);
				break;
			case 2:
				GetButtonImageArray()[m_TitleButtonStateIndex].Draw(hdc, 325, 550, 375, 100, 0, 0, 1307, 598);
				break;
			}
		}
	}
}

bool TitleScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool TitleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{	
	case WM_MOUSEMOVE:		
		if ((LOWORD(lParam) > 325 && HIWORD(lParam) > 550) && (LOWORD(lParam) < 700 && HIWORD(lParam) < 650)) {
			m_TitleButtonStateIndex = 1;
			if (buttonPushed) m_TitleButtonStateIndex = 2;
		}
		else 
			m_TitleButtonStateIndex = 0;
		//if (buttonPushed) m_TitleButtonStateIndex = 2;
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		m_TitleButtonStateIndex = 0;
		if ((LOWORD(lParam) > 325 && HIWORD(lParam) > 550) && (LOWORD(lParam) < 700 && HIWORD(lParam) < 650)) {
			m_TitleButtonStateIndex = 2;
			buttonPushed = true;
		}		
		else buttonPushed = false;
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		m_TitleButtonStateIndex = 0;
		if ((LOWORD(lParam) > 325 && HIWORD(lParam) > 550) && (LOWORD(lParam) < 700 && HIWORD(lParam) < 650)) {
			m_TitleButtonStateIndex = 1;
			if (buttonPushed) return true;
		}		
		else buttonPushed = false;
		break;
	default:
		m_TitleButtonStateIndex = 0;
		break;
	}
	return false;
}

HRESULT TitleScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return E_NOTIMPL;
}
