#include "RotateCameraState.h"
#include "BaseCamera.h"

void RotateCameraState::Enter()
{
}

void RotateCameraState::Update(float deltaTime)
{
    Kouro::WorldTransform* worldTransform = &camera_->GetWorldTransform();

	Kouro::Vector3 rotate = worldTransform->GetRotate();

	rotate.y += kRotateSpeed_ * deltaTime;

	camera_->GetWorldTransform().SetRotate(rotate);

	worldTransform->SetTranslate(camera_->CalculationOffset());
}

void RotateCameraState::Exit()
{
}
