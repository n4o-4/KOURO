#pragma once
#include "Kouro.h"

#include "BaseCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

class ColliderManager
{
public:  /// 公開メンバ関数
	
	// \brief  Update 更新

	void Update();

	/**
	 * \brief  AddCollider コライダーを追加する
	 * \param  collider コライダー
	 */

	void AddCollider(BaseCollider* collider) { colliders_.push_back(collider); }

	std::vector<BaseCollider*>& GetColliders() { return colliders_; }

private: /// 非公開メンバ関数

	
	
	// \brief  ClearColliders コライダーのリストをクリアする
	void ClearColliders() { colliders_.clear(); }

	void ScanColliders();

	void CheckCollision(BaseCollider* a, BaseCollider* b);

private: /// 非公開メンバ関数
public:  /// 公開メンバ変数
private: /// 非公開メンバ変数

	std::vector<BaseCollider*> colliders_; // コライダーのリスト
};