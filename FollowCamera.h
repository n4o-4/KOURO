#pragma once
#include "BaseCamera.h"
#include "WorldTransform.h"

class FollowCamera :    public BaseCamera
{
public:

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// ターゲットの設定
	void SetTarget(WorldTransform* target) { target_ = target; }

	// ターゲットの設定
	void SetOffset(const Vector3& offset) { offset_ = offset; }

private: // メンバ変数e
	
	WorldTransform* target_ = nullptr;

	Vector3 offset_ = {};

};

