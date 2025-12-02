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
#include "GrobalVariables.h"
#include "NumUi.h"

// \brief GameClear ゲームクリアシーンクラス

class GameClear : public BaseScene {

public:

	// \brief Initialzie 初期化
	void Initialize() override;

	// \brief Finialize 終了
	void Finalize() override;

	// \brief Update 更新
	void Update() override;

	// \brief Draw 描画
	void Draw() override;

private:

	std::unique_ptr<Sprite> gameClearSprite_ = nullptr;

	const float kMainTime = 3.0f;

	float timer_ = 0.0f;
	
	std::unique_ptr<NumUi> scoreUi_ = nullptr;

	GrobalVariables grobalVariables_;

	uint32_t eliminatedEnemyCount_ = 0;

	std::unique_ptr<Sprite> scoreLabelSprite_ = nullptr;
};

