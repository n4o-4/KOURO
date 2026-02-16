#include "TransitionCameraState.h"
#include "BaseCamera.h"

void TransitionCameraState::Enter()
{
	camera_->GetWorldTransform().SetParent(target_);
}

void TransitionCameraState::Update(float deltaTime)
{
	Kouro::Vector3 targetRotate = target_->GetRotate();

	Kouro::Vector3 cameraRotate = camera_->GetWorldTransform().GetRotate();

	// ターゲットの回転にイージングをかけてカメラの回転を更新
	cameraRotate.y = Kouro::Lerp(cameraRotate.y, targetRotate.y, 0.04f);

	camera_->GetWorldTransform().SetRotate(cameraRotate);

	camera_->GetWorldTransform().SetTranslate(camera_->CalculationOffset());
}

void TransitionCameraState::Exit()
{
}
