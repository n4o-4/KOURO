#pragma once
#include "BaseCollider.h"

class AABBCollider : public BaseCollider
{
public: /// 公開メンバ関数

	/**
	 * \brief  Initialize 初期化
	 * \param  worldTransform ワールド変形情報
	 */

	void Initialize(WorldTransform* worldTransform) override;
	

	/**
	 * \brief  GetAABB AABBを取得する
	 * \return AABB
	 */

	AABB GetAABB() const { return aabb_; }

	/**
	 * \brief AABBの情報を設定する
	 * \parma AABB AABBの最小座標と最大座標
	*/

	void SetAABB(const AABB& aabb) { aabb_ = aabb; }

	/**
	 * \brief  GetCollisionAttribute コリジョン属性を取得する
	 * \return AABB
	 */

	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }

	/**
	 * \brief  SetCollisionAttribute コリジョン属性を設定する
	 * \parma  CollisionAttribute コリジョン属性
	 */

	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/**
	 * \brief  SetCollisionMask コリジョンマスクを設定する
	 * \parma  CollisionMask コリジョンマスク
	 */

	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private: /// 非公開メンバ関数


public: /// 公開メンバ変数

private: /// 非公開メンバ変数
	
	// AABBの最小座標と最大座標

	AABB aabb_ = {};

};

