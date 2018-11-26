#include "stdafx.h"
#include "Scene1-MainGameScene.h"
#include "Object1-PlayerObject.h"

CMainScene::CMainScene()
{
}


CMainScene::~CMainScene()
{
}

void CMainScene::BuildObjects()
{
}

void CMainScene::Render(HDC hdc)
{
	GetBackgroundImage()->Draw(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, m_pPlayer->GetWindowLB().w, m_pPlayer->GetWindowLB().h, CLIENT_WIDTH, CLIENT_HEIGHT);
}

void CMainScene::Update(float fTimeElapsed)
{
	for (int i = 0; i < m_nObjects; ++i)
		if (m_ppObjects[i])
			m_ppObjects[i]->Update(fTimeElapsed);
}

bool CMainScene::ProcessInput(unsigned char* KeyBuffer)
{
	return false;
}
