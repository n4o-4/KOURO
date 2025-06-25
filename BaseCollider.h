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

public: /// 公開メンバ関数

	/**
	 * \brief  Initialize 初期化
	 * \param worldTransform ワールド変形情報
	 */

	virtual void Initialize(WorldTransform* worldTransform);
	
	/**
	 * \brief  Update 更新
	 */

	virtual void Update() = 0;

	///================================================================================
	///                                        setter

	/**================================================================================
	 * \brief  SetCollisionAttribute コリジョン属性を設定する
	 * \return collisionAttribute(コリジョン属性)
	 */

	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/**================================================================================
	 * \brief  SetCollisionMask コリジョンマスクを設定する
	 * \parma  CollisionMask コリジョンマスク
	 */

	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

	///================================================================================
	///                                        getter

	/**================================================================================
	 * \brief  GetCollisionAttribute コリジョン属性を取得する
	 * \return AABB
	 */

	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }

	/**================================================================================
	 * \brief  GetCollisionMask コリジョンマスクを取得する
	 * \return collisionMask(コリジョンマスク)
	 */

	uint32_t GetCollisionMask() const { return collisionMask_; }	

protected:

	WorldTransform* worldTransform_ = nullptr; // ワールドトランスフォーム

	uint32_t collisionAttribute_ = 0; // コリジョン属性

	uint32_t collisionMask_ = 0; // コリジョンマスク
};

