#include "stdafx.h"
#include "Object0-BaseObject.h"
#include "Object1-PlayerObject.h"

CBaseObject::CBaseObject()
{
}


CBaseObject::~CBaseObject()
{
	if (m_v2dBackgroundSize && BGAllocated)
		delete m_v2dBackgroundSize;
}

void CBaseObject::Update(float fTimeElapsed)
{
}

void CBaseObject::Render(HDC hdc)
{
}

bool CBaseObject::RectCollide(Vec2i position, Vec2i size)
{
	if (m_v2dPosition.x - (m_v2dSize.width / 2) >= position.x + (size.width / 2))
		return false;
	if (m_v2dPosition.x + (m_v2dSize.width / 2) <= position.x - (size.width / 2))
		return false;
	if (m_v2dPosition.y - (m_v2dSize.height / 2) >= position.y + (size.height / 2))
		return false;
	if (m_v2dPosition.y + (m_v2dSize.height / 2) <= position.y - (size.height / 2))
		return false;
	return true;
}

bool CBaseObject::RectCollide(CBaseObject * other)
{
	if (m_v2dPosition.x - (m_v2dSize.width / 2) >= other->GetPosition().x + (other->GetSize().width / 2))
		return false;
	if (m_v2dPosition.x + (m_v2dSize.width / 2) <= other->GetPosition().x - (other->GetSize().width / 2))
		return false;
	if (m_v2dPosition.y - (m_v2dSize.height / 2) >= other->GetPosition().y + (other->GetSize().height / 2))
		return false;
	if (m_v2dPosition.y + (m_v2dSize.height / 2) <= other->GetPosition().y - (other->GetSize().height / 2))
		return false;
	return true;
}

