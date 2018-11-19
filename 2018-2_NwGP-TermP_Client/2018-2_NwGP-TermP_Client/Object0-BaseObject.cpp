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
