#pragma once
namespace Kouro::SpriteAnimation
{
	enum class InterpolationType
	{
		Linear,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,
		EaseInCubic,
		EaseOutCubic,
		EaseInOutCubic
	};

	template <typename T>
	struct KeyFrame
	{
		float time; // キーフレームの時間
		T value;    // キーフレームの値
		InterpolationType interpolation; // 次のキーフレームへの補間方法
	};

	template <typename T>
	class AnimationCurve
	{
	private:

	};
}