#pragma once
#include "BaseCamera.h"
#include "WorldTransform.h"

#include "ICameraState.h"

// \brief TitleCamera タイトルで使うカメラ

class TitleCamera : public Kouro::BaseCamera
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
	//void SetTarget(WorldTransform* target) { target_ = target; }

	/**
	* \brief  SetIsRotate 回転フラグの設定
	* \param  isRotate 回転フラグ
	*/
	void SetIsRotate(bool isRotate) { isRotate_ = isRotate; }

	/**
	* \brief  SetIsDeparture 離脱フラグの設定
	* \param  isDeparture 離脱フラグ
	*/
	void SetIsDeparture(bool isDeparture) { isDeparture_ = isDeparture; }

	/**
	* \brief  GetIsDeparture 離脱フラグの取得
	* \return isDeparture_ 離脱フラグ
	*/
	bool GetIsDeparture() const { return isDeparture_; }

private:

	// \brief  CalculationRotate 回転の計算
	void CalculationRotate();

	// \brief  CalculationTranslate 移動の計算
	void CalculationTranslate();

	// \brief  FollowTarget ターゲットの追従
	void FollowTarget();

private:

	// ターゲット
	//WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Kouro::Vector3 interTarget_ = {};

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

	// 回転フラグ
	bool isRotate_ = false;

	bool isRun_ = false;

	// 離脱フラグ
	bool isDeparture_ = false;
};

