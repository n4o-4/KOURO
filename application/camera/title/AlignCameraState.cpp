#include "AlignCameraState.h"
#include "BaseCamera.h"

void AlignCameraState::Enter()
{
	camera_->GetWorldTransform().SetParent(target_);
}

void AlignCameraState::Update(float deltaTime)
{
	Kouro::Vector3 targetRotate = target_->GetRotate();
	Kouro::Vector3 cameraRotate = camera_->GetWorldTransform().GetRotate();

	// 角度差を求める
	float diff = targetRotate.y - cameraRotate.y;

	// -π ～ π に正規化（最短差分にする）
	while (diff > std::numbers::pi)  diff -= 2.0f * std::numbers::pi;
	while (diff < -std::numbers::pi) diff += 2.0f * std::numbers::pi;

	// 最短方向にイージング
	cameraRotate.y += diff * 0.04f;

	camera_->GetWorldTransform().SetRotate(cameraRotate);
	camera_->GetWorldTransform().SetTranslate(camera_->CalculationOffset());

	// 収束判定
	if (std::abs(diff) < 0.01f)
	{
		Kouro::Vector3 cameraRotate = camera_->GetWorldTransform().GetRotate();

		cameraRotate.y = 0.0f;

		camera_->GetWorldTransform().SetRotate(cameraRotate);

		isFinished_ = true;
	}
}

void AlignCameraState::Exit()
{
}
