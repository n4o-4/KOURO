#pragma once
namespace Kouro
{
	namespace Easing
	{
		float Linear(float t);
		float Lerp(float start, float end, float t);
		float EaseInQuad(float t);
		float EaseOutQuad(float t);
		float EaseInOutQuad(float t);
		float EaseInCubic(float t);
		float EaseOutCubic(float t);
		float EaseInOutCubic(float t);
		float EaseInSine(float t);
		float EaseOutSine(float t);
		float EaseInOutSine(float t);

		/**
		* \brief EaseOutBack イージング関数（EaseOutBack）
		* \param t 補間値（0.0f～1.0f）
		* \param overshoot オーバーシュートの量（デフォルトは1.70158f）
		* \return 補間後の値
		*/
		float EaseOutBack(float t, float overshoot = 1.70158f);
	}
}