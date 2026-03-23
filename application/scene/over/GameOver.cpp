#include "GameOver.h"
#include "MyMath.h"

void GameOver::Initialize(Kouro::EngineContext context)
{
	// 基底クラスの初期化
	BaseScene::Initialize(context);

	// テクスチャの読み込み
	Kouro::TextureManager::GetInstance()->LoadTexture("texture/GameOver.png");

	// ゲームオーバー用のスプライトの生成と初期化
	gameOverSprite_ = std::make_unique<Kouro::Sprite>();
	gameOverSprite_->Initialize(Kouro::SpriteCommon::GetInstance(), "texture/GameOver.png");
	gameOverSprite_->SetSize({ 1280.0f,720.0f });
	gameOverSprite_->SetPosition({ 640.0f,360.0f });
	gameOverSprite_->SetAnchorPoint({ 0.5f,0.5f });
	gameOverSprite_->SetTexSize({ 1536.0f,1024.0f });
	gameOverSprite_->Update();

	emitter_ = std::make_unique<Kouro::ModelEdgeEmitter>();
	emitter_->Initialize("normal",context);
	emitter_->CreateLineSegment("player/player.obj");
	emitter_->SetEmitCount(10);

	std::unique_ptr<Kouro::Camera> camera = std::make_unique<Kouro::Camera>();
	camera->Initialize();
	camera->SetTranslate(Kouro::Vector3(0.0f, 5.0f, -30.0f));

	cameraManager_->AddCamera("camera", std::move(camera));
	cameraManager_->SetActiveCamera("camera");
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

			emitter_->Emit(Kouro::MakeIdentity4x4());
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
