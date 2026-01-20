#include "RotateCameraState.h"
#include "BaseCamera.h"

void RotateCameraState::Enter()
{
}

void RotateCameraState::Update(float deltaTime)
{
    WorldTransform* worldTransform = &camera_->GetWorldTransform();

	Vector3 rotate = worldTransform->GetRotate();

	rotate.y += kRotateSpeed_ * deltaTime;

	camera_->GetWorldTransform().SetRotate(rotate);

	Vector3 offset = camera_->GetOffset();

	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);

	offset = TransformNormal(offset, rotateMatrix);

	Vector3 translate = offset;

	worldTransform->SetTranslate(offset);
}

void RotateCameraState::Exit()
{
}
