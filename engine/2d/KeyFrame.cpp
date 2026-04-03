#include "KeyFrame.h"
#include <cmath>

namespace Kouro
{
	// ============================================================================
	// ScalarInterpolation 実装
	// ============================================================================

	float ScalarInterpolation::Interpolate(float start, float end, float t, InterpolationType type)
	{
		// 補間パラメータをクランプ（0-1）
		t = std::max(0.0f, std::min(1.0f, t));

		float easedT = t;
		switch (type)
		{
		case InterpolationType::Linear:
			easedT = LinearInterp(t);
			break;
		case InterpolationType::EaseInQuad:
			easedT = EaseInQuadInterp(t);
			break;
		case InterpolationType::EaseOutQuad:
			easedT = EaseOutQuadInterp(t);
			break;
		case InterpolationType::EaseInOutQuad:
			easedT = EaseInOutQuadInterp(t);
			break;
		}

		return start + (end - start) * easedT;
	}

	float ScalarInterpolation::LinearInterp(float t)
	{
		return t;
	}

	float ScalarInterpolation::EaseInQuadInterp(float t)
	{
		return t * t;
	}

	float ScalarInterpolation::EaseOutQuadInterp(float t)
	{
		return t * (2.0f - t);
	}

	float ScalarInterpolation::EaseInOutQuadInterp(float t)
	{
		if (t < 0.5f)
		{
			return 2.0f * t * t;
		}
		else
		{
			return -1.0f + (4.0f - 2.0f * t) * t;
		}
	}

	// ============================================================================
	// Vector2Interpolation 実装
	// ============================================================================

	Vector2 Vector2Interpolation::Interpolate(const Vector2& start, const Vector2& end, float t, InterpolationType type)
	{
		Vector2 result;
		result.x = ScalarInterpolation::Interpolate(start.x, end.x, t, type);
		result.y = ScalarInterpolation::Interpolate(start.y, end.y, t, type);
		return result;
	}

	// ============================================================================
	// Vector4Interpolation 実装
	// ============================================================================

	Vector4 Vector4Interpolation::Interpolate(const Vector4& start, const Vector4& end, float t, InterpolationType type)
	{
		Vector4 result;
		result.x = ScalarInterpolation::Interpolate(start.x, end.x, t, type);
		result.y = ScalarInterpolation::Interpolate(start.y, end.y, t, type);
		result.z = ScalarInterpolation::Interpolate(start.z, end.z, t, type);
		result.w = ScalarInterpolation::Interpolate(start.w, end.w, t, type);
		return result;
	}

} // namespace Kouro
