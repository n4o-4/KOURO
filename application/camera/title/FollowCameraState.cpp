#include "FollowCameraState.h"
#include "BaseCamera.h"

void FollowCameraState::Enter()
{
	camera_->GetWorldTransform().SetParent(target_);
}

void FollowCameraState::Update(float deltaTime)
{
}

void FollowCameraState::Exit()
{
}