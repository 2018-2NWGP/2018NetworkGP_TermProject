#pragma once
#include "Object0-BaseObject.h"

class PlayerObject : public CBaseObject
{
private:
	DWORD directionBit = 0;	 // �ܺο��� �޾ƿͼ� �����ϱ� ���� �����Ʈ ����
	unsigned char dirrection = 0;	// ������ ��� ���� �ִ��� ��������� ���� �� ���
	unsigned int m_id = 0;
	int m_nHP = 100;	
	int m_nAttackDamage = 15;
	unsigned int m_nScore = 0;

	double m_dMoveSpeed = 100.0;	// �̵��ӵ�
	float m_fWalkMotionSpeed = 5.0f;// �̵� �ִϸ��̼� �ӵ�
	unsigned char WalkFrame = 0;	// �ȴ� �� ��� �̹����� �������� �׸� �� ���
	unsigned char WalkMotion = 0;	// �ȴ� �̹��� �������� �������� ���� ����
	
	float m_fAttackMotionSpeed = 10.0f;	// ���� �ִϸ��̼� �ӵ�
	unsigned char AttackFrame = 0;	// ���� �� ��� �̹����� �������� �׸� �� ���

	float walkAnimation_runtime = 0.0f;		// �ȴ� �ִϸ��̼� �ӵ��� �����ϱ� ���� ����
	float attackAnimation_runtime = 0.0f;	// ���� �ִϸ��̼� �ӵ��� �����ϱ� ���� ����
	
	int m_nMeleeAttackRange = 16;	// ���� �浹�ڽ��� �߻��� ���� ��ġ�� �����ϱ� ���� ���ݻ�Ÿ� ����
	Vec2i m_v2dMeleeAttachReach = { 32, 32 };	// ���� �浹�ڽ��� ���μ��� ũ�⸦ �������� ����

	// �� ��ũ�Ѹ��� ���Ͽ� ���� ��ǥ�� ȭ�� ��ǥ�� �ٲ��ֱ� ���� ����
	int window_left = 0;
	int window_bottom = 0;

	ObjectState m_State = idle;


public:
	PlayerObject();
	virtual ~PlayerObject();

	virtual void Update(float fTimeElapsed = 1);

	void SetDirectionBit(DWORD dwDir) { directionBit = dwDir; }
	DWORD GetDirectionBit() const { return directionBit; }
	unsigned char GetDirection() const { return dirrection; }

	void SetMovingSpeed(double speed) { m_dMoveSpeed = speed; }
	double GetMovingSpeed() const { return m_dMoveSpeed; }
	void SetAttackSpeed(float speed) { m_fAttackMotionSpeed = speed; }

	void SetHP(int hp) { m_nHP = hp; }
	void HitByDamage(int damage) { m_nHP -= damage; }
	int GetHP() const { return m_nHP; }

	void SetID(unsigned int id) { m_id = id; }
	unsigned int GetID() const { return m_id; }

	int GetAttackDamage() const { return m_nAttackDamage; }

	void SetState(ObjectState state) { m_State = state; }
	ObjectState GetState() { return m_State; }

	void CenterPlayerScrolling() {
		window_left = clamp(0, int(GetPosition().x) - CLIENT_WIDTH / 2, int(GetBackgroundSize().width - CLIENT_WIDTH));
		window_bottom = clamp(0, int(GetPosition().y) - CLIENT_HEIGHT / 2, int(GetBackgroundSize().height - CLIENT_HEIGHT));
	}
	void OtherScolling(CBaseObject * Center) {
		window_left = clamp(0, int(Center->GetPosition().x) - CLIENT_WIDTH / 2, int(GetBackgroundSize().width - CLIENT_WIDTH));
		window_bottom = clamp(0, int(Center->GetPosition().y) - CLIENT_HEIGHT / 2, int(GetBackgroundSize().height - CLIENT_HEIGHT));
	}
	Vec2i GetWindowLB() { return { (unsigned int)window_left, (unsigned int)window_bottom }; }

	bool RectAttackCollide(CBaseObject* Target);
};

