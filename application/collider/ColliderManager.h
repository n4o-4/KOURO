#pragma once

#include <variant>

#include "Kouro.h"

#include "BaseCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "OBBCollider.h"

using ColliderVariant = std::variant<
	AABBCollider*,
	SphereCollider*,
	OBBCollider*
>;

using CollisionPair = std::pair<BaseCollider*, BaseCollider*>;

// \brief ColliderManager collider管理クラス

class ColliderManager
{
public:  /// 公開メンバ関数
	
	// \brief  Update 更新
	void Update();

	/**
	 * \brief  AddCollider コライダーを追加する
	 * \param  collider コライダー
	 */
	void AddCollider(BaseCollider* collider);
	/**
	* \brief  GetColliders コライダーリストを取得する
	* \return colliders_ コライダーリスト
	*/
	std::vector<ColliderVariant> GetColliders() { return colliders_; }

private:

	void CleanUpColliders();

private: /// 非公開メンバ関数

	// \brief  ClearColliders コライダーのリストをクリアする
	void ClearColliders() { colliders_.clear(); }

	// \brief  ScanColliders  コライダーリスト内のコライダー同士の当たり判定を呼び出す
	void ScanColliders();

	// \brief  CheckCollision コライダー同士の当たり判定をとる
	void CheckCollision(ColliderVariant a, ColliderVariant b);

private: /// 非公開メンバ変数

	std::vector<ColliderVariant> colliders_; // コライダーのリスト
};