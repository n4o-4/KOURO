#pragma once
#include <type_traits>
#include <vector>
#include <memory>
#include <chrono>
#include "Sprite.h"
#include "SpriteCommon.h"

// \brief NumUi
// 数字をUI上に表示し、カウントアップアニメーションを行うクラス。
// 最大桁数を指定して初期化し、SetDestinationValueで目標値を設定。
// Updateでカウント処理を進め、Drawで画面に描画。
// 内部で各桁をSpriteとして保持し、時間経過に応じて数値を滑らかに変化させる。

class NumUi
{
public:

	void Initialize(size_t maxDigits);

	void Update();

	void Draw();

	void SetTexSize(float width, float height) {};

	void SetDestinationValue(uint32_t value,float countTime);

	bool GetIsCountFinished() const { return isCountFinished_; }

private:

	Vector2 position_ = { 0.0f, 0.0f };

	uint32_t destinationValue_ = 0;
	uint32_t startValue_ = 0;
	uint32_t value_ = 0;

	std::vector<std::unique_ptr<Sprite>> numSprites_;

    float digitSpacing_ = 0.0f;

	float texWidth_ = 64.0f;

	float texHeight_ = 128.0f;

	std::vector<int> digits_;

	bool isCount_ = false;

	std::chrono::steady_clock::time_point lastUpdateTime_;

	float countDuration_ = 0.1f; // カウントアップにかける時間（秒）

	float countTime_ = 0.0f;

	bool isCountFinished_ = false;
};