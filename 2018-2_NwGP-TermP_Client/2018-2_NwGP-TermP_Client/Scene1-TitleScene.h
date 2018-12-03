#pragma once
#include "Scene0-BaseScene.h"


class TitleScene : public CBaseScene
{
private:
	unsigned char m_TitleButtonStateIndex = 0;	// 버튼 이미지 배열의 인덱스이다. 버튼 위에 마우스를 올리면 1, 버튼을 클릭하면 2, 그 외엔 0으로 Set해준다.

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

