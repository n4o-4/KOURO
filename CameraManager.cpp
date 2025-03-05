#include "CameraManager.h"

void CameraManager::Initialize()
{
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	activeCamera_ = Camera::GetInstance();

	cameras_.push_back(Camera::GetInstance());
	cameras_.push_back(debugCamera_.get());
}

void CameraManager::Update()
{
	if (useDebugCamera_)
	{
		activeCamera_ = debugCamera_.get();
	}
	else
	{
		activeCamera_ = Camera::GetInstance();
	}

	activeCamera_->Update();
}
