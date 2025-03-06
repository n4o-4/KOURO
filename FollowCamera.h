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

	Vector3 CalculationOffset();

	void CalculationRotate();

	void CalculationTranslate();

private:
	//========================================
	// ターゲット
	WorldTransform* target_ = nullptr;
	//========================================
	// 追従対象の残像座標
	Vector3 interTarget_ = {};
	//========================================
	// オフセット
	Vector3 offset_ = {};
	//========================================
	// 現在のカメラ位置
	Vector3 currentPosition_ = {};

	Vector3 destinationRotate = { 0.0f,0.0f,0.0f };
	// イージング係数
	float easingFactor_ = 0.85f;

	float rotateSpeed_ = 0.04f;
};
