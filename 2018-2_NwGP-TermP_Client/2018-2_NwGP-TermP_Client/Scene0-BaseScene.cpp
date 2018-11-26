#include "stdafx.h"
#include "Object1-PlayerObject.h"
#include "Scene0-BaseScene.h"
#include "Network.h"

CBaseScene::CBaseScene()
{
}

CBaseScene::~CBaseScene()
{
}

void CBaseScene::SetMyPlayerImage(CImage * img)
{
	if (m_pNetwork && m_ppPlayer) m_ppPlayer[m_pNetwork->m_myid]->SetImage(img);
}

void CBaseScene::SetPlayerImageAll(CImage * img)
{
	for (int i = 0; i < MAX_USER; ++i)
		if (m_ppPlayer[i])
			m_ppPlayer[i]->SetImage(img);
}

void CBaseScene::SetPlayerImageByID(unsigned int id, CImage * img)
{
	if (m_pNetwork && m_ppPlayer[id]) m_ppPlayer[m_pNetwork->m_myid]->SetImage(img);
}

