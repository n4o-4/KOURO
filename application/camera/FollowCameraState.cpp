#include "FollowCameraState.h"
#include "BaseCamera.h"

void FollowCameraState::Enter()
{
}

void FollowCameraState::Update(float deltaTime)
{
	camera_->GetWorldTransform().SetParent(target_);
}

void FollowCameraState::Exit()
{
}
