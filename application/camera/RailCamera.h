#pragma once
#include "BaseCamera.h"

#include "MyMath.h"

#include "WorldTransform.h"

class RailCamera : public BaseCamera
{
public:

	struct ArcLengthTable
	{
		std::vector<float> lengths; // 各セグメントの長さ

		float totalLength;          // 全体の長さ
	};

public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	//getter
	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

	void SetControlPoints(const std::vector<Vector3>& points) { controlPoints_ = points; }

	std::vector<Vector3> GetControlPoints() const { return controlPoints_; }

	void SetIsMove(bool isMove) { isMove_ = isMove; }

private:

	void CreateArcLengthTable(int samplePerSegment);

private:

	std::unique_ptr<WorldTransform> worldTransform_ = nullptr; //!< ワールド変形情報

	std::vector<Vector3> controlPoints_; //!< カメラの制御点
	
	const float kMoveTime = 50.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間

	ArcLengthTable arcLengthTable_;

	bool isMove_ = false;

	float distanceTravelled = 0.0f;
};
