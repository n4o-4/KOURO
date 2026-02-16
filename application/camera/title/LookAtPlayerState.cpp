#include "LookAtPlayerState.h"
#include "BaseCamera.h"

void LookAtPlayerState::Enter()
{
	camera_->GetWorldTransform().UpdateMatrix();

	Kouro::Matrix4x4 matWorld = camera_->GetWorldTransform().GetWorldMatrix();

	camera_->GetWorldTransform().SetParent(nullptr);
	
	camera_->GetWorldTransform().SetTranslate({ matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] });
}

void LookAtPlayerState::Update(float deltaTime)
{
	Kouro::Vector3 targetPosition = target_->GetWorldPosition();
	Kouro::Vector3 cameraPosition = camera_->GetWorldTransform().GetWorldPosition();
	Kouro::Vector3 direction = Normalize(targetPosition - cameraPosition);
	float yaw = std::atan2(direction.x, direction.z);
	float pitch = std::asin(-direction.y);
	float roll = 0.0f;
	camera_->GetWorldTransform().SetRotate({ pitch, yaw, roll });
}

void LookAtPlayerState::Exit()
{
}
