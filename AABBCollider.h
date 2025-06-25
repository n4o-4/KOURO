#pragma once
#include "BaseCollider.h"

class AABBCollider : public BaseCollider
{
public: /// 公開メンバ関数

	// 初期化

	void Initialize(WorldTransform* worldTransform) override;
	

	/**
    * @brief AABBの情報を取得する
    *
    * @return AABBの情報
    */

	AABB GetAABB() const { return aabb_; }

	/**
	 * @brief AABBの情報を設定する
	*/

	void SetAABB(const AABB& aabb) { aabb_ = aabb; }

	// collisionAttributeを設定

	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	// collisionMaskを設定

	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private: /// 非公開メンバ関数


public: /// 公開メンバ変数

private: /// 非公開メンバ変数
	
	// AABBの最小座標と最大座標

	AABB aabb_ = {};

};

