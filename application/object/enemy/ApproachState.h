#pragma once
#include "EnemyState.h"

// \brief ApproachState 接近状態クラス

class ApproachState : public EnemyState
{
	public:
		void Enter(Enemy* enemy) override;
		void Update(Enemy* enemy) override;
		void Exit(Enemy* enemy) override;
};