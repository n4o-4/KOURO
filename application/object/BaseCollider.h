#pragma once
#include <unordered_set>

// Engine
#include "WorldTransform.h"
#include "CollisionMath.h"

class AABBCollider; // 前方宣言
class SphereCollider; // 前方宣言

class BaseCollider
{
public: /// 公開メンバ関数

	/**
	 * \brief  Initialize 初期化
	 * \param  worldTransform ワールド変形情報
	 */

	virtual void Initialize(WorldTransform* worldTransform);
	
	// \brief  Update 更新

	virtual void Update() = 0;

	/**
	 * \brief  AddPrevCollision 前回の衝突情報にコライダーを追加する
	 * \param  collider コライダー
	 */

	void AddCollision(BaseCollider* collider);


	void UpdateCollisionStates();


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
	 * \return collisionAttribute(コリジョン属性)
	 */

	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }

	/**================================================================================
	 * \brief  GetCollisionMask コリジョンマスクを取得する
	 * \return collisionMask(コリジョンマスク)
	 */

	uint32_t GetCollisionMask() const { return collisionMask_; }	

	///================================================================================
	///                                        当たり判定

	// \brief  OnCollisionEnter 衝突開始時の処理

	virtual void OnCollisionEnter(BaseCollider* other) = 0;

	// \brief  OnCollisionStay 衝突中の処理

	virtual void OnCollisionStay(BaseCollider* other) = 0;

	// \brief  OnCollisionExit 衝突終了時の処理

	virtual void OnCollisionExit(BaseCollider* other) = 0;

protected:

	WorldTransform* colliderTransform_ = nullptr; // ワールドトランスフォーム

	uint32_t collisionAttribute_ = 0xffffffff; // コリジョン属性

	uint32_t collisionMask_ = 0xffffffff; // コリジョンマスク

	std::unordered_set<BaseCollider*> prevCollisions_; // 前フレームで衝突していたコライダーのセット

	std::unordered_set<BaseCollider*> currentCollisions_; // 現フレームで衝突しているコライダーのセット

};

