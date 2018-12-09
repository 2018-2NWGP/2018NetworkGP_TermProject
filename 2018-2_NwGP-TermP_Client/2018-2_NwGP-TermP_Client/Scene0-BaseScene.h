#pragma once
#include "Object0-BaseObject.h"

class Framework;
class PlayerObject;

class CBaseScene
{
public: 
	// CScene::SceneTag:: �������� �ҷ��� �ܿ� �ʿ䵵 define�� �ʿ䵵 ���� ���� ���빰�� �� �� �ִ�. �� ������ϸ� include ����.
	enum SceneTag {
		Title
		, Main
		, Count
	};
public:
	CBaseScene();
	CBaseScene(SceneTag tag, Framework* pFramework)
	{
		m_Tag = tag;
		m_pFramework = pFramework;
		//m_pNetwork = pNetwork;
	}
	virtual ~CBaseScene();	

	// �����Լ��� ����ƽ����̶� ������ ���� ������ �� �� �ִ�. �����Լ� ���ο� �����Լ��� ������ �ȴ�.

	virtual void BuildObjects() = 0;
	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render(HDC hdc) = 0;
	virtual void UserInterface_Render(HDC hdc) = 0;

	virtual bool ProcessInput(unsigned char* KeyBuffer) = 0;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual HRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;

	virtual void SetBackgroundImage(CImage* img) { m_BackGroundImage = img; }
	virtual CImage* GetBackgroundImage() const { return m_BackGroundImage; }
	virtual void SetUIWindowImage(CImage* UIWindowImage) { m_UserInterfaceWindow = UIWindowImage; }
	virtual CImage* GetUIWindowImage() const { return m_UserInterfaceWindow; }
	virtual void SetGaugeImage(CImage* GaugeImage) { m_GaugeImage = GaugeImage; }
	virtual CImage* GetGaugeImage() const { return m_GaugeImage; }
	virtual void SetButtonImageArray(CImage * ButtonImageArray) { m_ButtonImage = ButtonImageArray; }
	virtual CImage* GetButtonImageArray() const { return m_ButtonImage; }

	void SetPlayer(PlayerObject* pPlayer) { m_pPlayer = pPlayer; }

protected:
	SceneTag m_Tag;
	Framework* m_pFramework;	// ���ڷ� �޾ƿ��°� �ƴ϶� �����Ҵ��� �ؿ´�.
	CImage* m_BackGroundImage = nullptr;
	CImage* m_GaugeImage = nullptr;
	CImage* m_UserInterfaceWindow = nullptr;
	CImage* m_ButtonImage = nullptr;

	PlayerObject * m_pPlayer = nullptr;
	//CNetwork* m_pNetwork{ NULL };
};