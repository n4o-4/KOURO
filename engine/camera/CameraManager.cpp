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

	DrawDebugUI();
}

void CameraManager::CamerasClear()
{
	cameras_.clear();
}

void CameraManager::DrawDebugUI()
{
#ifdef _DEBUG

	ImGui::Begin("CameraManager");

	if(ImGui::Button("Use Debug Camera"))
	{
		activeCamera_ = debugCamera_.get();
	}

	if (ImGui::Button("Use FollowCamera"))
	{
		activeCamera_ = followCamera_.get();
	}

	ImGui::End();

#endif
}
