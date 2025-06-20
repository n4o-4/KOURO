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

	enum class ColliderType
	{
		kNone = -1,               // コライダーなし
		kSphere,                  // 球
		kAABB,                    // 軸平行境界ボックス
		//kOBB,
		//kCapsule,
		//kPlane,
		kCountOfColliderType,
	};

public:

	// 初期化
	void Initialize(WorldTransform* worldTransform, ColliderType type);

	

private: // メンバ変数


	WorldTransform* worldTransform_ = nullptr; // ワールドトランスフォーム

	ColliderType type_ = ColliderType::kNone; // コライダーの種類
};

