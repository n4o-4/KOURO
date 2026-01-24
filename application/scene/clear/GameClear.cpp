#include "GameClear.h"

void GameClear::Initialize(Kouro::EngineContext context)
{
	// 基底クラスの初期化
	BaseScene::Initialize(context);

	// テクスチャの読み込み
	Kouro::TextureManager::GetInstance()->LoadTexture("Resources/GameClear.png");
	Kouro::TextureManager::GetInstance()->LoadTexture("Resources/score.png");

	// ゲームクリア用のスプライトの生成と初期化
	gameClearSprite_ = std::make_unique<Kouro::Sprite>();
	gameClearSprite_->Initialize(Kouro::SpriteCommon::GetInstance(), "Resources/GameClear.png");
	gameClearSprite_->SetSize({ 1280.0f,720.0f });
	gameClearSprite_->SetPosition({ 640.0f,360.0f });
	gameClearSprite_->SetAnchorPoint({ 0.5f,0.5f });
	gameClearSprite_->SetTexSize({ 1536.0f,1024.0f });
	gameClearSprite_->Update();

	// スコア(数字)UI用のクラスの生成と初期化
	scoreUi_ = std::make_unique<Kouro::NumUi>();
	scoreUi_->Initialize(2);
	eliminatedEnemyCount_ = grobalVariables_.LoadFile("ELIMINATED_ENEMY_COUNT",0);
	scoreUi_->SetDestinationValue(eliminatedEnemyCount_, 1.0f);

	// スコア(文字)用のスプライトクラスの生成と初期化
	scoreLabelSprite_ = std::make_unique<Kouro::Sprite>();
	scoreLabelSprite_->Initialize(Kouro::SpriteCommon::GetInstance(), "Resources/score.png");
	scoreLabelSprite_->SetTexSize({ 640.0f,128.0f });
	scoreLabelSprite_->SetPosition({ 320.0f,550.0f });
	scoreLabelSprite_->SetSize({ 480.0f,128.0f });
	scoreLabelSprite_->Update();
}

void GameClear::Finalize()
{
}

void GameClear::Update()
{
	switch (phase_)
	{
		// フェードイン中の処理
	case Phase::kFadeIn:

		// フェードが終了していた場合
		if (fade_->IsFinished())
		{
			// 入力の受付を有効に
			Kouro::Input::GetInstance()->SetIsReception(true);

			// フェーズを有効に
			phase_ = Phase::kMain;
		}

		break;

		// メインの処理
	case Phase::kMain:

		// スコアUIのカウントが終わっていたら
		if(scoreUi_->GetIsCountFinished())
		{
			// タイマーに経過時間を加算
			timer_ += kDeltaTime;
		}

		// メインタイマーの定数よりタイマーの時間が短かったら
		if(kMainTime >= timer_)
		{
			// フェードの設定をしてスタート
			fade_->Start(Fade::Status::FadeOut, fadeTime_);

			// フェーズをフェードアウトに変更
			phase_ = Phase::kFadeOut;
		}

		break;

		// フェードアウト中の処理
	case Phase::kFadeOut:

		// フェードが終わっていたら
		if (fade_->IsFinished())
		{
			// シーンをタイトルに変更
			Kouro::SceneManager::GetInstance()->ChangeScene("TITLE");

			return;
		}

		break;
	
	case Phase::kPlay:
		break;
	case Phase::kPose:
		break;
	}

	// スコアUIの更新
	scoreUi_->Update();

	// 基底クラスの更新
	BaseScene::Update();
}

void GameClear::Draw()
{
	DrawBackgroundSprite();

	DrawForegroundSprite();	

	gameClearSprite_->Draw();

	scoreLabelSprite_->Draw();

	scoreUi_->Draw();

	fade_->Draw();
}
