#pragma once
#include <memory>
#include <algorithm>
//==============================
// engine
#include "GpuResourceUtils.h"
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

	// TODO : Spriteを引数として渡すように変更
	// \brief 初期化
	void Initialize(ID3D12GraphicsCommandList* commandList, const Kouro::GpuResourceUtils* gpuResourceUtils);

	// \brief Update 更新
	void Update();

	// \brief Draw 描画
	void Draw();

	/**
	* \brief  Start   フェードスタート
	* \param status   フェードの種類
	* \param duration 時間
	*/
	void Start(Status status, float duration);

	/**
	* \brief  IsFinished フェードの終了フラグを取得する
	* \return bool       終了している場合true、していない場合false
	*/
	bool IsFinished() const;

private:

	// \brief UpdateFade 各種フェードの更新
	void UpdateFade();

	// \brief UpdateFadeIn フェードインの更新
	void UpdateFadeIn();

	// \brief UpdateFadeOut フェードアウトの更新
	void UpdateFadeOut();

	// \brief UpdateWhiteFadeIn 白フェードインの更新
	void UpdateWhiteFadeIn();

	// \brief UpdateWhiteFadeOut 白フェードアウトの更新
	void UpdateWhiteFadeOut();

private:

	std::unique_ptr<Kouro::Sprite> curtain_ = nullptr;

	Status status_ = Status::None;

	float duration_ = 0.0f;

	float counter_ = 0.0f;
};

