#pragma once
#include "Object0-BaseObject.h"

class PlayerObject : public CBaseObject
{
private:
	DWORD directionBit = 0;	 // 외부에서 받아와서 연산하기 위한 방향비트 정보
	unsigned char dirrection = 0;	// 방향이 어딜 보고 있는지 명시적으로 저장 후 사용
	unsigned int m_id = 0;
	int m_nHP = 100;	
	int m_nAttack = 15;
	unsigned int m_nScore = 0;

	double m_dMoveSpeed = 100.0;	// 이동속도
	float m_fWalkMotionSpeed = 5.0f;// 이동 애니메이션 속도
	unsigned char WalkFrame = 0;	// 걷는 중 어느 이미지의 프레임을 그릴 지 사용
	unsigned char WalkMotion = 0;	// 걷는 이미지 프레임을 결정짓기 위한 변수
	
	float m_fAttackMotionSpeed = 10.0f;	// 공격 애니메이션 속도
	unsigned char AttackFrame = 0;	// 공격 중 어느 이미지의 프레임을 그릴 지 사용

	float walkAnimation_runtime = 0.0f;		// 걷는 애니메이션 속도를 제어하기 위한 변수
	float attackAnimation_runtime = 0.0f;	// 공격 애니메이션 속도를 제어하기 위한 변수
	
	int m_nMeleeAttackRange = 16;	// 공격 충돌박스가 발생할 중점 위치를 제어하기 위한 공격사거리 변수
	Vec2i m_v2dMeleeAttachReach = { 32, 32 };	// 공격 충돌박스의 가로세로 크기를 제어해줄 변수

	// 맵 스크롤링을 위하여 월드 좌표와 화면 좌표를 바꿔주기 위한 변수
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