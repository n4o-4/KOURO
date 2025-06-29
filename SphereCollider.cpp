#include "SphereCollider.h"
#include "AABBCollider.h"

void SphereCollider::Initialize(WorldTransform* worldTransform)
{
	// 基底クラスの初期化
	BaseCollider::Initialize(worldTransform);

	// 球の初期化
	sphere_ = {
		Vector3(0.0f, 0.0f, 0.0f), // 中心座標
		0.5f // 半径
	};
}

void SphereCollider::Update()
{
	// ワールド座標系での球の中心座標を更新
	sphere_.center = Vector3(
		colliderTransform_->matWorld_.m[3][0],
		colliderTransform_->matWorld_.m[3][1],
		colliderTransform_->matWorld_.m[3][2]
	);
}

bool SphereCollider::CheckCollision(BaseCollider* other)
{
	// otherの型に応じた衝突判定を呼び出す
	return other->CheckCollisionWithSphere(this);
}

bool SphereCollider::CheckCollisionWithAABB(AABBCollider* other)
{
	// otherのAABBと自身の球で衝突判定
	return IsAABBSphereCollision(other->GetAABB(), sphere_);
}

bool SphereCollider::CheckCollisionWithSphere(SphereCollider* other)
{
	// 自身の球とotherの球で衝突判定
	return IsSphereCollision(sphere_, other->GetSphere());
}
