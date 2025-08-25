#pragma once

#include <variant>

#include "Kouro.h"

#include "BaseCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "OBBCollider.h"

using ColliderVariant = std::variant<
	std::shared_ptr<AABBCollider>,
	std::shared_ptr<SphereCollider>,
	std::shared_ptr<OBBCollider>
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

	template <typename T>
	void AddCollider(std::shared_ptr<T> collider) 
	{
		colliders_.emplace_back(std::move(collider));
	}

	std::vector<ColliderVariant> GetColliders() { return colliders_; }

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