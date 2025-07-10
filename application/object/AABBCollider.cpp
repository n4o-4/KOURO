#include "AABBCollider.h"
#include "SphereCollider.h"

void AABBCollider::Initialize(WorldTransform* worldTransform)
{
	// 基底クラスの初期化
	BaseCollider::Initialize(worldTransform);

	// AABBの初期化
	aabb_ = {
        {},
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
