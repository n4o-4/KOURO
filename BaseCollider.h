#pragma once
// Engine
#include "Kouro.h"

class BaseCollider
{
public:

	struct Sphere
	{
		Vector3 center; // 中心
		float radius; // 半径
	};

	struct AABB
	{
		Vector3 min; // 最小座標
		Vector3 max; // 最大座標
	};

public:

	// 初期化
	virtual void Initialize(WorldTransform* worldTransform);

private: // メンバ変数

	WorldTransform* worldTransform_ = nullptr; // ワールドトランスフォーム

protected:

	uint32_t collisionAttribute_ = 0; // コリジョン属性

	uint32_t collisionMask_ = 0; // コリジョンマスク
};

