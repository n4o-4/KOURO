#pragma once
#include "BaseCollider.h"

// \brief AABBCollider AABB型のコライダークラス

class AABBCollider : public BaseCollider
{
public: /// 公開メンバ関数

	/**
	 * \brief  Initialize 初期化
	 * \param  worldTransform ワールド変形情報
	 */
	void Initialize(WorldTransform* worldTransform, BaseEntity* owner) override;

	// \brief  Update 更新
	void Update() override;
	
	bool CheckCollision(BaseCollider* other) override;

	bool CheckCollisionWithAABB(BaseCollider* other) override;

	bool CheckCollisionWithSphere(BaseCollider* other) override;

	bool CheckCollisionWithOBB(BaseCollider* other) override;

	///================================================================================
	///                                        setter

	/**
	 * \brief AABBの情報を設定する
	 * \parma AABB AABBの最小座標と最大座標
	*/
	void SetAABB(const AABB& aabb) { aabb_ = aabb; }

	///================================================================================
	///                                        getter
	
	/**
	 * \brief  GetAABB AABBを取得する
	 * \return AABB
	 */
	AABB GetAABB() const { return aabb_; }

	/**
	 * \brief  colorを取得する
	 * \return color
	 */
	Vector4 GetColor() const { return color_; }

private: /// 非公開メンバ変数
	
	// AABB
	AABB aabb_ = {};
};

