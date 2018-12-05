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
			//ERROR::::::왜 이게 주석처리이면 충돌체크가 안됨?
			printf("파일 로드 : %d 번째 충돌 오브젝트 위치 : (%d, %d) / 크기 : (%d, %d) 생성\n", i++, x, y, w, h);
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
				if (distance < 1.0) distance = 1.0;	// 최소보정, 1 미만의 값인 경우 픽셀 좌표가 바뀌지 않기 때문
				while (m_ppObjects[i]->ObstacleRectCollide(m_ppPlayer[j], distance));
			}
		}
	bool exitOuterLoop = false;
	for (int i = 0; i < MAX_USER; ++i) {
		for (int j = 0; j < MAX_USER; ++j) {
			if (m_ppPlayer[i]->GetState() == melee_attack && i != j) {
				if (m_ppPlayer[i]->RectAttackCollide(m_ppPlayer[j])) {
//#ifdef USE_CONSOLE_WINDOW
				//	printf("%d번 플레이어가 %d번 플레이어에게 공격!\n", i, j);
//#endif
					m_ppPlayer[j]->HitByDamage(m_ppPlayer[i]->GetAttackDamage());
					m_ppPlayer[i]->SetScore(m_ppPlayer[i]->GetScore() + 3);
					m_ppPlayer[i]->SetState((ObjectState)idle);
					SC_Msg_Set_HP_Score p;
					p.Character_id = j;
					p.HIT_id = i;
					p.hp = m_ppPlayer[j]->GetHP();
					p.score = m_ppPlayer[i]->GetScore();
					p.size = sizeof(p);
					p.type = SC_SET_HP_SCORE;
					for (int k = 0; k < MAX_USER; ++k)
					{
						if (g_clients[k].m_isconnected) {
							int retVal = send(g_clients[k].m_s, (char*)&p, sizeof(p), 0);
						}
					}
					//printf("%d가 %d에게 쳐맞음\n", p.Character_id, p.HIT_id);
					break;
					exitOuterLoop = true;
				}
			}
		}
		if (exitOuterLoop) break;
	}
}


bool CMainScene::ProcessInput(unsigned char* KeyBuffer)
{
	return false;
}
