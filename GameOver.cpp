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

	if (Input::GetInstance()->Triggerkey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");

		return;
	}

	//title
	over_->Update();


}

void GameOver::Draw() {


	DrawBackgroundSprite();

	over_->Draw();

	DrawForegroundSprite();

}
