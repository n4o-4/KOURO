﻿#include "BaseCollider.h"

void BaseCollider::Initialize(WorldTransform* worldTransform)
{
	// コライダーに関連付けるワールド変換情報を設定する

	colliderTransform_ = worldTransform;
}

void BaseCollider::AddCollision(BaseCollider* collider)
{
	// 前回の衝突情報にコライダーを追加

	currentCollisions_.insert(collider);
}

void BaseCollider::UpdateCollisionStates()
{
	// Enter & Stay
	for (BaseCollider* other : currentCollisions_) {
		if (prevCollisions_.count(other)) {
			OnCollisionStay(other);
		}
		else {
			OnCollisionEnter(other);
		}
	}
	// Exit
	for (BaseCollider* other : prevCollisions_) {
		if (!currentCollisions_.count(other)) {
			OnCollisionExit(other);
		}
	}
	// 次フレーム用にセットを入れ替え
	prevCollisions_ = std::move(currentCollisions_);
	currentCollisions_.clear();
}