#pragma once
#include "WorldTransform.h"
#include "Rail.h"

constexpr float kEndOfCurveT = 1.0f;             //!< 曲線tの終端
constexpr float kLookAheadMultiplier = 2.0f;     //!< 先読み距離倍率
constexpr float kStartDistance = 0.0f;           //!< 開始距離
constexpr int kPreviousIndexOffset = 1;          //!< インデックス補正

/// \brief レール状の位置計算

class RailFollower
{
public:

	/**
	* \brief  初期化
	* \param  rail     : 参照元のレール
	* \param  moveTime : 1周当たりの時間
	*/
	void Initialize(const Rail& rail,float moveTime);

	/**
	* \brief  更新
	* \param  deltaTime : 経過時間
	*/
	void Update(float deltaTime);

	/**
	* \brief  ワールドトランスフォームの取得
	* \return ワールドトランスフォーム
	*/
	const Kouro::WorldTransform& GetWorldTransform() { return worldTransform_; }

	/**
	* \brief  移動距離の設定
	* \param  distance : 移動距離
	*/
	void SetDistanceTravelled(float distance) { distanceTravelled_ = distance; }

	/**
	* \brief  先読み距離の設定
	* \param  lookAheadDistance : 先読み距離
	*/
	void SetLookAheadDistance(float lookAheadDistance) { lookAheadDistance_ = lookAheadDistance; }

private: // メンバ変数

	Kouro::WorldTransform worldTransform_; //!< ワールドトランスフォーム
	RailData::ArcLengthTable arcLengthTable_; //!< 弧長データのポインタ
	std::vector<Kouro::Vector3> controlPoints_;
	float distanceTravelled_ = 0.0f; //!< 累計移動距離
	float moveTime_ = 0.0f; //!< 移動時間

	float lookAheadDistance_ = 0.1f; //!< 先読み距離

private: // メンバ定数
};

