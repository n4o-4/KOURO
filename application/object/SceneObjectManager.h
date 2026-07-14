#pragma once

// application
class Player;
class Enemy;
class BaseBullet;
class SpawnRequestQueue;

#include "ColliderManager.h"	
#include "LineModelManager.h"

#include <memory>
#include <vector>


class SceneObjectManager
{
public:
	void Initialize();

	/// \brief  更新
	void Update();
	
	/// \brief  オブジェクトのクリーンアップ
	void CleanupObjects();

	/// \brief  描画
	void Draw();

	void SetColliderManager(ColliderManager* colliderManager) { colliderManager_ = colliderManager; }
	void SetLineModelManager(Kouro::LineModelManager* lineModelManager) { lineModelManager_ = lineModelManager; }
	/**
     * \brief プレイヤーを登録する
     * \param player プレイヤーの所有権を持つスマートポインタ
     */
	void RegisterPlayer(std::unique_ptr<Player> player);

	/**
	 * \brief 敵を登録する
	 * \param enemy 敵の所有権を持つスマートポインタ
	 */
	void RegisterEnemy(std::unique_ptr<Enemy> enemy);

	/**
	 * \brief 弾を登録する
	 * \param bullet 弾の所有権を持つスマートポインタ
	 */
	void RegisterBullet(std::unique_ptr<BaseBullet> bullet);

	/**
	 * \brief プレイヤーを取得する
	 * \return プレイヤーへの生ポインタ
	 */
	Player* GetPlayer() { return player_.get(); }

	uint32_t GetEnemyCount() const { return enemyCount_; }

	uint32_t GetEnemyKillCount() const { return enemyKillCount_; }

	SpawnRequestQueue* GetSpawnRequestQueue() { return spawnRequestQueue_.get(); }
private:

	void ProcessSpawnRequests();

private:
	std::unique_ptr<Player> player_; // プレイヤー
	std::vector<std::unique_ptr<Enemy>> enemies_; // キャラクターのリスト
	std::vector<std::unique_ptr<BaseBullet>> bullets_; // 弾のリスト

	std::unique_ptr<SpawnRequestQueue> spawnRequestQueue_; // 生成要求キュー

	ColliderManager* colliderManager_ = nullptr; // コライダーマネージャー
	Kouro::LineModelManager* lineModelManager_ = nullptr; // ラインモデルマネージャー

	uint32_t enemyCount_ = 0; // 敵の数
	uint32_t enemyKillCount_ = 0; // 敵の撃破数

	
};