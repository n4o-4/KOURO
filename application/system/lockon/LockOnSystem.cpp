#include "LockOnSystem.h"

void LockOnSystem::Initialize(Reticle* reticle)
{
	reticle_ = reticle;
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
