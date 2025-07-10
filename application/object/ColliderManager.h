#pragma once

#include <variant>

#include "Kouro.h"

#include "BaseCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "OBBCollider.h"

using ColliderVariant = std::variant <
	AABBCollider*,
	SphereCollider*,
	OBBCollider*
>;
	

class ColliderManager
{
public:  /// 公開メンバ関数
	
	// \brief  Update 更新

	void Update();

	/**
	 * \brief  AddCollider コライダーを追加する
	 * \param  collider コライダー
	 */

	void AddCollider(ColliderVariant collider) { colliders_.push_back(collider); }

private: /// 非公開メンバ関数

	
	
	// \brief  ClearColliders コライダーのリストをクリアする
	void ClearColliders() { colliders_.clear(); }

	void ScanColliders();

	void CheckCollision(ColliderVariant a, ColliderVariant b);

private: /// 非公開メンバ関数
public:  /// 公開メンバ変数
private: /// 非公開メンバ変数

	std::vector<ColliderVariant> colliders_; // コライダーのリスト
};