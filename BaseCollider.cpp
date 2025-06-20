#include "BaseCollider.h"

void BaseCollider::Initialize(WorldTransform* worldTransform, ColliderType type)
{
	worldTransform_ = worldTransform;

	type_ = type;
}
