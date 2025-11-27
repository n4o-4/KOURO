#pragma once
#include <memory>
#include <algorithm>
//==============================
// engine
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Sprite.h"

// \brief Fade
// 画面フェード処理を管理するクラス。
// 黒または白によるフェードイン・フェードアウトをサポート。
// スプライトを用いて画面全体を覆い、フェードの状態・時間を管理。
// Updateで状態を進行させ、Drawで描画。Startでフェード開始、IsFinishedで終了判定。

class Fade
{
public:

	enum class Status
	{
		None,
		FadeIn,
		FadeOut,
		WhiteFadeIn,
		WhiteFadeOut,
	};

public:
	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画
	void Draw();

	// フェード開始
	void Start(Status status, float duration);

	// フェード終了判定
	bool IsFinished() const;

private:

	void UpdateFade();

	void UpdateFadeIn();

	void UpdateFadeOut();

	void UpdateWhiteFadeIn();

	void UpdateWhiteFadeOut();

private:

	// フェード用のスプライト
	std::unique_ptr<Sprite> curtain_ = nullptr;

	// フェードのステータス
	Status status_ = Status::None;

	// フェードの持続時間
	float duration_ = 0.0f;

	// 経過時間のカウンター		
	float counter_ = 0.0f;
};

