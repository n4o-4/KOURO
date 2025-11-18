#pragma once
#include "BaseCamera.h"

#include "MyMath.h"

#include "WorldTransform.h"

// \brief  RailCamera レールカメラ

class RailCamera : public BaseCamera
{
public:

	struct ArcLengthTable
	{
		std::vector<float> lengths; // 各セグメントの長さ

		float totalLength;          // 全体の長さ
	};

public:

	/**
	* \brief  Initialize 初期化
	*/
	void Initialize() override;

	/**
	* \brief  Update 更新
	*/
	void Update() override;

	/**
	* \brief  GetWorldTransform ワールド変換情報を取得する
	* \return worldTransform 
	*/
	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }
	
	/**
	* \brief  GetControlPoints 制御点を取得する
	* \return controlPoints_
	*/
	std::vector<Vector3> GetControlPoints() const { return controlPoints_; }

	/**
	* \brief  SetControlPoints 制御点を設定する
	*/
	void SetControlPoints(const std::vector<Vector3>& points) { controlPoints_ = points; }
	

	void SetIsMove(bool isMove) { isMove_ = isMove; }

private:

	/**
	* \brief  CreateArcLengthTable アーク長テーブルの作成
	*/
	void CreateArcLengthTable(int samplePerSegment);

private:

	// ワールド変換情報
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// 制御点
	std::vector<Vector3> controlPoints_;
	
	const float kMoveTime = 50.0f;

	// 移動タイマー
	float moveTimer_ = 0.0f;

	// アーク長テーブル
	ArcLengthTable arcLengthTable_;

	bool isMove_ = false;

	float distanceTravelled = 0.0f;
};
