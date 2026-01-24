#pragma once
#include <unordered_set>

// Engine
#include "WorldTransform.h"
#include "CollisionMath.h"

class AABBCollider; // 前方宣言
class SphereCollider; // 前方宣言

class BaseEntity; // 前方宣言

// \brief BaseCollider Collider基底クラス

class BaseCollider
{
public: /// 公開メンバ関数

	/**
	 * \brief  Initialize 初期化
	 * \param  worldTransform ワールド変形情報
	 */
	virtual void Initialize(Kouro::WorldTransform* worldTransform,BaseEntity* owner);
	
	/**
	* \brief  Update 更新
	*/
	virtual void Update();

	/**
	 * \brief  AddPrevCollision 前回の衝突情報にコライダーを追加する
	 * \param  collider コライダー
	 */
	void AddCollision(BaseCollider* collider);

	/**
	* \brief  UpdateCollisionStates 衝突状態を更新する
	*/
	void UpdateCollisionStates();

	/**
	* \brief  当たり判定
	* \param  collider 
	*/
	virtual bool CheckCollision(BaseCollider* other) = 0;

	virtual bool CheckCollisionWithAABB(BaseCollider* other) = 0;

	virtual bool CheckCollisionWithSphere(BaseCollider* other) = 0;

	virtual bool CheckCollisionWithOBB(BaseCollider* other) = 0;

	///================================================================================
	///                                        setter

	/**
	 * \brief  SetCollisionAttribute コリジョン属性を設定する
	 * \return collisionAttribute(コリジョン属性)
	 */
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/**
	 * \brief  SetCollisionMask コリジョンマスクを設定する
	 * \param  CollisionMask(コリジョンマスク)
	 */
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

	///================================================================================
	///                                        getter

	/**
	 * \brief  GetCollisionAttribute コリジョン属性を取得する
	 * \return collisionAttribute(コリジョン属性)
	 */
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }

	/**
	 * \brief  GetCollisionMask コリジョンマスクを取得する
	 * \return collisionMask(コリジョンマスク)
	 */
	uint32_t GetCollisionMask() const { return collisionMask_; }	
	
	/**
	 * \brief  GetColor コライダーの色を取得する
	 * \return color(コライダーの色)
	 */
	bool GetIsAlive() const { return isAlive_; }

	///================================================================================
	///                                        当たり判定

	/**
	* \brief  OnCollisionEnter 衝突開始時の処理
	*/
	virtual void OnCollisionEnter(BaseCollider* other) = 0;

	/**
	* \brief  OnCollisionStay 衝突継続時の処理
	*/
	virtual void OnCollisionStay(BaseCollider* other) = 0;

	/**
	* \brief  OnCollisionExit 衝突終了時の処理
	*/
	virtual void OnCollisionExit(BaseCollider* other) = 0;

protected: /// 非公開メンバ変数

	// コライダーの所有者
	BaseEntity* owner_ = nullptr; 

	// コライダーに関連付けるワールド変換情報
	Kouro::WorldTransform* colliderTransform_ = nullptr;

	// コリジョン属性
	uint32_t collisionAttribute_ = 0xffffffff;

	// コリジョンマスク
	uint32_t collisionMask_ = 0xffffffff;

	// 前フレームで衝突していたコライダーのセット
	std::unordered_set<BaseCollider*> prevCollisions_;

	// 今フレームで衝突しているコライダーのセット
	std::unordered_set<BaseCollider*> currentCollisions_;

	// コライダーの色
	Kouro::Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 有効フラグ
	bool isAlive_ = true;
};