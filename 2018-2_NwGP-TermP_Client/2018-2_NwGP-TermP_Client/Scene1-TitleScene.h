#pragma once
#include "Scene0-BaseScene.h"


class TitleScene : public CBaseScene
{
private:
	unsigned char m_TitleButtonStateIndex = 0;	// ��ư �̹��� �迭�� �ε����̴�. ��ư ���� ���콺�� �ø��� 1, ��ư�� Ŭ���ϸ� 2, �� �ܿ� 0���� Set���ش�.

public:
	TitleScene();
	~TitleScene();

	void BuildObjects() override;
	void Render(HDC hdc) override;
	void Update(float fTimeElapsed) override;

	void SetButtonState(unsigned char index) { if (index >= 0 || index < 3) m_TitleButtonStateIndex = index; }

	bool ProcessInput(unsigned char* KeyBuffer) override;
	void UserInterface_Render(HDC hdc) override;
};

