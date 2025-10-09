#pragma once
#include "BaseCollider.h"

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

	///================================================================================
	///                                        setter

	/**
	 * \brief Sphereの情報を設定する
	 * \parma Sphere Sphereの数値を設定
	*/

	void SetSphere(const Sphere& sphere) { sphere_ = sphere; }

	///================================================================================
	///                                        getter

	/**================================================================================
	 * \brief  GetSphere Sphereを取得する
	 * \return Sphere
	 */

	Sphere GetSphere() const { return sphere_; }

private: /// 非公開メンバ関数

public: /// 公開メンバ変数

private: /// 非公開メンバ変数

	// AABBの最小座標と最大座標

	Sphere sphere_; // 球の中心と半径
};