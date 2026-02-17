#pragma once

#include "BaseScene.h"

#include "Player.h"
#include "NumUi.h"
#include "ModelEdgeEmitter.h"

#include "title/TitleCamera.h"
#include "PointEmitter.h"

// \brief TitleScene タイトルシーンクラス

class TitleScene : public Kouro::BaseScene
{
public: // メンバ関数

	// \brief 初期化
	void Initialize(Kouro::EngineContext context) override;

	// \brief Finalize 終了
	void Finalize() override;

	// \brief Update 更新
	void Update() override;

	// \brief Draw 描画
	void Draw() override;

private: // メンバ関数

	void CreateEvents();

private:

	std::unique_ptr<Kouro::LineDrawerBase> lineDrawer_ = nullptr;

	std::unique_ptr<BaseCharacter> player_ = nullptr;

	std::unique_ptr<TitleCamera> titleCamera_ = nullptr;

	// グローバルまたはクラスメンバとして開始時刻を記録
	std::chrono::steady_clock::time_point startTime;

	std::vector<Kouro::Vector3> controlPoints_ = {};

	std::unique_ptr<Kouro::Sprite> startBotton_ = nullptr;

	float kFadeStartTime = 4.5f;

	float fadeTimer_ = 0.0f;

	// プレイヤーモデルの移動演出のフラグ
	bool isMoveActive_ = false;

	bool isAlignCamera_ = false;

	std::unique_ptr<Kouro::LineModelManager> lineModelManager_ = nullptr;

	std::unique_ptr<Kouro::NumUi> scoreUi_ = nullptr;

	std::unique_ptr<Kouro::ModelEdgeEmitter> mEmitter = nullptr;

	std::unique_ptr<Kouro::ObjectLine> tonnel_ = nullptr;

	float progressFactor_ = 0.0f; //!< 

	float speedFactor_ = 0.0f;

	static constexpr float kMultiplyFactor_ = 1.01f;

	std::unique_ptr<Kouro::PointEmitter> pointEmitter_ = nullptr;
};