#include "BaseScene.h"
#include "AudioManager.h"

void BaseScene::Initialize()
{
	AudioManager::GetInstance()->Initialize();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	AudioManager::GetInstance()->SoundLoadFile("Resources/Alarm01.wav");

	AudioManager::GetInstance()->SoundLoadFile("Resources/Spinning_World.mp3");
}

void BaseScene::Finalize()
{

	AudioManager::GetInstance()->Finalize();

}

void BaseScene::Update()
{
	Camera::GetInstance()->Update();
	if (isDebugCamera_)
	{
		debugCamera_->Update();
	}

	
}

void BaseScene::Draw()
{
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
