#include "Fade.h"

void Fade::Initialize()
{
	Kouro::TextureManager::GetInstance()->LoadTexture("Resources/white.png");

	// スプライトの生成
	curtain_ = std::make_unique<Kouro::Sprite>();

	// スプライトの初期化
	curtain_->Initialize(Kouro::SpriteCommon::GetInstance(), "Resources/white.png");

	// スプライトのアンカーポイントを真ん中に設定
	curtain_->SetAnchorPoint(Kouro::Vector2(0.5f, 0.5f));

	// スプライトのサイズ、位置、色を設定
	curtain_->SetSize(Kouro::Vector2(1280.0f, 720.0f));

	// TODO: WindowWidth, WindowHeightを使うように変更する
	curtain_->SetTexSize(Kouro::Vector2(1280.0f, 720.0f));
	curtain_->SetPosition(Kouro::Vector2(640.0f, 360.0f));
	curtain_->SetColor(Kouro::Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// スプライトの更新
	curtain_->Update();

	// フェードの初期ステータスをNoneに設定
	Status status_ = Status::None;
}

void Fade::Update()
{
	// Fade処理の更新
	UpdateFade();
	
	// スプライトの更新
	curtain_->Update();
}

void Fade::Draw()
{
	// Noneの場合描画しない
	if (status_ == Status::None)
	{
		return;
	}

	curtain_->Draw();
}

void Fade::Start(Status status, float duration)
{
	// フェードのステータスと持続時間を設定
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

bool Fade::IsFinished() const
{
	// ステータスでの個別の終了判定
	switch (status_)
	{
	case Status::FadeIn:

		if (counter_ >= duration_)
		{
			return true;
		}
		else
		{
			return false;
		}

	case Status::FadeOut:

		if (counter_ >= duration_)
		{
			return true;
		}
		else
		{
			return false;
		}

	case Status::WhiteFadeIn:

		if (counter_ >= duration_)
		{
			return true;
		}
		else
		{
			return false;
		}

	case Status::WhiteFadeOut:	

		if (counter_ >= duration_)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Fade::UpdateFade()
{
	// ステータスでの個別の処理
	switch (status_)
	{
	case Status::None:
		// 何もしない
		break;

	case Status::FadeIn:
		// フェードインの更新処理
		UpdateFadeIn();
		break;

	case Status::FadeOut:
		// フェードアウトの更新処理
		UpdateFadeOut();
		break;

	case Status::WhiteFadeIn:
		// フェードインの更新処理
		UpdateWhiteFadeIn();
		break;

	case Status::WhiteFadeOut:
		// フェードアウトの更新処理
		UpdateWhiteFadeOut();
		break;
	}
}

void Fade::UpdateFadeIn()
{
	// 1フレーム分の秒数をカウント
	counter_ += 1.0f / 60.0f; // 可変フレームレートに変更予定

	// フェード継続時間に達したら終了
	if (counter_ >= duration_)
	{
		counter_ = duration_;
	}

	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
	curtain_->SetColor(Kouro::Vector4(0.0f, 0.0f, 0.0f, 1.0f - std::clamp(counter_ / duration_, 0.0f, 1.0f)));

	curtain_->Update();
}

void Fade::UpdateFadeOut()
{
	// 1フレーム分の秒数をカウント
	counter_ += 1.0f / 60.0f; // 可変フレームレートに変更予定

	// フェード継続時間に達したら終了
	if (counter_ >= duration_)
	{
		counter_ = duration_;
	}

	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
	curtain_->SetColor(Kouro::Vector4(0.0f, 0.0f, 0.0f, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

	curtain_->Update();
}

void Fade::UpdateWhiteFadeIn()
{
	// 1フレーム分の秒数をカウント
	counter_ += 1.0f / 60.0f; // 可変フレームレートに変更予定

	// フェード継続時間に達したら終了
	if (counter_ >= duration_)
	{
		counter_ = duration_;
	}

	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
	curtain_->SetColor(Kouro::Vector4(1.0f, 1.0f, 1.0f, 1.0f - std::clamp(counter_ / duration_, 0.0f, 1.0f)));

	curtain_->Update();
}

void Fade::UpdateWhiteFadeOut()
{
	// 1フレーム分の秒数をカウント
	counter_ += 1.0f / 60.0f; // 可変フレームレートに変更予定

	// フェード継続時間に達したら終了
	if (counter_ >= duration_)
	{
		counter_ = duration_;
	}

	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
	curtain_->SetColor(Kouro::Vector4(1.0f, 1.0f, 1.0f, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

	curtain_->Update();
}
