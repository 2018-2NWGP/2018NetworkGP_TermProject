#pragma once
#include "Scene0-BaseScene.h"

class CMainScene : public CBaseScene
{
public:
	CMainScene();
	~CMainScene();

	void BuildObjects() override;
	void Render(HDC hdc) override;
	void Update(float fTimeElapsed) override;

	bool ProcessInput(unsigned char* KeyBuffer) override;
	void UserInterface_Render(HDC hdc) override;

private:

	CBaseObject** m_ppObjects;
	int m_nObjects;
};