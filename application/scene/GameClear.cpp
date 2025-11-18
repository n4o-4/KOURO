#include "GameClear.h"

void GameClear::Initialize()
{
	BaseScene::Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/GameClear.png");
	TextureManager::GetInstance()->LoadTexture("Resources/score.png");

	gameClearSprite_ = std::make_unique<Sprite>();
	gameClearSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/GameClear.png");
	gameClearSprite_->SetSize({ 1280.0f,720.0f });
	gameClearSprite_->SetPosition({ 640.0f,360.0f });
	gameClearSprite_->SetAnchorPoint({ 0.5f,0.5f });
	gameClearSprite_->SetTexSize({ 1536.0f,1024.0f });
	gameClearSprite_->Update();

	scoreUi_ = std::make_unique<NumUi>();
	scoreUi_->Initialize(2);

	eliminatedEnemyCount_ = grobalVariables_.LoadFile("ELIMINATED_ENEMY_COUNT",0);

	scoreUi_->SetDestinationValue(eliminatedEnemyCount_, 1.0f);

	scoreLabelSprite_ = std::make_unique<Sprite>();
	scoreLabelSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/score.png");
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
	case Phase::kFadeIn:

		if (fade_->IsFinished())
		{
			Input::GetInstance()->SetIsReception(true);
			phase_ = Phase::kMain;
		}

		break;
	case Phase::kMain:

		if(scoreUi_->GetIsCountFinished())
		{
			timer_ += kDeltaTime;
		}

		if(kMainTime >= timer_)
		{
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
			phase_ = Phase::kFadeOut;
		}

		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished())
		{
			SceneManager::GetInstance()->ChangeScene("TITLE");

			return;
		}

		break;
	
	case Phase::kPlay:
		break;
	case Phase::kPose:
		break;
	}

	scoreUi_->Update();

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
