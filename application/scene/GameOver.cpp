#include "GameOver.h"

void GameOver::Initialize(EngineContext context)
{
	BaseScene::Initialize(context);

	TextureManager::GetInstance()->LoadTexture("Resources/GameOver.png");

	gameOverSprite_ = std::make_unique<Sprite>();
	gameOverSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/GameOver.png");
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
	case Phase::kFadeIn:

		if (fade_->IsFinished())
		{
			Input::GetInstance()->SetIsReception(true);
			phase_ = Phase::kMain;
		}

		break;
	case Phase::kMain:

		if (Input::GetInstance()->Triggerkey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A))
		{
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
			phase_ = Phase::kFadeOut;
		}

		timer_ += kDeltaTime;

		if(timer_ > 3.0f)
		{
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
			phase_ = Phase::kFadeOut;
		}

		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished())
		{
			SceneManager::GetInstance()->ChangeScene("GAME");

			return;
		}

		break;

	case Phase::kPlay:
		break;
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
