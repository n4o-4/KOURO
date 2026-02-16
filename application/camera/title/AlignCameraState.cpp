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

	// ターゲットの回転にイージングをかけてカメラの回転を更新
	cameraRotate.y = Kouro::Lerp(cameraRotate.y, targetRotate.y, 0.04f);

	camera_->GetWorldTransform().SetRotate(cameraRotate);

	camera_->GetWorldTransform().SetTranslate(camera_->CalculationOffset());

	// カメラの回転がターゲットの回転に十分近づいたら状態を終了
	if (std::abs(cameraRotate.y - targetRotate.y) < 0.01f)
	{
		isFinished_ = true;
	}
}

void AlignCameraState::Exit()
{
}
