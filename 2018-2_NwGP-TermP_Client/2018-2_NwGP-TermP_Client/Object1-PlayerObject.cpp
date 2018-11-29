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

		walkAnimation_runtime += (WalkMotionSpeed * fTimeElapsed);
		WalkMotion = static_cast<unsigned char>(walkAnimation_runtime) % 4;
		WalkFrame = (WalkMotion < 3) ? WalkMotion : 1;

		if (directionBit == 0)
			m_State = idle;
	}
		break;
	case melee_attack:
		attackAnimation_runtime += (AttackMotionSpeed * fTimeElapsed);
		AttackFrame = static_cast<unsigned char>(attackAnimation_runtime) % 3;
		if (AttackFrame > 1) {
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
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v, GetSize().height, GetSize().width, GetSize().height);
			break;
		case 2: case 5:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v, 0, GetSize().width, GetSize().height);
			break;
		case 4:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v, GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 6:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v, GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 7:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * 4, GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 9:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * 4, GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 1:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * 4, 0, GetSize().width, GetSize().height);
			break;
		case 3:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * 4, GetSize().height, GetSize().width, GetSize().height);
			break;
		default:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v, 0, GetSize().width, GetSize().height);
			break;
		}
	}
		break;
	case walking:
	{
		switch (dirrection)
		{
		case 8:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * WalkFrame, GetSize().height, GetSize().width, GetSize().height);
			break;
		case 2: case 5:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * WalkFrame, 0, GetSize().width, GetSize().height);
			break;
		case 4:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * WalkFrame, GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 6:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * WalkFrame, GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 7:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), GetSize().height * 2, GetSize().width, GetSize().height);
			break;
		case 9:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), GetSize().height * 3, GetSize().width, GetSize().height);
			break;
		case 1:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), 0, GetSize().width, GetSize().height);
			break;
		case 3:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v * (WalkFrame + 3), GetSize().height, GetSize().width, GetSize().height);
			break;
		default:
			GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width, GetSize().height, GetSize().v, 0, GetSize().width, GetSize().height);
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
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 6, GetSize().height, GetSize().width * 2, GetSize().height);
				break;
			}		
			break;
		case 2: case 5:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 6, 0, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 4:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 32, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 6, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 32, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 6, GetSize().height * 2, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 6:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 6, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 6, GetSize().height * 3, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 7:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 5, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, 0, GetSize().height * 6, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 9:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 4, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 7, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 12, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, 0, GetSize().height * 7, GetSize().width * 2, GetSize().height);
				break;
			}			
			break;
		case 1:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 24, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 4, GetSize().height * 7, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, 0, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			}
			break;
		case 3:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 5, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, 0, GetSize().height * 5, GetSize().width * 2, GetSize().height);
				break;
			}
			break;			
		default:
			switch (AttackFrame)
			{
			case 0:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 2, GetSize().height * 4, GetSize().width * 2, GetSize().height);
				break;
			case 1:
				GetImage()->Draw(hdc, GetPosition().x - window_left - 16, GetPosition().y - window_bottom, GetSize().width * 2, GetSize().height, GetSize().v * 6, 0, GetSize().width * 2, GetSize().height);
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