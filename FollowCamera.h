#pragma once
#include "BaseCamera.h"

#include "WorldTransform.h"

class FollowCamera : public BaseCamera
{
public:

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	void SetTarget(WorldTransform* target) { target_ = target; }

private:

	WorldTransform* target_ = nullptr;

	Vector3 offset_ = {};
};

