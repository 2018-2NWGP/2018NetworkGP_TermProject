#include "stdafx.h"
#include "Scene1-MainGameScene.h"
#include "Object1-PlayerObject.h"
#include <fstream>

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
	for (int i = 0; i < m_nObjects; ++i) if (m_ppObjects[i]) delete m_ppObjects[i];
	if (m_ppObjects) delete m_ppObjects;
}

void CMainScene::BuildObjects()
{
	m_nObjects = 20;
	m_ppObjects = new CBaseObject*[m_nObjects];
	for (int i = 0; i < m_nObjects; ++i) m_ppObjects[i] = nullptr;
	std::ifstream in("ResourceImage\\BattleStage_Forest-Obstacle.txt");
	unsigned int x, y, w, h;
	if (!in) std::cout << "충돌 체크용 방해 오브젝트 파일이 존재하지 않습니다.\n";
	else {
		int i = 0;
		while (in >> x >> y >> w >> h) {
			if (i > m_nObjects - 1)
				break;
			m_ppObjects[i] = new CBaseObject();
			m_ppObjects[i]->SetPosition(x, y);
			m_ppObjects[i]->SetSize(w, h);
			printf("파일 로드 : %d 번째 충돌 오브젝트 위치 : (%d, %d) / 크기 : (%d, %d) 생성\n", i++, x, y, w, h);
		}
	}
}

void CMainScene::Render(HDC hdc)
{
	/*
	if (m_ppPlayer[m_pNetwork->m_myid])
		GetBackgroundImage()->Draw(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, 
			m_ppPlayer[m_pNetwork->m_myid]->GetWindowLB().width, 
			m_ppPlayer[m_pNetwork->m_myid]->GetWindowLB().height, 
			CLIENT_WIDTH, CLIENT_HEIGHT);
	*/
	GetBackgroundImage()->Draw(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT,m_pPlayer->GetWindowLB().w, m_pPlayer->GetWindowLB().h, CLIENT_WIDTH, CLIENT_HEIGHT);
}

void CMainScene::Update(float fTimeElapsed)
{
	/*
	if (m_ppPlayer[m_pNetwork->m_myid])
		m_ppPlayer[m_pNetwork->m_myid]->Update(fTimeElapsed);
	*/
	for (int i = 0; i < m_nObjects; ++i)
		if (m_ppObjects[i]) {
			m_ppObjects[i]->Update(fTimeElapsed);
			if (m_ppObjects[i]->ObstacleRectCollide(m_pPlayer)) {
				printf("플레이어 : (x, y) : (%d, %d), 가로 : %d, 세로 : %d\n", m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_pPlayer->GetSize().w, m_pPlayer->GetSize().h);
				printf("%d번째 객체와 플레이어가 충돌. - (x, y) : (%d, %d), 가로 : %d, 세로 : %d\n", i, m_ppObjects[i]->GetPosition().x, m_ppObjects[i]->GetPosition().y, m_ppObjects[i]->GetSize().w, m_ppObjects[i]->GetSize().h);
			}
		}
}

bool CMainScene::ProcessInput(unsigned char* KeyBuffer)
{
	/*
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
	*/
	return false;
}
