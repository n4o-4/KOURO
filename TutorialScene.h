#pragma once
#include "BaseScene.h"
#include "Kouro.h"
//========================================
// アプリケーション
#include "BaseEnemy.h"
#include "CollisionManager.h"
#include "Enemy.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "GroundTypeEnemy.h"
#include "GroundTypeEnemy2.h"
#include "GroundTypeEnemy3.h"
#include "GroundTypeEnemy4.h"
#include "Hud.h"
#include "LineDrawerBase.h"
#include "LockOn.h"
#include "Player.h"
#include "SkyDome.h"
#include "SkyTypeEnemy.h"
#include "Spawn.h"
#include <random>

#include <algorithm>

class TutorialScene : public BaseScene
{
private:
	//========================================
	// ライトクラス
	std::unique_ptr<DirectionalLight> directionalLight = nullptr;
	std::unique_ptr<PointLight> pointLight = nullptr;
	std::unique_ptr<SpotLight> spotLight = nullptr;

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

public: // メンバ関数
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;


};

