#include "GameOver.h"

void GameOver::Initialize(Kouro::EngineContext context)
{
	// 基底クラスの初期化
	BaseScene::Initialize(context);

	// テクスチャの読み込み
	Kouro::TextureManager::GetInstance()->LoadTexture("Resources/GameOver.png");

	// ゲームオーバー用のスプライトの生成と初期化
	gameOverSprite_ = std::make_unique<Kouro::Sprite>();
	gameOverSprite_->Initialize(Kouro::SpriteCommon::GetInstance(), "Resources/GameOver.png");
	gameOverSprite_->SetSize({ 1280.0f,720.0f });
	gameOverSprite_->SetPosition({ 640.0f,360.0f });
	gameOverSprite_->SetAnchorPoint({ 0.5f,0.5f });
	gameOverSprite_->SetTexSize({ 1536.0f,1024.0f });
	gameOverSprite_->Update();
}

void GameOver::Finalize() {
}

void GameOver::Update()
{

	switch (phase_)
	{
		// フェードイン中の処理
	case Phase::kFadeIn:

		// フェードが終了していたら
		if (fade_->IsFinished())
		{
			// 入力の受付を有効に
			Kouro::Input::GetInstance()->SetIsReception(true);

			// フェーズをメインに変更
			phase_ = Phase::kMain;
		}

		break;

		// メインの処理
	case Phase::kMain:

		// エンターキーかAボタンを押してたら
		if (Kouro::Input::GetInstance()->TriggerKey(DIK_RETURN) || Kouro::Input::GetInstance()->TriggerGamePadButton(Kouro::Input::GamePadButton::A))
		{
			// フェードのステータスを設定して始める
			fade_->Start(Fade::Status::FadeOut, fadeTime_);

			// フェーズをフェードアウトに
			phase_ = Phase::kFadeOut;
		}

		// タイマーに経過時間を加算
		timer_ += kDeltaTime;

		// タイマーの時間が3以上だったら
		if(timer_ > 3.0f)
		{
			// フェードのステータスを設定して始める
			fade_->Start(Fade::Status::FadeOut, fadeTime_);

			// フェーズをフェードアウトに
			phase_ = Phase::kFadeOut;
		}

		break;

		// フェードアウト中の処理
	case Phase::kFadeOut:

		// フェードが終了したら
		if (fade_->IsFinished())
		{
			// シーンをタイトルにする
			Kouro::SceneManager::GetInstance()->ChangeScene("TITLE");

			return;
		}

		break;

		// プレイ中の処理
	case Phase::kPlay:
		break;

		// ポーズ中の処理
	case Phase::kPose:
		break;
	}

	BaseScene::Update();
}

void GameOver::Draw()
{
	DrawBackgroundSprite();

	DrawForegroundSprite();

	gameOverSprite_->Draw();

	fade_->Draw();

}
