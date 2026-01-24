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

	Kouro::Vector3 offset = camera_->GetOffset();

	Kouro::Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);

	offset = TransformNormal(offset, rotateMatrix);

	Kouro::Vector3 translate = offset;

	worldTransform->SetTranslate(offset);
}

void RotateCameraState::Exit()
{
}
