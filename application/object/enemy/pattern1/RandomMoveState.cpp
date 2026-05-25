#include "RandomMoveState.h"

#include "AttackState.h"
#include "Enemy.h"

void RandomMoveState::OnEnter(Enemy* enemy)
{
	randomGenerate.Initialize();

	stateTimer_ = randomGenerate.Random(5.0f, 5.0f);
}

void RandomMoveState::Update(Enemy* enemy)
{
	stateTimer_ -= 1.0f / 60.0f;

	if (stateTimer_ <= 0.0f)
	{
		std::unique_ptr<AttackState> newState = std::make_unique<AttackState>();
		enemy->ChangeState(std::move(newState));
		return;
	}

	Kouro::WorldTransform* enemyTransform = enemy->GetWorldTransform();

	Kouro::Vector3 acceleration = randomGenerate.RandomVector3(-0.2f, 0.2f);

	velocity_ += acceleration;

	enemyTransform->SetTranslate(enemy->GetBasePosition() + velocity_);
}

void RandomMoveState::OnExit(Enemy* enemy)
{
	Kouro::WorldTransform* enemyTransform = enemy->GetWorldTransform();
	enemyTransform->SetTranslate(enemy->GetBasePosition());
}
