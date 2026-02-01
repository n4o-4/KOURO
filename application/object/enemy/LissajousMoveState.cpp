#include "LissajousMoveState.h"
#include "Enemy.h"

#include "AttackState.h"
#include "TackleChargeState.h"
#include "TackleState.h"

void LissajousMoveState::OnEnter(Enemy* enemy)
{
	transform_ = enemy->GetWorldTransform();
	time_ = 0.0f;
}

void LissajousMoveState::Update(Enemy* enemy)
{
	if (time_ > 5.0f)
	{
		std::unique_ptr<TackleChargeState> newState = std::make_unique<TackleChargeState>();
		enemy->ChangeState(std::move(newState));
		return;
	}

	time_ += 1.0f / 60.0f;
	offset_.x = sinf(kWidthAmp_ * time_);
	offset_.y = sinf(kHeightAmp_ * time_);
	transform_->SetTranslate(enemy->GetBasePosition() + offset_);
}

void LissajousMoveState::OnExit(Enemy* enemy)
{
	Kouro::WorldTransform* enemyTransform = enemy->GetWorldTransform();
	enemyTransform->SetTranslate(enemy->GetBasePosition());
}
