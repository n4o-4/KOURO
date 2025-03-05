#pragma once
#include "BaseCamera.h"

#include "WorldTransform.h"

#include "Input.h"

class FollowCamera : public BaseCamera
{
public:

	// 初期化
	void Initialize();
	// 更新
	void Update();

	void SetTarget(WorldTransform* target) { target_ = target; }

private:

	void UpdateCameraTranslate();

	void UpdateCameraRotate();

private:
	//========================================
	// ターゲット
	WorldTransform* target_ = nullptr;
	//========================================
	// オフセット
	Vector3 offset_ = {};
	//========================================
	// 現在のカメラ位置
	Vector3 currentPosition_ = {};
	// イージング係数
	float easingFactor_ = 0.85f;
};

