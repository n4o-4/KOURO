#include "SceneObjectManager.h"

void SceneObjectManager::Update()
{
	player_->Update();

	for (auto& enemy : enemies_)
	{
		// 有効でないキャラクターは更新しない
		if (!enemy->IsActive()) continue;
		enemy->Update();
	}

	for(auto& bullet : bullets_)
	{
		// 有効でない弾は更新しない
		if (!bullet->IsActive()) continue;
		bullet->Update();
	}
}

void SceneObjectManager::CleanupObjects()
{
	auto isDead = [](const auto& object)
		{
			return !object || !object->IsValid();
		};

	std::erase_if(enemies_, isDead);
	std::erase_if(bullets_, isDead);
}

void SceneObjectManager::Draw()
{
	player_->Draw();

	for(auto& enemy : enemies_)
	{
		if(!enemy->IsActive())continue;
		enemy->Draw();
	}

	for(auto& bullet : bullets_)
	{
		if(!bullet->IsActive()) continue;
		bullet->Draw();
	}
}

void SceneObjectManager::RegisterPlayer(std::unique_ptr<Player> player)
{
	// プレイヤーを追加
	player_ = std::move(player);
}

void SceneObjectManager::RegisterEnemy(std::unique_ptr<Enemy> enemy)
{
	// キャラクターを追加
	enemies_.push_back(std::move(enemy));
}

void SceneObjectManager::RegisterBullet(std::unique_ptr<BaseBullet> bullet)
{
	// 弾を追加
	bullets_.push_back(std::move(bullet));
}
