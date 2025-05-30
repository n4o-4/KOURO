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

	if (useDebugCamera_ || useFollowCamera_ || useDefaultCamera_)
	{
		ChangeActiveCamera();
	}

	activeCamera_->Update();

	const char* cameraType = "Unknown";

	if (dynamic_cast<DebugCamera*>(activeCamera_)) {
		cameraType = "DebugCamera";
	}
	else if (dynamic_cast<FollowCamera*>(activeCamera_)) {
		cameraType = "FollowCamera";
	}
	else if (activeCamera_ == Camera::GetInstance()) {
		cameraType = "DefaultCamera";
	}

#ifdef _DEBUG

	ImGui::Begin("Camera Info");
	ImGui::Text("Active Camera: %s", cameraType);
	ImGui::End();

#endif
}

void CameraManager::ChangeActiveCamera()
{
	if (useDebugCamera_)
	{
		useDebugCamera_ = false;
		activeCamera_ = debugCamera_.get();
	}
	else if (useFollowCamera_)
	{
		useFollowCamera_ = false;
		activeCamera_ = followCamera_.get();
	}
	else if(useDefaultCamera_)
	{
		useDefaultCamera_ = false;
		activeCamera_ = Camera::GetInstance();
	}
}
