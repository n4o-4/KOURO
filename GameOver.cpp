#include "GameOver.h"

void GameOver::Initialize() {
	BaseScene::Initialize();
	//over
	TextureManager::GetInstance()->LoadTexture("Resources/scene/game_over.png");
	over_ = std::make_unique<Sprite>();
	over_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/game_over.png");
	over_->SetTexSize({ 1280.0f,720.0f });
	over_->SetSize({ 1280.0f,720.0f });
	over_->SetPosition({ 0.0f,0.0f });

}

void GameOver::Finalize() {
}

void GameOver::Update() {


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
	
	//title
	over_->Update();

	BaseScene::Update();
}

void GameOver::Draw() {


	DrawBackgroundSprite();

	over_->Draw();

	DrawForegroundSprite();

	fade_->Draw();

}
