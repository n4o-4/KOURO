#pragma once
#include "BaseCollider.h"

// \brief OBBCollider OBB型ぼコライダークラス

class OBBCollider : public BaseCollider
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
	 * \brief OBBの情報を設定する
	 * \param OBB OBBの最小座標と最大座標
	*/
	void SetOBB(const OBB& obb) { obb_ = obb; }

	///================================================================================
	///                                        getter

	/**================================================================================
	 * \brief  GetOBBを取得する
	 * \return OBB
	 */
	OBB GetOBB() const { return obb_; }

	/**================================================================================
	 * \brief  colorを取得する
	 * \return color
	 */
	Vector4 GetColor() const { return color_; }

private: /// 非公開メンバ関数

public: /// 公開メンバ変数

private: /// 非公開メンバ変数

	// AABBの最小座標と最大座標

	OBB obb_ = {};
};

