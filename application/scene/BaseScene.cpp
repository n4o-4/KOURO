#include "BaseScene.h"
#include "AudioManager.h"

#include "SceneManager.h"

void BaseScene::Initialize()
{
	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(),srvManager_);

	cameraManager_ = std::make_unique<CameraManager>();	
	cameraManager_->Initialize();

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	phase_ = Phase::kFadeIn;

	fade_->Start(Fade::Status::FadeIn, fadeTime_);

	Input::GetInstance()->SetIsReception(false);
}

void BaseScene::Finalize()
{

	AudioManager::GetInstance()->Finalize();

}

void BaseScene::Update()
{

	lineDrawer_->Update();

	cameraManager_->Update();

	fade_->Update();
}

void BaseScene::Draw()
{
}

void BaseScene::LineDraw()
{
	lineDrawer_->Draw(Camera::GetInstance()->GetViewProjection());
}

void BaseScene::DrawObject()
{

	Object3dCommon::GetInstance()->SetView();
	
}

void BaseScene::DrawBackgroundSprite()
{
	SpriteCommon::GetInstance()->SetView();

	SpriteCommon::GetInstance()->DrawBackground();
}

void BaseScene::DrawForegroundSprite()
{
	SpriteCommon::GetInstance()->SetView();

	SpriteCommon::GetInstance()->DrawForeground();
}

void BaseScene::DrawFade()
{
	fade_->Draw();
}
