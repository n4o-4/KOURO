#include "BaseCollider.h"

void BaseCollider::Initialize(WorldTransform* worldTransform)
{
	// コライダーに関連付けるワールド変換情報を設定する

	worldTransform_ = worldTransform;
}
