#include "GameClear.h"

void GameClear::Initialize() {
	BaseScene::Initialize();
	//clear
	TextureManager::GetInstance()->LoadTexture("Resources/scene/game_clear.png");
	clear_ = std::make_unique<Sprite>();
	clear_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/game_clear.png");
	clear_->SetTexSize({ 1280.0f,720.0f });
	clear_->SetSize({ 1280.0f,720.0f });
	clear_->SetPosition({ 0.0f,0.0f });

	AudioManager::GetInstance()->Initialize();
	AudioManager::GetInstance()->SoundLoadFile("Resources/bgm/gameCLEAR.mp3");
	clearBGM_ = std::make_unique<Audio>();
	clearBGM_->Initialize();
	clearBGM_->SoundPlay("Resources/bgm/gameCLEAR.mp3", 9999);

}

void GameClear::Finalize() {
}

void GameClear::Update() {
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
	clear_->Update();

	BaseScene::Update();
}

void GameClear::Draw() {
	DrawBackgroundSprite();

	clear_->Draw();

	DrawForegroundSprite();

	fade_->Draw();
}
