#include "AABBCollider.h"

void AABBCollider::Initialize(WorldTransform* worldTransform)
{
	// Šî’êƒNƒ‰ƒX‚Ì‰Šú‰»
	BaseCollider::Initialize(worldTransform);
}

void AABBCollider::Update()
{
	Vector3 center = Vector3(worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]);
}
