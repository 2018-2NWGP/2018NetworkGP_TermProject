#pragma once
#include "Object0-BaseObject.h"

class Framework;
class PlayerObject;

class CBaseScene
{
public: 
	// CScene::SceneTag:: 형식으로 불러서 외울 필요도 define할 필요도 없이 안의 내용물을 쓸 수 있다. 이 헤더파일만 include 하자.
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
	/* CBaseScene을 상속받은 타이틀씬의 소멸자도 ~CBaseScene();이 된다. 그럼 타이틀씬이 소멸할 때 이 소멸자도 받아온다.
	이미 소멸자가 있는데 타이틀씬이 상위계층을 신경써서 지울 필요가 없다. 따라서, virtual을 붙여 상속받아 소멸시킨다.
	virtual 타고 상위 클래스의 끝까지 올라간 다음, 하위 소멸자를 전부 호출해서 상속받은 클래스를 한번에 소멸시키는 것이다.*/

	/*
	루프 한번 돌때 std::vector는 처리하는 벡터가 한 루프에서 10만개 이상 넘어가면 쓰면 안된다.
	for(auto& p : list) p = &arr // 범위기반 for문 사용예제
	*/

	// 람다함수는 스태틱취급이라 쓰레드 인자 안으로 들어갈 수 있다. 람다함수 내부에 가상함수를 넣으면 된다.

	virtual void BuildObjects() = 0;
	virtual void Update(float fTimeElapsed) = 0;

	virtual bool ProcessInput(unsigned char* KeyBuffer) = 0;
	
	virtual void SetBackgroundImage(CImage* img) { m_BackGroundImage = img; }
	virtual CImage* GetBackgroundImage() const { return m_BackGroundImage; }
	void SetPlayer(PlayerObject** ppPlayer) { m_ppPlayer = ppPlayer; }
	
protected:
	SceneTag m_Tag;
	Framework* m_pFramework;	// 인자로 받아오는게 아니라 동적할당을 해온다.
	CImage* m_BackGroundImage = nullptr;
	PlayerObject * m_pPlayer = nullptr;
	PlayerObject** m_ppPlayer{ NULL };
	//CNetwork* m_pNetwork{ NULL };
};