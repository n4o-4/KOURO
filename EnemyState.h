#pragma once

class Enemy; // 前方宣言

class EnemyState
{
	public:
	virtual ~EnemyState() = default;

	virtual void Enter(Enemy* enemy) = 0;  // 状態に入るとき
	virtual void Update(Enemy* enemy) = 0; // 状態の更新
	virtual void Exit(Enemy* enemy) = 0;   // 状態から出るとき
};