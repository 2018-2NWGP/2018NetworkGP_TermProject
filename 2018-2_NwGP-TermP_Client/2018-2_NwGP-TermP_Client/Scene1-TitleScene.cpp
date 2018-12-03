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
				GetButtonImageArray()[m_TitleButtonStateIndex].Draw(hdc, 325, 550, 405, 117, 0, 0, 1413, 704);
				break;
			case 2:
				GetButtonImageArray()[m_TitleButtonStateIndex].Draw(hdc, 325, 550, 375, 100, 0, 0, 1307, 598);
				break;
			}
		}
	}
}
