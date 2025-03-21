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

	// 前方向ベクトルを取得
	Vector3 GetForwardDirection() const;

	// 高さによるFOV演出
	void UpdateHeightFOVEffect();

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
	// 目標のカメラ位置
	Vector3 destinationRotate = { 0.0f,0.0f,0.0f };
	// イージング係数
	float easingFactor_ = 0.85f;
	// 回転速度
	float rotateSpeed_ = 0.04f;

	//========================================
	// FOV関連パラメータ
	float baseFOV_ = 45.0f;          // 基本FOV値（地上での視野角）
	float currentFOV_ = 45.0f;       // 現在のFOV
	float targetFOV_ = 45.0f;        // 目標FOV
	float fovLerpSpeed_ = 0.4f;      // FOV補間速度
	float heightFOVIncrease_ = 15.0f; // 最大高度での追加FOV量
	float maxHeightForFOV_ = 30.0f;  // 最大効果を得るための高度
	// 高度追跡用
	float prevPlayerHeight_ = 0.0f;   // 前フレームの高さ（必要に応じて）
};
