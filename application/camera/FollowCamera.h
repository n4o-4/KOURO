#pragma once
#include "BaseCamera.h"

#include "WorldTransform.h"

#include "Input.h"

// \brief  FollowCamera 追従カメラ

class FollowCamera : public Kouro::BaseCamera
{
public:

	// \brief  Initialize 初期化
	void Initialize() override;

	// \brief  Update 更新
	void Update() override;
	
	/**
	* \brief  SetTarget ターゲットの設定
	* \param  target ターゲットのワールド変換のポインタ
	*/
	void SetTarget(Kouro::WorldTransform* target) { target_ = target; }

private:

	// \brief  CalculationRotate 回転の計算
	void CalculationRotate();

	// \brief  CalculationTranslate 移動の計算
	void CalculationTranslate();

private:

	// ターゲット
	Kouro::WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Kouro::Vector3 interTarget_ = {};

	// オフセット
	Kouro::Vector3 offset_ = {};

	// デフォルトのオフセット（元の値を保持）
	Kouro::Vector3 defaultOffset_ = { 0.0f, 3.0f, -30.0f };

	// 現在のカメラ位置
	Kouro::Vector3 currentPosition_ = {};

	// 目標のカメラ位置
	Kouro::Vector3 destinationRotate = { 0.0f,0.0f,0.0f };

	// イージング係数
	float easingFactor_ = 0.85f;

	// 回転速度
	float rotateSpeed_ = 0.04f;

	float distanceTravelled = 0.0f;
};
