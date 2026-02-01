#include "TackleState.h"
#include "Enemy.h"
#include "TackleRecoverState.h"

void TackleState::OnEnter(Enemy* enemy)
{
	enemy->SetDrawDummy(true);
	Kouro::Vector3 spawnPos = enemy->GetWorldPosition();
	enemy->SetDummyPosition(spawnPos);
	Kouro::Vector3 targetPos = enemy->GetTarget()->GetWorldPosition();
	direction_ = Kouro::Normalize(targetPos - spawnPos);
	enemy->SetDummyVelocity(direction_);
}

void TackleState::Update(Enemy* enemy)
{
	timer_ += 1.0f / 60.0f;
	if(timer_ > duration_)
	{
		std::unique_ptr<TackleRecoverState> newState = std::make_unique<TackleRecoverState>();
		enemy->ChangeState(std::move(newState));
		return;
	}
}

void TackleState::OnExit(Enemy* enemy)
{
}
