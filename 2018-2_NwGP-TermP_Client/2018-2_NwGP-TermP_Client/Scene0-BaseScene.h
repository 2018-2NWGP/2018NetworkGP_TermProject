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
	}
	virtual ~CBaseScene();	
	/* CBaseScene�� ��ӹ��� Ÿ��Ʋ���� �Ҹ��ڵ� ~CBaseScene();�� �ȴ�. �׷� Ÿ��Ʋ���� �Ҹ��� �� �� �Ҹ��ڵ� �޾ƿ´�.
	�̹� �Ҹ��ڰ� �ִµ� Ÿ��Ʋ���� ���������� �Ű�Ἥ ���� �ʿ䰡 ����. ����, virtual�� �ٿ� ��ӹ޾� �Ҹ��Ų��.
	virtual Ÿ�� ���� Ŭ������ ������ �ö� ����, ���� �Ҹ��ڸ� ���� ȣ���ؼ� ��ӹ��� Ŭ������ �ѹ��� �Ҹ��Ű�� ���̴�.*/

	/*
	���� �ѹ� ���� std::vector�� ó���ϴ� ���Ͱ� �� �������� 10���� �̻� �Ѿ�� ���� �ȵȴ�.
	for(auto& p : list) p = &arr // ������� for�� ��뿹��
	*/

	// �����Լ��� ����ƽ����̶� ������ ���� ������ �� �� �ִ�. �����Լ� ���ο� �����Լ��� ������ �ȴ�.

	virtual void BuildObjects() = 0;
	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render(HDC hdc) = 0;

	virtual bool ProcessInput(unsigned char* KeyBuffer) = 0;
	
	virtual void SetBackgroundImage(CImage* img) { m_BackGroundImage = img; }
	virtual CImage* GetBackgroundImage() const { return m_BackGroundImage; }
	void SetPlayer(PlayerObject* pPlayer) { m_pPlayer = pPlayer; }
	
protected:
	SceneTag m_Tag;
	Framework* m_pFramework;	// ���ڷ� �޾ƿ��°� �ƴ϶� �����Ҵ��� �ؿ´�.
	CImage* m_BackGroundImage = nullptr;
	PlayerObject * m_pPlayer = nullptr;
};