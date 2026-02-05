#include "SphereCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

void SphereCollider::Initialize(Kouro::WorldTransform* worldTransform, BaseEntity* owner)
{
	// 基底クラスの初期化
	BaseCollider::Initialize(worldTransform, owner);

	// 球の初期化
	sphere_ = {
		Kouro::Vector3(0.0f, 0.0f, 0.0f), // 中心座標
		0.5f // 半径
	};
}

void SphereCollider::Update()
{
	BaseCollider::Update();

	Kouro::Matrix4x4 matWorld = colliderTransform_->GetWorldMatrix();

	// ワールド座標系での球の中心座標を更新
	sphere_.center = Kouro::Vector3(
		matWorld.m[3][0],
		matWorld.m[3][1],
		matWorld.m[3][2]
	);
}

bool SphereCollider::CheckCollision(BaseCollider* other)
{
	return other->CheckCollisionWithSphere(this);
}

bool SphereCollider::CheckCollisionWithAABB(BaseCollider* other)
{
	auto c = dynamic_cast<AABBCollider*>(other);
	if (!c) return false;

	return IsCollision(GetSphere(), c->GetAABB());
}

bool SphereCollider::CheckCollisionWithSphere(BaseCollider* other)
{
	auto c = dynamic_cast<SphereCollider*>(other);
	if (!c) return false;

	return IsCollision(GetSphere(), c->GetSphere());
}

bool SphereCollider::CheckCollisionWithOBB(BaseCollider* other)
{
	auto c = dynamic_cast<OBBCollider*>(other);
	if (!c) return false;

	return IsCollision(GetSphere(), c->GetOBB());
}
