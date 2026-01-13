#include "FollowCameraState.h"
#include "BaseCamera.h"

void FollowCameraState::Enter()
{
}

void FollowCameraState::Update(float deltaTime)
{
	// 追従対象のワールド行列を取得
	Matrix4x4 pMatWorld = target_->matWorld_;

	Matrix4x4 matWorld = camera_->GetViewProjection().matWorld_;

	matWorld = Multiply(matWorld, pMatWorld);
	
	camera_->GetViewProjection().matView_ = Inverse(matWorld);

	camera_->GetViewProjection().TransferMatrix();
}

void FollowCameraState::Exit()
{
}
