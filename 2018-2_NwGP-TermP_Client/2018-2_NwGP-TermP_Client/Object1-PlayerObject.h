#pragma once
#include "Object0-BaseObject.h"

class PlayerObject : public CBaseObject
{
private:
	DWORD directionBit = 0;	 // �ܺο��� �޾ƿͼ� �����ϱ� ���� �����Ʈ ����
	unsigned char dirrection = 0;	// ������ ��� ���� �ִ��� ��������� ���� �� ���
	unsigned int m_id = 0;
	int m_nHP = 100;	
	int m_nAttack = 15;
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
	bool m_SetIdle = false;

public:
	PlayerObject();
	virtual ~PlayerObject();

	virtual void Update(float fTimeElapsed = 1);
	virtual void Render(HDC hdc);

	void SetDirectionBit(DWORD dwDir) { directionBit = dwDir; }
	unsigned char GetDirection() const { return dirrection; }
	
	void SetMovingSpeed(double speed) { m_dMoveSpeed = speed; }
	void SetAttackSpeed(float speed) { m_fAttackMotionSpeed = speed; }

	void SetID(unsigned int id) { m_id = id; }
	unsigned int GetID() const { return m_id; }

	int GetAttack() const { return m_nAttack; }

	void SetState(ObjectState state) { m_State = state; }
	ObjectState GetState() const { return m_State; }

	void SetIdleState() { m_SetIdle = false; }
	bool GetIdleState() const { return m_SetIdle; }

	Vec2i GetWindowLB() { return { (unsigned int)window_left, (unsigned int)window_bottom }; }

	bool RectAttackCollide(CBaseObject* Target);
};