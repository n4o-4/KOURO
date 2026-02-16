#pragma once

namespace Kouro
{
	namespace Easing
	{
		static float Lerp(float start, float end, float t)
		{
			return start + (end - start) * t;
		}

		static float EaseInQuad(float t)
		{
			return t * t;
		}

		static inline float EaseInOutQuad(float t)
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

	    static float EaseOutQuad(float t)
		{
			return t * (2 - t);
		}
	}
}