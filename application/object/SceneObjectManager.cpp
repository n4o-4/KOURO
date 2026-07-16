#include "SceneObjectManager.h"

/// Application
#include "Player.h"
#include "Enemy.h"
#include "BaseBullet.h"
#include "SpawnRequestQueue.h"

/// Engine
#include "LineModelManager.h"

void SceneObjectManager::Initialize()
{
	spawnRequestQueue_ = std::make_unique<SpawnRequestQueue>();
}

void SceneObjectManager::Update()
{
	// プレイヤーの更新
	player_->Update();

	// 敵と弾の更新
	auto updateObjects = [](auto& objects)
		{
			for (auto& object : objects)
			{
				if (!object->IsActive()) continue;
				object->Update();
			}
		};

	enemyCount_ = static_cast<uint32_t>(enemies_.size());

	updateObjects(enemies_);

	ProcessSpawnRequests();

	updateObjects(bullets_);
}

void SceneObjectManager::CleanupObjects()
{
	auto countKill = [](const auto& object)
		{
			return object->GetDestroyReason() == Enemy::DestroyReason::CollisionWithBullet 
				|| object->GetDestroyReason() == Enemy::DestroyReason::CollisionWithPlayer;
		};

	enemyKillCount_ += std::count_if(enemies_.begin(), enemies_.end(), countKill);

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

void SceneObjectManager::ProcessSpawnRequests()
{
	auto requests = spawnRequestQueue_->Consume();

	for (const auto& request : requests)
	{
		std::unique_ptr<BaseBullet> bullet = request.creator();
		bullet->Initialize(lineModelManager_->FindLineModel(request.modelName), request.position);
		bullet->SetVelocity(request.velocity);
		colliderManager_->AddCollider(bullet->GetCollider());
		bullets_.push_back(std::move(bullet));
	}
}
