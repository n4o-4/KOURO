#include "RailAnimation.h"

void RailAnimation::Initialize(std::vector<Vector3> controlPoints, int samplePerSegment)
{
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	controlPoints_ = controlPoints;

	CreateArcLengthTable(samplePerSegment);
}

WorldTransform* RailAnimation::GetWorldTransform(const float progress)
{
	// 現在位置
	size_t segmentCount = controlPoints_.size() - 1;
	float t = progress * static_cast<float>(segmentCount);
	size_t segmentIndex = static_cast<size_t>(t);
	if (segmentIndex >= segmentCount)
	{
		segmentIndex = segmentCount - 1;
		t = static_cast<float>(segmentCount);
	}
	float localT = t - static_cast<float>(segmentIndex);
	// 線形補間で位置
	Vector3 position = {
		Lerp(controlPoints_[segmentIndex], controlPoints_[segmentIndex + 1], localT)
	};
	// 数フレーム先の位置を取得して進行方向を計算
	float lookAheadT = localT + offsetT_;
	if (lookAheadT > 1.0f && segmentIndex + 1 < segmentCount)
	{
		lookAheadT -= 1.0f;
		segmentIndex += 1;
	}
	Vector3 lookAheadPosition =
	{
		Lerp(controlPoints_[segmentIndex], controlPoints_[segmentIndex + 1], lookAheadT)
	};
	// 進行方向ベクトル
	Vector3 forward =
	{
		lookAheadPosition.x - position.x,
		lookAheadPosition.y - position.y,
		lookAheadPosition.z - position.z
	};
	forward = Normalize(forward);
	// forwardベクトルからオイラー角を算出
	float yaw = std::atan2(forward.x, forward.z);
	float pitch = std::asin(-forward.y);
	float roll = 0.0f; // 必要に応じて
	Vector3 rotation = { pitch, yaw, roll };

	// ワールドトランスフォームに設定
	worldTransform_->transform.translate = position;
	worldTransform_->transform.rotate = rotation;
	worldTransform_->UpdateMatrix();
	return worldTransform_.get();
}

void RailAnimation::CreateArcLengthTable(int samplePerSegment)
{
	// Linear
	arcLengthTable_.lengths.clear();
	arcLengthTable_.lengths.push_back(0.0f);
	float totalLength = 0.0f;

	// 各セグメントごとに長さを計算
	for (size_t i = 0; i < controlPoints_.size() - 1; ++i)
	{
		// セグメントごとの長さを計算
		Vector3 prevPoint = controlPoints_[i];
		float segmentLength = 0.0f;
		for (int j = 1; j <= samplePerSegment; ++j)
		{
			float t = static_cast<float>(j) / static_cast<float>(samplePerSegment);
			Vector3 currentPoint = { Lerp(controlPoints_[i], controlPoints_[i + 1], t) };
			segmentLength += sqrtf(
				(currentPoint.x - prevPoint.x) * (currentPoint.x - prevPoint.x) +
				(currentPoint.y - prevPoint.y) * (currentPoint.y - prevPoint.y) +
				(currentPoint.z - prevPoint.z) * (currentPoint.z - prevPoint.z)
			);
			prevPoint = currentPoint;
		}
		totalLength += segmentLength;
		arcLengthTable_.lengths.push_back(totalLength);
	}
}