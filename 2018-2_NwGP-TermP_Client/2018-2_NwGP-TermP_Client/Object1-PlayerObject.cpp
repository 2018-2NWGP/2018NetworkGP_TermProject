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
	window_left = clamp(0, int(GetPosition().x) - CLIENT_WIDTH / 2, int(GetBackgroundSize().width - CLIENT_WIDTH));
	window_bottom = clamp(0, int(GetPosition().y) - CLIENT_HEIGHT / 2, int(GetBackgroundSize().height - CLIENT_HEIGHT));
	//printf("%d, %d\n", m_State, directionBit);
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
		double distance = m_dMoveSpeed * fTimeElapsed;
		if (distance < 1.0) distance = 1.0;	// 최소보정, 1 미만의 값인 경우 픽셀 좌표가 바뀌지 않기 때문
		if (directionBit & DIR_UP) {
			dirrection = 8;
		}
		if (directionBit & DIR_DOWN) {
			dirrection = 2;
		}
		if (directionBit & DIR_LEFT) {
			dirrection = 4;
		}
		if (directionBit & DIR_RIGHT) {
			dirrection = 6;
		}
		if ((directionBit & DIR_UP) && (directionBit & DIR_LEFT)) dirrection = 7;
		if ((directionBit & DIR_UP) && (directionBit & DIR_RIGHT)) dirrection = 9;
		if ((directionBit & DIR_DOWN) && (directionBit & DIR_LEFT)) dirrection = 1;
		if ((directionBit & DIR_DOWN) && (directionBit & DIR_RIGHT)) dirrection = 3;

		walkAnimation_runtime += (m_fWalkMotionSpeed * fTimeElapsed);
		WalkMotion = static_cast<unsigned char>(walkAnimation_runtime) % 4;
		WalkFrame = (WalkMotion < 3) ? WalkMotion : 1;

		if (directionBit == 0) {
			m_SetIdle = true;
			m_State = idle;
		}
	}
		break;
	case melee_attack:
		attackAnimation_runtime += (m_fAttackMotionSpeed * fTimeElapsed);
		AttackFrame = static_cast<unsigned char>(attackAnimation_runtime) % 3;
		if (AttackFrame > 1) {
			m_SetIdle = true;
			m_State = idle;
		}			
		break;
	default:
		break;
	}
}

void PlayerObject::Render(HDC hdc)
{
	// GetImage()->TransparentBlt(hdc, 50, 50, 256, 128, 0, 0, 1024, 768);	// 특정 영역을 특정 옵션을 통해 투명화처리하는 함수
	switch (m_State) {
	case idle:
	{
		switch (dirrection)
		{
		case 8:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v, GetSize().height, GetSize().width, GetSize().height);
			break;
		case 2: case 5:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v, 0, GetSize().width, GetSize().height);
			break;
		case 4:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v, GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 6:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v, GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 7:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * 4, GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 9:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * 4, GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 1:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * 4, 0, GetSize().width, GetSize().height);
			break;
		case 3:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * 4, GetSize().height, GetSize().width, GetSize().height);
			break;
		default:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v, 0, GetSize().width, GetSize().height);
			break;
		}
	}
	break;
	case walking:
	{
		switch (dirrection)
		{
		case 8:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * WalkFrame, GetSize().height, GetSize().width, GetSize().height);
			break;
		case 2: case 5:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * WalkFrame, 0, GetSize().width, GetSize().height);
			break;
		case 4:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * WalkFrame, GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 6:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * WalkFrame, GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 7:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 9:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 1:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), 0, GetSize().width, GetSize().height);
			break;
		case 3:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), GetSize().height, GetSize().width, GetSize().height);
			break;
		default:
			GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width, GetSize().height, GetSize().v, 0, GetSize().width, GetSize().height);
			break;
		}
	}
	break;
	case melee_attack:
	{
		switch (dirrection)
		{
		case 8:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 6, GetSize().height, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 2: case 5:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 6, 0, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 4:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 32 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 6, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 32 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 6, GetSize().height * 2, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 6:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 6, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 6, GetSize().height * 3, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 7:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 5, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, 0, GetSize().height * 6, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 9:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 4 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 7, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 12 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, 0, GetSize().height * 7, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 1:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 24 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 7, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, 0, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 3:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 5, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, 0, GetSize().height * 5, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		default:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16 - (GetSize().width / 2), GetPosition().y - window_bottom - (GetSize().height / 2), GetSize().width * 2, GetSize().height, GetSize().v * 6, 0, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		}
	}
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
