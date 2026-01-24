#include "RailAnimation.h"
#define NOMINMAX
#include <algorithm>
#include <cmath>

void RailAnimation::Initialize(std::vector<Kouro::Vector3> controlPoints, int samplePerSegment)
{
	worldTransform_ = std::make_unique<Kouro::WorldTransform>();
	worldTransform_->Initialize();

	controlPoints_ = controlPoints;

	worldTransform_ = std::make_unique<Kouro::WorldTransform>();

	const float cornerRadius = 5.0f; // 調整用

	segments_.clear();

	for (size_t i = 1; i + 1 < controlPoints_.size(); ++i)
	{
		const Kouro::Vector3& prev = controlPoints_[i - 1];
		const Kouro::Vector3& curr = controlPoints_[i];
		const Kouro::Vector3& next = controlPoints_[i + 1];

		if (IsCorner(prev, curr, next))
		{
			auto bez = CreateCornerBezier(
				prev, curr, next, cornerRadius);

			RailSegment seg{};
			seg.type = SegmentType::Bezier;
			seg.start = bez.p0;
			seg.end = bez.p2;
			seg.bezier = bez;

			segments_.push_back(seg);
		}
		else
		{
			RailSegment seg{};
			seg.type = SegmentType::Line;
			seg.start = curr;
			seg.end = next;

			segments_.push_back(seg);
		}
	}

	CreateArcLengthTable(samplePerSegment);
}

Kouro::WorldTransform* RailAnimation::GetWorldTransform(const float progress)
{
	Kouro::Vector3 pos = GetPositionByProgress(progress);

	Kouro::Vector3 look = GetPositionByProgress(progress + lookAheadDistance_);

	worldTransform_->SetTranslate(pos);
	worldTransform_->SetRotate(LookRotation(look - pos));

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
		Kouro::Vector3 prevPoint = controlPoints_[i];
		float segmentLength = 0.0f;
		for (int j = 1; j <= samplePerSegment; ++j)
		{
			float t = static_cast<float>(j) / static_cast<float>(samplePerSegment);
			Kouro::Vector3 currentPoint = { Lerp(controlPoints_[i], controlPoints_[i + 1], t) };
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