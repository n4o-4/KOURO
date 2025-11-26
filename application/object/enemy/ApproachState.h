#pragma once
#include "EnemyState.h"

// \brief ApproachState Ú‹ßó‘ÔƒNƒ‰ƒX

class ApproachState : public EnemyState
{
	public:
		void Enter(Enemy* enemy) override;
		void Update(Enemy* enemy) override;
		void Exit(Enemy* enemy) override;
};