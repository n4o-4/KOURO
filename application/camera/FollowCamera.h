#pragma once
#include "BaseCamera.h"

#include "WorldTransform.h"

#include "Input.h"

/// <summary>
/// 追従カメラ
/// ターゲットを追従するカメラ
/// </summary>

class FollowCamera : public BaseCamera
{
public:

	/// <summary>
	/// 初期化
	/// 基底クラスの初期化を行い、オフセットの初期化、初期位置の設定を行う
	/// </summary>

	void Initialize();

	/// <summary>
	/// 更新
	/// ターゲットが設定されていない場合はアサート、オフセットの計算、回転の計算、基底クラスの更新を行う
	/// </summary>

	void Update();

	void SetTarget(WorldTransform* target) { target_ = target; }

private:

	/// <summary>
	/// オフセットの計算
	/// videwProjectionの回転を考慮したオフセットを計算して返す
	/// </summary>
	/// <returns></returns>

	Vector3 CalculationOffset();

	/// <summary>
	/// 
	/// </summary>

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
};
