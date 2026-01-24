#pragma once
#include "BaseCharacter.h"
class Enemy; // 前方宣言

// \brief EnemyState 敵の状態基底クラス

class EnemyState
{
	public:
	virtual ~EnemyState() = default;

	virtual void OnEnter(Enemy* enemy) = 0;  // 状態に入るとき
	virtual void Update(Enemy* enemy) = 0; // 状態の更新
	virtual void OnExit(Enemy* enemy) = 0;   // 状態から出るとき
};