#include "LockOnSystem.h"

#include "IdleState.h"

void LockOnSystem::Initialize(Reticle* reticle)
{
	// クロスヘアのポインタを保存
	reticle_ = reticle;

	// 初期状態は待機中
	ChangeState(std::make_unique<IdleState>(reticle_));
}

void LockOnSystem::Update(std::vector<Enemy*> enemies)
{
	enemies_ = enemies;

	// ロックオン状態の更新
	if (state_)
	{
		state_->Update(enemies);
	}
}

void LockOnSystem::ChangeState(std::unique_ptr<ILockOnState> newState)
{
	// 現在の状態を終了
	if (state_)
	{
		state_->OnExit();
	}
	// 新しい状態に変更
	state_ = std::move(newState);

	// 新しい状態の開始処理
	if (state_)
	{
		state_->OnEnter();
	}
}