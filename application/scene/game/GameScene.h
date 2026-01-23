#pragma once

#include "BaseScene.h"
#include "Kouro.h"
//========================================
// アプリケーション

#include <random>
#include "LineDrawerBase.h"
#include "LineModelManager.h"
#include "Player.h"
#include "Enemy.h"

#include "LevelLoader.h"
#include "GlobalVariables.h"

#include "RailAnimation.h"

// \brief GameScene ゲームシーンクラス

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

	std::unique_ptr<LineModelManager> lineModelManager_ = nullptr;

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

	std::unique_ptr<ModelEdgeEmitter> mEmitter = nullptr;

	std::unique_ptr<ParticleEmitter> hitEffect2_Circle1_ = nullptr;
	std::unique_ptr<ParticleEmitter> hitEffect2_Circle2_ = nullptr;

	std::shared_ptr<Player> player_ = nullptr;

	std::unique_ptr<RailCamera> railCamera_ = nullptr;

	std::unique_ptr<LevelLoader::LevelData> levelData_ = nullptr;

	std::vector<std::shared_ptr<Enemy>> enemies_;

	std::unique_ptr<ColliderManager> colliderManager_ = nullptr;

	std::unique_ptr<Sprite> countSprite_[3];

	Vector4 kDefaultUIColor_ = { 0.612f, 0.000f, 0.357f, 1.0f };

	std::unique_ptr<Sprite> WASD_ = nullptr;

	std::unique_ptr<Sprite> fireUI_ = nullptr;

	float countScale_[3] = { 0.0f, 0.0f, 0.0f };
	float countRotation_[3] = { 0.0f, 0.0f, 0.0f };
	float countAlpha_[3] = { 0.0f, 0.0f, 0.0f }; // フェードイン用


	float deltaTime = 1.0f / 60.0f;

	float deltaRotation = 6.28f / 60.0f;

	float countTimer_ = 0.0f;


	float randomSpeed = 2.0f;
	float elapsedTime_ = 0.0f;

	float k = 0.0f;

	std::unique_ptr<ObjectLine> stage_ = nullptr;

	GrobalVariables grobalVariables_;

	uint32_t eliminatedEnemyCount_ = 0;

	/*std::unique_ptr<RailAnimation> enemyRailAnimation_ = nullptr;
	std::unique_ptr<RailAnimation> playerRailAnimation_ = nullptr;
	std::unique_ptr<RailAnimation> cameraRailAnimation_ = nullptr;*/

	float playerProgress_ = 0.05f;
	float enemyProgressOffset_ = 0.01f;
	float cameraProgressOffset_ = -0.01f;

	RailFollower enemyRail_;
	RailFollower playerRail_;
	RailFollower cameraRail_;

private:

public: // メンバ関数

	// \brief 初期化
	void Initialize(EngineContext context) override;

	// \brief Finalize 終了
	void Finalize() override;

	// \brief Update 更新
	void Update() override;

	// \brief Draw 描画
	void Draw() override;

private: //静的メンバ関数

private:

};