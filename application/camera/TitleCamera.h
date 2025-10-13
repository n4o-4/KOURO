#pragma once
#include "BaseCamera.h"
#include "WorldTransform.h"
class TitleCamera : public BaseCamera
{
public:

	// 初期化
	void Initialize();
	// 更新
	void Update();

	void SetTarget(WorldTransform* target) { target_ = target; }

	void SetIsRotate(bool isRotate) { isRotate_ = isRotate; }

	void SetIsDeparture(bool isDeparture) { isDeparture_ = isDeparture; }

	bool GetIsDeparture() const { return isDeparture_; }

private:

	Vector3 CalculationOffset();

	void CalculationRotate();

	void CalculationTranslate();

	void FollowTarget();

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
	// デフォルトのオフセット（元の値を保持）
	Vector3 defaultOffset_ = { 0.0f, 3.0f, -30.0f };

	//========================================
	// 現在のカメラ位置
	Vector3 currentPosition_ = {};
	// 目標のカメラ位置
	Vector3 destinationRotate = { 0.0f,0.0f,0.0f };
	// イージング係数
	float easingFactor_ = 0.85f;
	// 回転速度
	float rotateSpeed_ = 0.04f;

	bool isRotate_ = false;

	bool isDeparture_ = false;
};

