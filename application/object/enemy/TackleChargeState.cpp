#include "TackleChargeState.h"
#include "Enemy.h"

#include "TackleState.h"

void TackleChargeState::OnEnter(Enemy* enemy)
{
	//enemy->SetDrawDummy(true);
	randomGenerate.Initialize();
	Kouro::Vector3 spawnPos = enemy->GetWorldPosition();
	enemy->SetDummyPosition(spawnPos);
	Kouro::Vector3 targetPos = enemy->GetTarget()->GetWorldPosition();
	forward_ = Kouro::Normalize(targetPos - spawnPos);

	float yaw = std::atan2(forward_.x, forward_.z);
	float pitch = std::asin(-forward_.y);
	float roll = 0.0f;

	forward_ = { pitch, yaw, roll };

	transform_ = enemy->GetWorldTransform();
}

void TackleChargeState::Update(Enemy* enemy)
{
	if (timer_ > 1.0f)
	{
		std::unique_ptr<TackleState> newState = std::make_unique<TackleState>();
		enemy->ChangeState(std::move(newState));
		return;
	}

	// タイマーを加算
	timer_ += 1.0f / 60.0f;

	// 徐々に角度を変化させる
	transform_->SetRotate(Kouro::Lerp(transform_->transform.rotate, forward_, timer_ / 1.0f));

	offset_ = randomGenerate.RandomVector3(-0.2f, 0.2f);

	transform_->SetTranslate(enemy->GetBasePosition() + offset_);
}

void TackleChargeState::OnExit(Enemy* enemy)
{
	Kouro::WorldTransform* enemyTransform = enemy->GetWorldTransform();
	enemyTransform->SetTranslate(enemy->GetBasePosition());
}
