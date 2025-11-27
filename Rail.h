#pragma once

#include "WorldTransform.h"

struct ArcLengthTable
{
	std::vector<float> lengths; // 各セグメントの長さ

	float totalLength;          // 全体の長さ
};

// \brief Rail
// 3次ベジェ曲線やスプラインに基づくカメラやオブジェクトの軌道管理クラス。
// 制御点（controlPoints）から曲線を生成し、距離に基づく移動をサポート。
// ArcLengthTableを用いて移動距離と時間の補間を行い、WorldTransformを通じて位置・向きを取得可能。

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

	const float kMoveTime = 50.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間

	ArcLengthTable arcLengthTable_;

	std::unique_ptr<WorldTransform> worldTransform_;

	float distanceTravelled = 0.0f;
};

