#include "stdafx.h"
#include "Object0-BaseObject.h"


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

bool CBaseObject::ObstacleRectCollide(CBaseObject * other)
{
	// 이 오브젝트와 충돌한 other 오브젝트를 원래대로 밀어냄
	while (RectCollide(other)) {
		if (m_v2dPosition.x < other->GetPosition().x - (other->GetSize().width / 2)) {
			other->SetPosition(m_v2dPosition.x + (m_v2dSize.width / 2) + (other->GetSize().width / 2), other->GetPosition().y);
			continue;
		}
		if (m_v2dPosition.x > other->GetPosition().x + (other->GetSize().width / 2)) {
			other->SetPosition(m_v2dPosition.x - (m_v2dSize.width / 2) - (other->GetSize().width / 2), other->GetPosition().y);
			continue;
		}
		if (m_v2dPosition.y > other->GetPosition().y + (other->GetSize().height / 2)) {
			other->SetPosition(other->GetPosition().x, m_v2dPosition.y - (m_v2dSize.height / 2) - (other->GetSize().height / 2));
			continue;
		}
		if (m_v2dPosition.y < other->GetPosition().y - (other->GetSize().height / 2)) {
			other->SetPosition(other->GetPosition().x, m_v2dPosition.y + (m_v2dSize.height / 2) + (other->GetSize().height / 2));
			continue;
		}
		return true;
	}
	return false;
}
