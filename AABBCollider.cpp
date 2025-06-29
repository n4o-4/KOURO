#include "AABBCollider.h"
#include "SphereCollider.h"

void AABBCollider::Initialize(WorldTransform* worldTransform)
{
	// 基底クラスの初期化
	BaseCollider::Initialize(worldTransform);

	// AABBの初期化
	aabb_ = {
		Vector3(-0.5f, -0.5f, -0.5f), // 最小点
		Vector3(0.5f, 0.5f, 0.5f)  // 最大点
	};
}

void AABBCollider::Update()
{
    // ワールド座標系でのAABB中心座標を取得（例：今後の拡張用）
    aabb_.center = Vector3(
        colliderTransform_->matWorld_.m[3][0],
        colliderTransform_->matWorld_.m[3][1],
        colliderTransform_->matWorld_.m[3][2]
    );
}

bool AABBCollider::CheckCollision(BaseCollider* other)
{
    // otherの型に応じた衝突判定を呼び出す
    return other->CheckCollisionWithAABB(this);
}

bool AABBCollider::CheckCollisionWithAABB(AABBCollider* other)
{
    // 自身のAABBとotherのAABBで衝突判定
    return IsAABBCollision(aabb_, other->GetAABB());
}

bool AABBCollider::CheckCollisionWithSphere(SphereCollider* other)
{
    // 自身のAABBとotherのSphereで衝突判定
    return IsAABBSphereCollision(aabb_, other->GetSphere());
}
