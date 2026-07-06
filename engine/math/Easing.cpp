#include "Easing.h"

#include <cmath>
#include <numbers>

float Kouro::Easing::Linear(float t) { return t; }
float Kouro::Easing::Lerp(float start, float end, float t) { return start + (end - start) * t; }
float Kouro::Easing::EaseInQuad(float t) { return t * t; }
float Kouro::Easing::EaseOutQuad(float t) { return t * (2 - t); }

float Kouro::Easing::EaseInOutQuad(float t)
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

float Kouro::Easing::EaseInCubic(float t) { return t * t * t; }
float Kouro::Easing::EaseOutCubic(float t) { return 1.0f - std::pow(1.0f - t, 3); }

float Kouro::Easing::EaseInOutCubic(float t)
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

float Kouro::Easing::EaseInSine(float t) { return 1.0f - std::cos((t * static_cast<float>(std::numbers::pi)) / 2.0f); }
float Kouro::Easing::EaseOutSine(float t) { return std::sin((t * static_cast<float>(std::numbers::pi)) / 2.0f); }
float Kouro::Easing::EaseInOutSine(float t) { return -(std::cos(static_cast<float>(std::numbers::pi) * t) - 1.0f) / 2.0f; }

float Kouro::Easing::EaseOutBack(float t, float overshoot)
{
	const float c1 = overshoot;
	const float c3 = c1 + 1.0f;

	return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

