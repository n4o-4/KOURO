#include "ApproachState.h"
#include "Enemy.h"

void ApproachState::Enter(Enemy* enemy)
{
	Matrix4x4 targetWMatrix = enemy->GetTarget()->GetWorldTransform()->matWorld_;

	Vector3 pos = { targetWMatrix.m[3][0],targetWMatrix.m[3][1],targetWMatrix.m[3][2] };

	Vector3 forward = Normalize(pos - Vector3(enemy->GetWorldTransform()->matWorld_.m[3][0], enemy->GetWorldTransform()->matWorld_.m[3][1], enemy->GetWorldTransform()->matWorld_.m[3][2]));

	float backDistance = 5.0f;

	enemy->GetWorldTransform()->transform.translate = pos - forward * backDistance;
}

void ApproachState::Update(Enemy* enemy)
{
}

void ApproachState::Exit(Enemy* enemy)
{
}
