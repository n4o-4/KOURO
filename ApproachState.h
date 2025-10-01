#pragma once
#include "EnemyState.h"

class ApproachState : public EnemyState
{
	public:
		void Enter(Enemy* enemy) override;
		void Update(Enemy* enemy) override;
		void Exit(Enemy* enemy) override;
};