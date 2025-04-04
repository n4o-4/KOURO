#pragma once

#include "BaseScene.h"
#include "Kouro.h"
//========================================
// アプリケーション
#include "Player.h"
#include "FollowCamera.h"
#include "SkyDome.h"  
#include "Ground.h"  +
#include "Enemy.h"
#include "CollisionManager.h"
#include "LockOn.h"
#include "BaseEnemy.h"
#include "GroundTypeEnemy.h"
#include "SkyTypeEnemy.h"
#include "Spawn.h"
#include <random>
#include "LineDrawerBase.h"
#include "GroundTypeEnemy2.h"
#include "Hud.h"


class GameScene : public BaseScene
{
private:
	
	//========================================
	// ライトクラス
	std::unique_ptr<DirectionalLight> directionalLight = nullptr;
	std::unique_ptr<PointLight> pointLight = nullptr;
	std::unique_ptr<SpotLight> spotLight = nullptr;
	//========================================
	// アニメーションマネージャ
	std::unique_ptr<AnimationManager> animationManager = nullptr;
	//========================================
	// ゲームの状態
	// ゲームクリア
	bool isGameClear_ = false;
	// ゲームオーバー
	bool isGameOver_ = false;
	// コンティニュー
	bool isContinue_ = true;
	//========================================
	// 天球
	std::unique_ptr<SkyDome> skyDome_ = nullptr;
	//========================================
	// 地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//========================================
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// ロックオン
	std::unique_ptr<LockOn> lockOnSystem_ = nullptr;
	// HUD
	std::unique_ptr<Hud> hud_ = nullptr;
	//========================================
	// 敵出現
    std::stringstream enemyPopCommands;
    // 敵のリスト
    std::vector<std::unique_ptr<BaseEnemy>> enemies_;
	// 待機フラグ
    bool isWaiting_ = false;
	// 待機時間
    int32_t waitTimer_ = 0;
	//========================================
	// 当たり判定マネージャ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	//
	std::vector<std::unique_ptr<BaseEnemy>> spawns_;

	std::unique_ptr<LineDrawerBase> lineDrawer_ = nullptr;	

	std::unique_ptr<Object3d> human_ = nullptr;	

	Dissolve* dissolve_ = nullptr;

private:

	//std::unique_ptr<SkyDome> skyDome_ = nullptr;
	//std::unique_ptr<Object3d> skyDomeObj_ = nullptr;


public: // メンバ関数

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

private: //静的メンバ関数

	// 敵の出現データの読み込み
	void LoadEnemyPopData(int index);

	// 敵の出現データの更新
	void UpdateEnemyPopCommands();

	// 敵の出現  
	void SpawnEnemy(const Vector3& position);

	void SpawnEnemyKumo(const Vector3& position);

	void SpawnEnemyBat(const Vector3& position);

	void SpawnEnemyBomb(const Vector3& position);

	void SpawnSet(const Vector3& position);

	std::string currentSpawnType_;

	void AvoidOverlap(std::vector<BaseEnemy*>& allEnemies, float avoidRadius);

//private: //csv
//	std::vector<std::string> enemyPopData_; // CSVデータの保存
//	size_t enemyPopIndex_ = 0; // 現在読んでいる行のインデックス
//	int numEnemiesToSpawn_ = 0; // 残った敵数
//	std::string currentSpawnType_; // 現在スポーンする敵タイプ
//	Vector3 spawnPosition_; // 敵のスポーン位置
private:
	
	// 敵のスポーン範囲の設定
	const float minX = -50.0f, maxX = 50.0f;
	const float minY = 10.0f, maxY = 30.0f;  
	const float minZ = -50.0f, maxZ = 50.0f;

	// 乱数ジェネレータ
	std::random_device rd;
	std::mt19937 gen;


	// wave
	int waveIndex_ = 0;
	bool waveReady_ = true;
	std::vector<std::string> waveCsvPaths_;
	int waveDisplayTimer_ = 120;
	const int waveDisplayDuration_ = 120;
	int currentWaveImageIndex_ = 1;
	std::unique_ptr<Sprite> wave1_ = nullptr;
	std::unique_ptr<Sprite> wave2_ = nullptr;
	std::unique_ptr<Sprite> wave3_ = nullptr;
};