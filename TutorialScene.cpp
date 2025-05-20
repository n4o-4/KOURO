#include "TutorialScene.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"

void TutorialScene::Initialize() {
	BaseScene::Initialize();

	//// テクスチャ読み込みと初期化
	//TextureManager::GetInstance()->LoadTexture("Resources/scene/tutorial.png");
	//tutorialImage_ = std::make_unique<Sprite>();
	//tutorialImage_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/tutorial.png");
	//tutorialImage_->SetTexSize({ 1280.0f, 720.0f });
	//tutorialImage_->SetSize({ 1280.0f, 720.0f });
	//tutorialImage_->SetPosition({ 0.0f, 0.0f });

	fade_->Start(Fade::Status::FadeIn, fadeTime_);
	phase_ = Phase::kFadeIn;
	Input::GetInstance()->SetIsReception(false);

	// BGMを追加するならここ（任意）
	// AudioManager::GetInstance()->SoundLoadFile("Resources/tutorial_bgm.mp3");
	// audio_ = std::make_unique<Audio>();
	// audio_->Initialize();
	// audio_->SoundPlay("Resources/tutorial_bgm.mp3", 1);
}

void TutorialScene::Finalize() {
	BaseScene::Finalize();
}

void TutorialScene::Update() {
	BaseScene::Update();

	// STARTボタンでタイトルに戻る
	if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::START)) {
		fade_->Start(Fade::Status::FadeOut, fadeTime_);
		phase_ = Phase::kPose; // 一時的な遷移フェーズ
		return;
	}

	// フェードアウト完了後に TITLE に戻る
	if (phase_ == Phase::kPose && fade_->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
		return;
	}

	// 既存のフェーズ遷移（コメントアウトされてたら戻してもOK）
	if (phase_ == Phase::kFadeIn && fade_->IsFinished()) {
		Input::GetInstance()->SetIsReception(true);
		phase_ = Phase::kMain;
	} //else if (phase_ == Phase::kMain &&
	//	Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
	//	fade_->Start(Fade::Status::FadeOut, fadeTime_);
	//	phase_ = Phase::kFadeOut;
	//} else if (phase_ == Phase::kFadeOut && fade_->IsFinished()) {
	//	SceneManager::GetInstance()->ChangeScene("GAME");
	//}

	if (tutorialImage_) {
		tutorialImage_->Update();
	}
}


void TutorialScene::Draw() {
	BaseScene::Draw();

	DrawBackgroundSprite();
	DrawObject();

	if (tutorialImage_) {
		tutorialImage_->Draw();
	}

	DrawForegroundSprite();
	DrawFade();
}
