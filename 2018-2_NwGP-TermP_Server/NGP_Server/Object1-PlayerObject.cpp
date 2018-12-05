#include "stdafx.h"
#include "Object1-PlayerObject.h"


PlayerObject::PlayerObject()
{
}


PlayerObject::~PlayerObject()
{
}

void PlayerObject::Update(float fTimeElapsed)
{
	if (m_State != melee_attack) {
		AttackFrame = 0;
		attackAnimation_runtime = 0.0f;
	}
	switch (m_State) {
	case idle:
		if (directionBit != 0) m_State = walking;
		break;
	case walking:
	{
		double distance = m_dMoveSpeed * fTimeElapsed /* * 333333*/;
#ifdef USE_CONSOLE_WINDOW
		//printf("m_dMoveSpeed : %.10lf\n", m_dMoveSpeed);
		//printf("fTimeElapsed : %.10lf\n", fTimeElapsed);
		//printf("distance : %.10lf\n", distance);
#endif
		if (distance < 1.0) distance = 1.0;	// 최소보정, 1 미만의 값인 경우 픽셀 좌표가 바뀌지 않기 때문
		if (directionBit & DIR_UP) {
			SetPosY(max(static_cast<int>(GetPosition().y - distance), 0));
			dirrection = 8;
		}
		if (directionBit & DIR_DOWN) {
			SetPosY(min(static_cast<int>(GetPosition().y + distance), GetBackgroundSize().height - GetSize().height));
			dirrection = 2;
		}
		if (directionBit & DIR_LEFT) {
			SetPosX(max(static_cast<int>(GetPosition().x - distance), 0));
			dirrection = 4;
		}
		if (directionBit & DIR_RIGHT) {
			SetPosX(min(static_cast<int>(GetPosition().x + distance), GetBackgroundSize().width - GetSize().width));
			dirrection = 6;
		}
		if ((directionBit & DIR_UP) && (directionBit & DIR_LEFT)) dirrection = 7;
		if ((directionBit & DIR_UP) && (directionBit & DIR_RIGHT)) dirrection = 9;
		if ((directionBit & DIR_DOWN) && (directionBit & DIR_LEFT)) dirrection = 1;
		if ((directionBit & DIR_DOWN) && (directionBit & DIR_RIGHT)) dirrection = 3;

		walkAnimation_runtime += (m_fWalkMotionSpeed * fTimeElapsed);
		WalkMotion = static_cast<unsigned char>(walkAnimation_runtime) % 4;
		WalkFrame = (WalkMotion < 3) ? WalkMotion : 1;

		if (directionBit == 0)
			m_State = idle;
		break;
	}
		
	case melee_attack:
		attackAnimation_runtime += (m_fAttackMotionSpeed * fTimeElapsed);
		AttackFrame = static_cast<unsigned char>(attackAnimation_runtime) % 3;
		if (AttackFrame > 1) 
			m_State = idle;	

		break;
	default:
		break;
	}
}

bool PlayerObject::RectAttackCollide(CBaseObject * Target)
{
	CBaseObject * meleeAttackHitBox = new CBaseObject();
	switch (dirrection) {
	case 8:
		meleeAttackHitBox->SetPosition(GetPosition().x, GetPosition().y - m_nMeleeAttackRange);
		break;
	case 2:
		meleeAttackHitBox->SetPosition(GetPosition().x, GetPosition().y + m_nMeleeAttackRange);
		break;
	case 4:
		meleeAttackHitBox->SetPosition(GetPosition().x - m_nMeleeAttackRange, GetPosition().y);
		break;
	case 6:
		meleeAttackHitBox->SetPosition(GetPosition().x + m_nMeleeAttackRange, GetPosition().y);
		break;
	case 7:
		meleeAttackHitBox->SetPosition(GetPosition().x - (sqrt(0.5) * m_nMeleeAttackRange), GetPosition().y - (sqrt(0.5) * m_nMeleeAttackRange));
		break;
	case 9:
		meleeAttackHitBox->SetPosition(GetPosition().x + (sqrt(0.5) * m_nMeleeAttackRange), GetPosition().y - (sqrt(0.5) * m_nMeleeAttackRange));
		break;
	case 1:
		meleeAttackHitBox->SetPosition(GetPosition().x - (sqrt(0.5) * m_nMeleeAttackRange), GetPosition().y + (sqrt(0.5) * m_nMeleeAttackRange));
		break;
	case 3:
		meleeAttackHitBox->SetPosition(GetPosition().x + (sqrt(0.5) * m_nMeleeAttackRange), GetPosition().y + (sqrt(0.5) * m_nMeleeAttackRange));
		break;
	default:
		meleeAttackHitBox->SetPosition(GetPosition().x, GetPosition().y);
		break;
	}
	meleeAttackHitBox->SetSize(m_v2dMeleeAttachReach);
	if (Target)
		return Target->RectCollide(meleeAttackHitBox->GetPosition(), meleeAttackHitBox->GetSize());
	// return Target->RectCollide(meleeAttackHitBox);
	else
		return false;
}
