#include "TackleState.h"
#include "Enemy.h"
#include "TackleRecoverState.h"

void TackleState::OnEnter(Enemy* enemy)
{
	Kouro::Vector3 spawnPos = enemy->GetWorldPosition();
	Kouro::Vector3 targetPos = enemy->GetTarget()->GetWorldPosition();
	direction_ = Kouro::Normalize(targetPos - spawnPos);
}

void TackleState::Update(Enemy* enemy)
{
	timer_ += 1.0f / 60.0f;
	if(timer_ > duration_)
	{
		enemy->ChangeState(std::make_unique<TackleRecoverState>());
		return;
	}

	Kouro::Vector3 newPos = enemy->GetMoveOffset() + direction_ * speed_ * (1.0f / 60.0f);

	enemy->SetMoveOffset(newPos);
}

void TackleState::OnExit(Enemy* enemy)
{
	enemy->Destroy(Enemy::DestroyReason::TimeOut);
}
