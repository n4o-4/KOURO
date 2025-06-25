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

	/**
	 * \brief  Initialize 初期化
	 * \param worldTransform ワールド変形情報
	 */

	virtual void Initialize(WorldTransform* worldTransform);
	
	/**
	 * \brief  Update 更新
	 */

	virtual void Update() = 0;

private: // メンバ変数

	WorldTransform* worldTransform_ = nullptr; // ワールドトランスフォーム

protected:

	uint32_t collisionAttribute_ = 0; // コリジョン属性

	uint32_t collisionMask_ = 0; // コリジョンマスク
};

