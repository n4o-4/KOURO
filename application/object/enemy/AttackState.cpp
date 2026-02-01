#include "AttackState.h"

#include "RandomMoveState.h"
#include "LissajousMoveState.h"
#include "Enemy.h"

void AttackState::OnEnter(Enemy* enemy)
{
	randomGenerate.Initialize();

	fireCount_ = static_cast<int>(randomGenerate.Random(1.0f, 4.0f));
}

void AttackState::Update(Enemy* enemy)
{
	// 
	if (chargeTimer_ < kChargeTime_)
	{
		chargeTimer_ += 1.0f / 60.0f;

		return;
	}


	if (fireIntervalTimer_ < kFireInterval_)
	{
		fireIntervalTimer_ += 1.0f / 60.0f;

		enemy->GetWorldTransform()->transform.scale = { 1.0f,1.0f,1.0f };

		return;
	}

	
	enemy->Fire();
	enemy->GetWorldTransform()->transform.scale *= 1.5f;
	fireIntervalTimer_ = 0.0f;
	--fireCount_;

	if (fireCount_ == 0)
	{
		std::unique_ptr<LissajousMoveState> newState = std::make_unique<LissajousMoveState>();
		enemy->ChangeState(std::move(newState));
	}
}

void AttackState::OnExit(Enemy* enemy)
{
	enemy->GetWorldTransform()->transform.scale = { 1.0f,1.0f,1.0f };
}