bool CBaseObject::ObstacleRectCollide(PlayerObject * player, double distance)
{
	/* 이 오브젝트와 충돌한 other 오브젝트를 원래대로 밀어냄 */
	/*
	while (RectCollide(player)) {
		if (m_v2dPosition.x < player->GetPosition().x - (player->GetSize().width / 2)) {
			player->SetPosition(m_v2dPosition.x + (m_v2dSize.width / 2) + (player->GetSize().width / 2), player->GetPosition().y);
			continue;
		}
		if (m_v2dPosition.x > player->GetPosition().x + (player->GetSize().width / 2)) {
			player->SetPosition(m_v2dPosition.x - (m_v2dSize.width / 2) - (player->GetSize().width / 2), player->GetPosition().y);
			continue;
		}
		if (m_v2dPosition.y > player->GetPosition().y + (player->GetSize().height / 2)) {
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y - (m_v2dSize.height / 2) - (player->GetSize().height / 2));
			continue;
		}
		if (m_v2dPosition.y < player->GetPosition().y - (player->GetSize().height / 2)) {
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y + (m_v2dSize.height / 2) + (player->GetSize().height / 2));
			continue;
		}
	}*/
	/*
	if (RectCollide(player)) {
		switch (player->GetDirection()) {
		case 8:
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y + (m_v2dSize.height / 2) + (player->GetSize().height / 2));
			break;
		case 2:
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y - (m_v2dSize.height / 2) - (player->GetSize().height / 2));
			break;
		case 4:
			player->SetPosition(m_v2dPosition.x + (m_v2dSize.width / 2) + (player->GetSize().width / 2), player->GetPosition().y);
			break;
		case 6:
			player->SetPosition(m_v2dPosition.x - (m_v2dSize.width / 2) - (player->GetSize().width / 2), player->GetPosition().y);
			break;
		case 7:
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y + (m_v2dSize.height / 2) + (player->GetSize().height / 2));
			player->SetPosition(m_v2dPosition.x + (m_v2dSize.width / 2) + (player->GetSize().width / 2), player->GetPosition().y);
			break;
		case 9:
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y + (m_v2dSize.height / 2) + (player->GetSize().height / 2));
			player->SetPosition(m_v2dPosition.x - (m_v2dSize.width / 2) - (player->GetSize().width / 2), player->GetPosition().y);
			break;
		case 1:
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y - (m_v2dSize.height / 2) - (player->GetSize().height / 2));
			player->SetPosition(m_v2dPosition.x + (m_v2dSize.width / 2) + (player->GetSize().width / 2), player->GetPosition().y);
			break;
		case 3:
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y - (m_v2dSize.height / 2) - (player->GetSize().height / 2));
			player->SetPosition(m_v2dPosition.x - (m_v2dSize.width / 2) - (player->GetSize().width / 2), player->GetPosition().y);
			break;
		}
		return true;
	}
	*/
	/*
	while (RectCollide(player)) {
		if (player->GetDirectionBit() & DIR_UP) {
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y + (m_v2dSize.height / 2) + (player->GetSize().height / 2));
			continue;
		}
		if (player->GetDirectionBit() & DIR_DOWN) {
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y - (m_v2dSize.height / 2) - (player->GetSize().height / 2));
			continue;
		}
		if (player->GetDirectionBit() & DIR_LEFT) {
			player->SetPosition(m_v2dPosition.x + (m_v2dSize.width / 2) + (player->GetSize().width / 2), player->GetPosition().y);
			continue;
		}
		if (player->GetDirectionBit() & DIR_RIGHT) {
			player->SetPosition(m_v2dPosition.x - (m_v2dSize.width / 2) - (player->GetSize().width / 2), player->GetPosition().y);
			continue;
		}
		return true;
	}
	*/
	/*
	if (RectCollide(player)) {
		if (player->GetDirectionBit() & DIR_UP) {
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y + (m_v2dSize.height / 2) + (player->GetSize().height / 2));
			return true;
		}
		if (player->GetDirectionBit() & DIR_DOWN) {
			player->SetPosition(player->GetPosition().x, m_v2dPosition.y - (m_v2dSize.height / 2) - (player->GetSize().height / 2));
			return true;
		}
		if (player->GetDirectionBit() & DIR_LEFT) {
			player->SetPosition(m_v2dPosition.x + (m_v2dSize.width / 2) + (player->GetSize().width / 2), player->GetPosition().y);
			return true;
		}
		if (player->GetDirectionBit() & DIR_RIGHT) {
			player->SetPosition(m_v2dPosition.x - (m_v2dSize.width / 2) - (player->GetSize().width / 2), player->GetPosition().y);
			return true;
		}
	}
	*/
	if (RectCollide(player)) {
		switch (player->GetDirection()) {
		case 8:
			player->SetPosY(min(static_cast<int>(player->GetPosition().y + distance), player->GetBackgroundSize().height - (player->GetSize().height / 2)));
			break;
		case 2:
			player->SetPosY(max(static_cast<int>(player->GetPosition().y - distance), (player->GetSize().height / 2)));
			break;
		case 4:
			player->SetPosX(min(static_cast<int>(player->GetPosition().x + distance), player->GetBackgroundSize().width - (player->GetSize().width / 2)));
			break;
		case 6:
			player->SetPosX(max(static_cast<int>(player->GetPosition().x - distance), (player->GetSize().width / 2)));
			break;
		case 7:
			if (this->GetPosition().x + this->GetSize().width > player->GetPosition().x - (player->GetSize().width / 2)) {
				player->SetPosX(min(static_cast<int>(player->GetPosition().x + distance), player->GetBackgroundSize().width - (player->GetSize().width / 2)));
				//return true;
			}
			if (this->GetPosition().y + this->GetSize().height > player->GetPosition().y - (player->GetSize().height / 2)) {
				player->SetPosY(min(static_cast<int>(player->GetPosition().y + distance), player->GetBackgroundSize().height - (player->GetSize().height / 2)));
				//return true;
			}
			break;
		case 9:
			if (this->GetPosition().x - this->GetSize().width < player->GetPosition().x + (player->GetSize().width / 2)) {
				player->SetPosX(max(static_cast<int>(player->GetPosition().x - distance), (player->GetSize().width / 2)));
				//return true;
			}
			if (this->GetPosition().y + this->GetSize().height > player->GetPosition().y - (player->GetSize().height / 2)) {
				player->SetPosY(min(static_cast<int>(player->GetPosition().y + distance), player->GetBackgroundSize().height - (player->GetSize().height / 2)));
				//return true;
			}
			break;
		case 1:
			if (this->GetPosition().x + this->GetSize().width > player->GetPosition().x - (player->GetSize().width / 2)) {
				player->SetPosX(min(static_cast<int>(player->GetPosition().x + distance), player->GetBackgroundSize().width - (player->GetSize().width / 2)));
				//return true;
			}
			if (this->GetPosition().y - this->GetSize().height < player->GetPosition().y + (player->GetSize().height / 2)) {
				player->SetPosY(max(static_cast<int>(player->GetPosition().y - distance), (player->GetSize().height / 2)));
				//return true;
			}
			break;
		case 3:
			if (this->GetPosition().x - this->GetSize().width < player->GetPosition().x + (player->GetSize().width / 2)) {
				player->SetPosX(max(static_cast<int>(player->GetPosition().x - distance), (player->GetSize().width / 2)));
				//return true;
			}
			if (this->GetPosition().y - this->GetSize().height < player->GetPosition().y + (player->GetSize().height / 2)) {
				player->SetPosY(max(static_cast<int>(player->GetPosition().y - distance), (player->GetSize().height / 2)));
				//return true;
			}
			break;
		}
		return true;
	}
	return false;
}