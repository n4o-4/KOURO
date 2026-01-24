#pragma once

#include "BaseScene.h"

#include "audio.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"
#include "GlobalVariables.h"
#include "NumUi.h"

// \brief GameClear ゲームクリアシーンクラス

class GameClear : public Kouro::BaseScene {

public:

	// \brief 初期化
	void Initialize(Kouro::EngineContext context) override;

	// \brief Finalize 終了
	void Finalize() override;

	// \brief Update 更新
	void Update() override;

	// \brief Draw 描画
	void Draw() override;

private:

	std::unique_ptr<Kouro::Sprite> gameClearSprite_ = nullptr;

	const float kMainTime = 3.0f;

	float timer_ = 0.0f;

	std::unique_ptr<Kouro::NumUi> scoreUi_ = nullptr;

	Kouro::GlobalVariables grobalVariables_;

	uint32_t eliminatedEnemyCount_ = 0;

	std::unique_ptr<Kouro::Sprite> scoreLabelSprite_ = nullptr;
};

