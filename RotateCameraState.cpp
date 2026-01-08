#include "RotateCameraState.h"
#include "BaseCamera.h"

void RotateCameraState::Enter()
{
}

void RotateCameraState::Update(float deltaTime)
{
	ViewProjection* viewPro = &camera_->GetViewProjection();

	viewPro->transform.rotate.y += kRotateSpeed_ * deltaTime;

	Vector3 offset = camera_->GetOffset();

	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewPro->transform.rotate);

	offset = TransformNormal(offset, rotateMatrix);

	viewPro->transform.translate = offset;
}

void RotateCameraState::Exit()
{
}
