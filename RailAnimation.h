#pragma once
#include <memory>
#include <vector>

#include "MyMath.h"
#include "WorldTransform.h"

class RailAnimation
{
public: // メンバ構造体

	struct ArcLengthTable
	{
		std::vector<float> lengths; // 各セグメントの長さ
		float totalLength;          // 全体の長さ
	};

	enum class SegmentType
	{
		Line,
		Bezier
	};

	struct BezierCurve
	{
		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
	};

	struct RailSegment
	{
		SegmentType type;
		Vector3 start;
		Vector3 end;
		BezierCurve bezier;
		float length;
	};

public: // メンバ関数

	void Initialize(std::vector<Vector3> controlPoints, int samplePerSegment = 100);

	WorldTransform* GetWorldTransform(const float progress);

private: // メンバ関数

	void CreateArcLengthTable(int samplePerSegment);

	bool IsCorner(
		const Vector3& prev,
		const Vector3& curr,
		const Vector3& next)
	{
		Vector3 a = Normalize(curr - prev);
		Vector3 b = Normalize(next - curr);

		float dot = Dot(a, b);
		return dot < 0.99f;
	}
	
	BezierCurve CreateCornerBezier(
		const Vector3& prev,
		const Vector3& curr,
		const Vector3& next,
		float radius)
	{
		Vector3 dirIn = Normalize(curr - prev);
		Vector3 dirOut = Normalize(next - curr);

		Vector3 p0 = curr - dirIn * radius;
		Vector3 p2 = curr + dirOut * radius;

		return { p0, curr, p2 };
	}

	Vector3 EvaluateBezier(const BezierCurve& b, float t)
	{
		float u = 1.0f - t;
		return
			u * u * b.p0 +
			2 * u * t * b.p1 +
			t * t * b.p2;
	}

	void rCreateArcLengthTable(int samplePerSegment)
	{
		arcLengthTable_.lengths.clear();
		arcLengthTable_.totalLength = 0.0f;

		for (auto& seg : segments_)
		{
			float length = 0.0f;
			Vector3 prev;

			for (int i = 0; i <= samplePerSegment; ++i)
			{
				float t = static_cast<float>(i) / samplePerSegment;

				Vector3 pos =
					(seg.type == SegmentType::Line)
					? Lerp(seg.start, seg.end, t)
					: EvaluateBezier(seg.bezier, t);

				if (i > 0)
				{
					length += Length(pos - prev);
				}

				prev = pos;
			}

			seg.length = length;
			arcLengthTable_.lengths.push_back(length);
			arcLengthTable_.totalLength += length;
		}
	}

	Vector3 GetPositionByProgress(float progress)
	{
		return {};
	}

	Vector3 LookRotation(const Vector3& forward)
	{
		Vector3 f = Normalize(forward);

		// Y軸回転（Yaw）
		float yaw = std::atan2(f.x, f.z);

		// X軸回転（Pitch）
		float pitch = std::atan2(-f.y,
			std::sqrt(f.x * f.x + f.z * f.z));

		// Roll は使わない（0）
		float roll = 0.0f;

		return { pitch, yaw, roll };
	}

private: // メンバ変数

	std::vector<RailSegment> segments_;

	std::vector<Vector3> controlPoints_; //!< コントロールポイントのリスト

	ArcLengthTable arcLengthTable_; //!< 弧長データ

	std::unique_ptr<WorldTransform> worldTransform_ = nullptr; //!< ワールドトランスフォーム

	float lookAheadDistance_ = 0.1f;
};

