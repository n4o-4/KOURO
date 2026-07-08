#pragma once
#include <memory>
#include <vector>

// application
#include "Player.h"
#include "Enemy.h"
#include "BaseBullet.h"


class SceneObjectManager
{
public:
	void Initialize();

	void Update();

	void CleanupObjects();

	void Draw();

	void RegisterPlayer(std::unique_ptr<Player> player);

	void RegisterEnemy(std::unique_ptr<Enemy> enemy);

	void RegisterBullet(std::unique_ptr<BaseBullet> bullet);

	Player* GetPlayer() { return player_.get(); }

private:
	std::unique_ptr<Player> player_; // プレイヤー
	std::vector<std::unique_ptr<Enemy>> enemies_; // キャラクターのリスト
	std::vector<std::unique_ptr<BaseBullet>> bullets_; // 弾のリスト
};

