#pragma once
#include "BaseCollider.h"

// \brief SphereCollider Sphere型のコライダークラス

class SphereCollider : public BaseCollider
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
	 * \brief Sphereの情報を設定する
	 * \param Sphere Sphereの数値を設定
	*/
	void SetSphere(const Sphere& sphere) { sphere_ = sphere; }

	///================================================================================
	///                                        getter

	/**================================================================================
	 * \brief  GetSphere Sphereを取得する
	 * \return Sphere
	 */
	Sphere GetSphere() const { return sphere_; }

private: /// 非公開メンバ変数

	// AABBの最小座標と最大座標

	Sphere sphere_; // 球の中心と半径
};