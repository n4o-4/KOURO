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

	std::random_device seedGenerator;
	randomEngine.seed(seedGenerator());
}

void CameraManager::Update()
{
	Shake();

	activeCamera_->SetOffset(shakeOffSet);

	activeCamera_->Update();

	DrawDebugUI();
}

void CameraManager::CamerasClear()
{
	cameras_.clear();
}

void CameraManager::CameraShake(float time)
{
	shakeTime = time;

	isShaking = true;
}

void CameraManager::Shake()
{
	timer += 1.0f / 60.0f;

	if (shakeTime < timer)
	{
		isShaking = false;
		timer = 0.0f;
		shakeTime = 0.0f;

		shakeOffSet = { 0.0f,0.0f,0.0f };
	}

	if (!isShaking) return;

	Vector3 translate = activeCamera_->GetViewProjection().transform.translate;

	std::uniform_real_distribution<float> dist(-0.2f, 0.2f);

	shakeOffSet = { dist(randomEngine),dist(randomEngine),dist(randomEngine) };
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
