#include "CameraManager.h"

void CameraManager::Initialize()
{
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	activeCamera_ = Camera::GetInstance();

	cameras_.push_back(Camera::GetInstance());
	cameras_.push_back(debugCamera_.get());
}

void CameraManager::Update()
{
	activeCamera_->Update();
}
