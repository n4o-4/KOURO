#include "RandomMoveState.h"

#include "AttackState.h"
#include "Enemy.h"

void RandomMoveState::OnEnter(Enemy* enemy)
{
	randomGenerate.Initialize();

	stateTimer_ = randomGenerate.Random(2.0f, 3.0f);
}

void RandomMoveState::Update(Enemy* enemy)
{
	stateTimer_ -= 1.0f / 60.0f;

	if (stateTimer_ <= 0.0f)
	{
		std::unique_ptr<AttackState> newState = std::make_unique<AttackState>();
		enemy->ChangeState(std::move(newState));
	}

	Kouro::WorldTransform* enemyTransform = enemy->GetWorldTransform();

	Kouro::Vector3 velocity = randomGenerate.RandomVector3(-0.3f, 0.3f);

	enemyTransform->transform.translate += velocity;
}

void RandomMoveState::OnExit(Enemy* enemy)
{
}
