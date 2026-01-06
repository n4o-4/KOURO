#include "Rail.h"

void Rail::Initialize(const std::vector<Vector3>& controlPoints, int samplePerSegment)
{
	controlPoints_ = controlPoints;

	CreateArcLengthTable(samplePerSegment);
}

void Rail::CreateArcLengthTable(int samplePerSegment)
{
	int numSegments = static_cast<int>(controlPoints_.size()) - kCatmullRomPointOffset;
	int totalSamples = numSegments * samplePerSegment;

	arcLengthTable_.lengths.resize(totalSamples + kIncludeStartPoint);
	arcLengthTable_.lengths[0] = 0.0f;
	arcLengthTable_.totalLength = 0.0f;

	Vector3 prevPoint = CatmullRomPosition(controlPoints_, 0.0f);

	for (int i = 1; i <= totalSamples; ++i)
	{
		float t = static_cast<float>(i) / totalSamples; // 0?1 正規化
		Vector3 currPoint = CatmullRomPosition(controlPoints_, t);

		float segmentLength = Length(currPoint - prevPoint);
		arcLengthTable_.lengths[i] = arcLengthTable_.lengths[i - 1] + segmentLength;

		prevPoint = currPoint;
	}

	arcLengthTable_.totalLength = arcLengthTable_.lengths.back();
}
