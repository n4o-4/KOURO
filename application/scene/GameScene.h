#pragma once

#include "BaseScene.h"
#include "Kouro.h"
//========================================
// アプリケーション
#include "Player.h"
#include "FollowCamera.h"
#include "SkyDome.h"  
#include "Ground.h"  
#include "Enemy.h"
#include "CollisionManager.h"
#include "LockOn.h"
#include "BaseEnemy.h"

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
	//========================================
	// 敵出現
    std::stringstream enemyPopCommands;
    // 敵のリスト
    std::vector<std::unique_ptr<Enemy>> enemies_;
	// 待機フラグ
    bool isWaiting_ = false;
	// 待機時間
    int32_t waitTimer_ = 0;
	//========================================
	// 当たり判定マネージャ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

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
	void LoadEnemyPopData();

	// 敵の出現データの更新
	void UpdateEnemyPopCommands();

	// 敵の出現  
	void SpawnEnemy(const Vector3& position);

};