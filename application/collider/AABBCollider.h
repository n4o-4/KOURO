#pragma once
#include "BaseCollider.h"

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

	/**================================================================================
	 * \brief  colorを取得する
	 * \return color
	 */

	Vector4 GetColor() const { return color_; }

private: /// 非公開メンバ関数

public: /// 公開メンバ変数

private: /// 非公開メンバ変数
	
	// AABBの最小座標と最大座標

	AABB aabb_ = {};

	Vector3 center_ = { 0.0f, 0.0f, 0.0f }; //!< AABBの中心座標
};

