#pragma once
#include <numbers>
#include <algorithm>

namespace Kouro
{
	namespace Easing
	{
		static float Linear(float t)
		{
			return t;
		}

		static float Lerp(float start, float end, float t)
		{
			return start + (end - start) * t;
		}

		static float EaseInQuad(float t)
		{
			return t * t;
		}

		static float EaseOutQuad(float t)
		{
			return t * (2 - t);
		}

		static float EaseInOutQuad(float t)
		{
			// 符号を記憶して、絶対値で補間計算
			float sign = (t >= 0.0f) ? 1.0f : -1.0f;
			float input = std::abs(t);

			if (input < 0.5f)
			{
				input = 2.0f * input * input;
			}
			else
			{
				input = -1.0f + (4.0f - 2.0f * input) * input;
			}

			return input * sign;
		}

		static float EaseInCubic(float t)
		{
			return t * t * t;
		}

		static float EaseOutCubic(float t)
		{
			return 1.0f - std::pow(1.0f - t, 3);
		}

		static float EaseInOutCubic(float t)
		{
			if (t < 0.5f)
			{
				return 4.0f * t * t * t;
			}
			else
			{
				return 1.0f - std::pow(-2.0f * t + 2.0f, 3) / 2.0f;
			}
		}

		static float EaseInSine(float t)
		{
			return 1.0f - std::cos((t * static_cast<float>(std::numbers::pi)) / 2.0f);
		}

		static float EaseOutSine(float t)
		{
			return std::sin((t * static_cast<float>(std::numbers::pi)) / 2.0f);
		}

		static float EaseInOutSine(float t)
		{
			return -(std::cos(static_cast<float>(std::numbers::pi) * t) - 1.0f) / 2.0f;
		}
	}
}