#include "stdafx.h"
#include "Scene1-MainGameScene.h"
#include "Object1-PlayerObject.h"
#include "Network.h"

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
	if (m_ppPlayer[m_pNetwork->m_myid])
		GetBackgroundImage()->Draw(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, 
			m_ppPlayer[m_pNetwork->m_myid]->GetWindowLB().width, 
			m_ppPlayer[m_pNetwork->m_myid]->GetWindowLB().height, 
			CLIENT_WIDTH, CLIENT_HEIGHT);
}

void CMainScene::Update(float fTimeElapsed)
{
	if (m_ppPlayer[m_pNetwork->m_myid])
		m_ppPlayer[m_pNetwork->m_myid]->Update(fTimeElapsed);
	// printf("x : %.2lf, y : %.2lf\n", m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y);

	for (int i = 0; i < m_nObjects; ++i)
		if (m_ppObjects[i])
			m_ppObjects[i]->Update(fTimeElapsed);
}

bool CMainScene::ProcessInput(unsigned char* KeyBuffer)
{
	if (m_ppPlayer[m_pNetwork->m_myid]) {
		DWORD dwDirection = 0;
		if (KeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_UP;
		if (KeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_DOWN;
		if (KeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (KeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if ((KeyBuffer['a'] & 0xF0) || (KeyBuffer['A'] & 0xF0)) m_ppPlayer[m_pNetwork->m_myid]->SetState(melee_attack);
		m_ppPlayer[m_pNetwork->m_myid]->SetDirection(dwDirection);
	}
	return true;
}

void CMainScene::BuildPlayer()
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution<> randW(32, 4800);
	std::uniform_int_distribution<> randH(64, 3200);

	if (!m_ppPlayer[m_pNetwork->m_myid]) {
		m_ppPlayer[m_pNetwork->m_myid] = new PlayerObject();
		m_ppPlayer[m_pNetwork->m_myid]->SetPosition(randW(dre), randH(dre));
		m_ppPlayer[m_pNetwork->m_myid]->SetSize(32, 64);
		m_ppPlayer[m_pNetwork->m_myid]->SetBackgroundSize(4800, 3200);
	}
}
