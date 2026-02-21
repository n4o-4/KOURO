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

	if(rotate.y > static_cast<float>(std::numbers::pi * 2))
	{
		rotate.y -= static_cast<float>(std::numbers::pi * 2);
	}

	camera_->GetWorldTransform().SetRotate(rotate);

	worldTransform->SetTranslate(camera_->CalculationOffset());
}

void RotateCameraState::Exit()
{
}
