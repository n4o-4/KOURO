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

}

void GameClear::Finalize() {
}

void GameClear::Update() {

	if (Input::GetInstance()->Triggerkey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");

		return;
	}

	//title
	clear_->Update();
}

void GameClear::Draw() {
	DrawBackgroundSprite();

	clear_->Draw();

	DrawForegroundSprite();
}
