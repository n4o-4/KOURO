#include "AABBCollider.h"
#include "SphereCollider.h"
#include "OBBCollider.h"

void AABBCollider::Initialize(Kouro::WorldTransform* worldTransform, BaseEntity* owner)
{
	// 基底クラスの初期化
	BaseCollider::Initialize(worldTransform, owner);

	// AABBの初期化
	aabb_ = {
        {},
		Kouro::Vector3(-1.0f, -1.0f, -1.0f), // 最小点
		Kouro::Vector3(1.0f, 1.0f, 1.0f)  // 最大点
	};
}

void AABBCollider::Update()
{
	// 基底クラスの更新
	BaseCollider::Update();

    // ワールド座標系でのAABB中心座標を取得（例：今後の拡張用）
    aabb_.center = Kouro::Vector3(
        colliderTransform_->matWorld_.m[3][0],
        colliderTransform_->matWorld_.m[3][1],
        colliderTransform_->matWorld_.m[3][2]
    );
}

bool AABBCollider::CheckCollision(BaseCollider* other)
{
	return other->CheckCollisionWithAABB(this);
}

bool AABBCollider::CheckCollisionWithAABB(BaseCollider* other)
{
	auto c = dynamic_cast<AABBCollider*>(other);
	if (!c) return false; // 安全確認

	return IsCollision(GetAABB(), c->GetAABB());
}

bool AABBCollider::CheckCollisionWithSphere(BaseCollider* other)
{
	auto c = dynamic_cast<SphereCollider*>(other);
	if (!c) return false;

	return IsCollision(GetAABB(), c->GetSphere());
}

bool AABBCollider::CheckCollisionWithOBB(BaseCollider* other)
{
	auto c = dynamic_cast<OBBCollider*>(other);
	if (!c) return false;

	return IsCollision(GetAABB(), c->GetOBB());
}
