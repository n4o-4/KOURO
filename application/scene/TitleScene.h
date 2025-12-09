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
#include "Audio.h"
#include "LineModelManager.h"
#include "Player.h"
#include "NumUi.h"
#include "ModelEdgeEmitter.h"
// \brief TitleScene タイトルシーンクラス

class TitleScene : public BaseScene
{
private:

private:
public: // メンバ関数

	// \brief Initialzie 初期化
	void Initialize(EngineContext context) override;

	// \brief Finialize 終了
	void Finalize() override;

	// \brief Update 更新
	void Update() override;

	// \brief Draw 描画
	void Draw() override;

private:

	std::unique_ptr<LineDrawerBase> lineDrawer_ = nullptr;

	std::unique_ptr<BaseCharacter> player_ = nullptr;

	std::unique_ptr<TitleCamera> titleCamera_ = nullptr;

	// グローバルまたはクラスメンバとして開始時刻を記録
	std::chrono::steady_clock::time_point startTime;

	std::vector<Vector3> controlPoints_
	{
		{0.0f,0.0f,0.0f},
		{0.0f,10.0f,30.0f},
		{0.0f,25.0f,60.0f},
		{0.0f,50.0f,90.0f},
		{0.0f,90.0f,120.0f},
		{0.0f,150.0f,150.0f},
		{0.0f,200.0f,180.0f}
	}; //!< 

	std::unique_ptr<Sprite> startBotton_ = nullptr;

	float kFadeStartTime = 4.5f;
	float fadeTimer_ = 0.0f;

	// プレイヤーモデルの移動演出のフラグ
	bool isMoveActive_ = false;

	std::unique_ptr<LineModelManager> lineModelManager_ = nullptr;

	std::unique_ptr<NumUi> scoreUi_ = nullptr;

	std::unique_ptr<ModelEdgeEmitter> mEmitter = nullptr;
};