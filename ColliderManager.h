#pragma once
#include "Kouro.h"

#include "BaseCollider.h"

class ColliderManager
{
public:  /// 公開メンバ関数
	
	
	// \brief  Update 更新

	void Update();


private: /// 非公開メンバ関数
public:  /// 公開メンバ変数
private: /// 非公開メンバ変数

	std::vector<BaseCollider*> colliders_; // コライダーのリスト
	
};