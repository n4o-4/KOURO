#pragma once

class Enemy; // �O���錾

class EnemyState
{
	public:
	virtual ~EnemyState() = default;

	virtual void Enter(Enemy* enemy) = 0;  // ��Ԃɓ���Ƃ�
	virtual void Update(Enemy* enemy) = 0; // ��Ԃ̍X�V
	virtual void Exit(Enemy* enemy) = 0;   // ��Ԃ���o��Ƃ�
};