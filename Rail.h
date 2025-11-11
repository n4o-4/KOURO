#pragma once

#include "WorldTransform.h"

struct ArcLengthTable
{
	std::vector<float> lengths; // 各セグメントの長さ

	float totalLength;          // 全体の長さ
};


class Rail
{
public:

	void Initialize(const std::vector<Vector3>& controlPoints, int samplePerSegment = 100);

	void Update();

	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

private:

	void CreateArcLengthTable(int samplePerSegment);

private:

	std::vector<Vector3> controlPoints_; //!< カメラの制御点

	const float kMoveTime = 100.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間

	ArcLengthTable arcLengthTable_;

	std::unique_ptr<WorldTransform> worldTransform_;
};

