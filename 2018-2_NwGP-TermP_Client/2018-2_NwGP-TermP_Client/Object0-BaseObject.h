#pragma once

// 모든 오브젝트의 기본 정보를 담고 있는 부모 클래스.
// 이 클래스를 상속하여 오브젝트를 파생시킨다.
class CBaseObject
{
private:
	Vec2i m_v2dPosition{ 0, 0 };
	Vec2i m_v2dSize{ 0, 0 };
	Vec2i * m_v2dBackgroundSize = nullptr;	// 벨트스크롤링을 하기 위한 백그라운드 크기 저장
	CImage * Obj_Image = nullptr;

	bool BGAllocated = false;	// 백그라운드 크기를 동적할당으로 저장중인지 여부를 판가름하는 flag 변수
public:
	Vector3D<unsigned char> m_vColor{ 0, 0, 0 };

	CBaseObject();
	CBaseObject(unsigned int x, unsigned int y) : m_v2dPosition{ x, y } { CBaseObject::CBaseObject(); }
	virtual ~CBaseObject();

	virtual void Update(float fTimeElapsed);
	virtual void Render(HDC hdc);

	void SetPosition(unsigned int x = 0, unsigned int y = 0) { m_v2dPosition = { x, y }; }
	void SetPosition(Vec2i position) { m_v2dPosition = position; }
	void SetPosX(unsigned int x) { m_v2dPosition.x = x; }
	void SetPosY(unsigned int y) { m_v2dPosition.y = y; }
	void SetSize(unsigned int w = 0, unsigned int h = 0) { m_v2dSize = { w, h }; }
	void SetSize(Vec2i size) { m_v2dSize = size; }
	void SetBackgroundSize(unsigned int w = 0, unsigned int h = 0) {
		if (!m_v2dBackgroundSize) { m_v2dBackgroundSize = new Vec2i{ w, h }; BGAllocated = true; }
		else *m_v2dBackgroundSize = { w, h };
	}	
	void SetBackgroundSize(Vec2i& BG_Size) {
		if (!m_v2dBackgroundSize) { m_v2dBackgroundSize = new Vec2i(BG_Size); BGAllocated = true; }
		else *m_v2dBackgroundSize = BG_Size;
	}
	void SetBackgroundSize(void * BG_Size = nullptr) { 
		if (m_v2dBackgroundSize && BGAllocated) {
			delete m_v2dBackgroundSize;
			BGAllocated = false;
		}
		if (!BG_Size) m_v2dBackgroundSize = static_cast<Vector2D<unsigned int>*>(BG_Size); 
	}

	void SetImage(CImage * Image) { Obj_Image = Image; }

	Vec2i GetPosition() const { return m_v2dPosition; }
	Vec2i GetSize() const { return m_v2dSize; }
	Vec2i GetBackgroundSize() const { return *m_v2dBackgroundSize; }
	CImage* GetImage() const { return Obj_Image; }

	bool RectCollide(Vec2i position, Vec2i size);
	bool RectCollide(CBaseObject * other);
};

