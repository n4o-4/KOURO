#include "BaseCollider.h"

void BaseCollider::Initialize(WorldTransform* worldTransform)
{
	// コライダーに関連付けるワールド変換情報を設定する

	worldTransform_ = worldTransform;
}

void BaseCollider::PrevCollisionsClear()
{
	// 衝突していたコライダーのセットをクリア

	prevCollisions_.clear();
}

void BaseCollider::AddPrevCollision(BaseCollider* collider)
{
	// 前回の衝突情報にコライダーを追加

	prevCollisions_.insert(collider);
}
