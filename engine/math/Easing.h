#pragma once

namespace Kouro
{
	namespace Easing
	{
		float Lerp(float start, float end, float t)
		{
			return start + (end - start) * t;
		}

		float EaseInQuad(float t)
		{
			return t * t;
		}

		float EaseInOutQuad(float t)
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

		float EaseOutQuad(float t)
		{
			return t * (2 - t);
		}
	}
}