#include "BaseScene.h"
#include "AudioManager.h"

void BaseScene::Initialize()
{
	AudioManager::GetInstance()->Initialize();

	AudioManager::GetInstance()->SoundLoadFile("Resources/Alarm01.wav");

	AudioManager::GetInstance()->SoundLoadFile("Resources/Spinning_World.mp3");
}

void BaseScene::Finalize()
{

	AudioManager::GetInstance()->Finalize();

}

void BaseScene::Update()
{
}

void BaseScene::Draw()
{
}
