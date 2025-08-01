﻿#pragma once
#include "BaseCollider.h"

class SphereCollider; // 前方宣言

class AABBCollider : public BaseCollider
{
public: /// 公開メンバ関数

	/**
	 * \brief  Initialize 初期化
	 * \param  worldTransform ワールド変形情報
	 */

	void Initialize(WorldTransform* worldTransform) override;

	// \brief  Update 更新

	void Update() override;
	
	///================================================================================
	///                                        setter

	/**
	 * \brief AABBの情報を設定する
	 * \parma AABB AABBの最小座標と最大座標
	*/

	void SetAABB(const AABB& aabb) { aabb_ = aabb; }

	///================================================================================
	///                                        getter
	
	/**================================================================================
	 * \brief  GetAABB AABBを取得する
	 * \return AABB
	 */

	AABB GetAABB() const { return aabb_; }

	///================================================================================
	///                                        当たり判定計算
	///                                        visitorパターンお試し

	/**================================================================================
	 * \brief  CheckCollision
	 * \return
	 */

	bool CheckCollision(BaseCollider* other) override;

	bool CheckCollisionWithAABB(AABBCollider* other) override;

	bool CheckCollisionWithSphere(SphereCollider* other)override;

private: /// 非公開メンバ関数

public: /// 公開メンバ変数

private: /// 非公開メンバ変数
	
	// AABBの最小座標と最大座標

	AABB aabb_ = {};

	Vector3 center_ = { 0.0f, 0.0f, 0.0f }; //!< AABBの中心座標

};

