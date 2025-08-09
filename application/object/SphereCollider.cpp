#include "SphereCollider.h"

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