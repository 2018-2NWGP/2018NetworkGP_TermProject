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
	if (!in) std::cout << "�浹 üũ�� ���� ������Ʈ ������ �������� �ʽ��ϴ�.\n";
	else {
		int i = 0;
		while (in >> x >> y >> w >> h) {
			if (i > m_nObjects - 1)
				break;
			m_ppObjects[i] = new CBaseObject();
			m_ppObjects[i]->SetPosition(x, y);
			m_ppObjects[i]->SetSize(w, h);
			printf("���� �ε� : %d ��° �浹 ������Ʈ ��ġ : (%d, %d) / ũ�� : (%d, %d) ����\n", i++, x, y, w, h);
		}
	}
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
			for (int j = 0; j < MAX_USER; ++j) {
				double distance = m_ppPlayer[j]->GetMovingSpeed() * fTimeElapsed;
				if (distance < 1.0) distance = 1.0;	// �ּҺ���, 1 �̸��� ���� ��� �ȼ� ��ǥ�� �ٲ��� �ʱ� ����
				while (m_ppObjects[i]->ObstacleRectCollide(m_ppPlayer[j], distance)) {					
					
				}
				//printf("�÷��̾� : (x, y) : (%d, %d), ���� : %d, ���� : %d\n", m_ppPlayer[i]->GetPosition().x, m_pPlayer->GetPosition().y, m_pPlayer->GetSize().w, m_pPlayer->GetSize().h);
				//printf("%d��° ��ü�� �÷��̾ �浹. - (x, y) : (%d, %d), ���� : %d, ���� : %d\n", i, m_ppObjects[i]->GetPosition().x, m_ppObjects[i]->GetPosition().y, m_ppObjects[i]->GetSize().w, m_ppObjects[i]->GetSize().h);
			}
		}
}


bool CMainScene::ProcessInput(unsigned char* KeyBuffer)
{
	return false;
}
