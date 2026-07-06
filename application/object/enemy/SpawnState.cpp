#include "SpawnState.h"

// engine
#include "Easing.h"

// application
#include "Enemy.h"

#include "RandomMoveState.h"

void SpawnState::OnEnter(Enemy* enemy)
{
	// 敵の位置を基本位置に設定
	enemy->SetPosition(enemy->GetBasePosition());

	// 敵のスケールを初期スケールに設定
	enemy->SetScale(kInitialScale_);
}

void SpawnState::Update(Enemy* enemy)
{
	// タイマーを更新
	timer_ += 1.0f / 60.0f;

	if(timer_ >= kSpawnDuration_)
	{
		// 出現時間が経過したら次の状態に遷移
		enemy->ChangeState(std::make_unique<RandomMoveState>());
		return;
	}
	
	// 出現時間に応じて補間値を計算
	float t = timer_ / kSpawnDuration_;

	// イージング関数を適用
	t = Kouro::Easing::EaseOutBack(t,kOvershoot_);
	
	// スケールを補間
	Kouro::Vector3 scale = kBaseScale_ * t;

	enemy->SetScale(scale);
}

void SpawnState::OnExit(Enemy* enemy)
{
}