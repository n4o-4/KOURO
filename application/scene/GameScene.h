#pragma once

#include "BaseScene.h"
#include "Kouro.h"
//========================================
// アプリケーション

#include <random>
#include "LineDrawerBase.h"
#include "Player.h"
#include "Enemy.h"

#include "LevelLoader.h"

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
	
	// 待機フラグ
    bool isWaiting_ = false;
	// 待機時間
    int32_t waitTimer_ = 0;

	std::unique_ptr<LineDrawerBase> lineDrawer_ = nullptr;	

	Dissolve* dissolve_ = nullptr;

	std::unique_ptr<Object3d> ground_ = nullptr;

	std::unique_ptr<WorldTransform> transform_ = nullptr;

	std::unique_ptr<ParticleEmitter> emitter1_ = nullptr;

	std::unique_ptr<ParticleEmitter> emitter2_ = nullptr;

	std::unique_ptr<ParticleEmitter> emitter3_ = nullptr;

	std::unique_ptr<ParticleEmitter> emitter4_ = nullptr;

	std::unique_ptr<ParticleEmitter> hitEffect1_Ring1_ = nullptr;
	std::unique_ptr<ParticleEmitter> hitEffect1_Ring2_ = nullptr;
	std::unique_ptr<ParticleEmitter> hitEffect1_Circle_ = nullptr;
	std::unique_ptr<ParticleEmitter> hitEffect1_CenterCircle_ = nullptr;


	std::unique_ptr<ParticleEmitter> hitEffect2_Circle1_ = nullptr;
	std::unique_ptr<ParticleEmitter> hitEffect2_Circle2_ = nullptr;

	std::unique_ptr<Player> player_ = nullptr;

	std::unique_ptr<RailCamera> railCamera_ = nullptr;

	std::unique_ptr<LevelLoader::LevelData> levelData_ = nullptr;

	std::vector<std::unique_ptr<Enemy>> enemies_;

	std::unique_ptr<ColliderManager> colliderManager_ = nullptr;

private:

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

private:

};