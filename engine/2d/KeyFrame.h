#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include "../math/Vectors.h"
#include "../../externals/yaml-cpp/include/yaml-cpp/yaml.h"

namespace Kouro
{
	/// \brief キーフレームアニメーションシステム
	/// 
	/// このモジュールは、スプライトの各種パラメーター（位置、回転、スケール、透明度、色）を
	/// キーフレームで制御するアニメーションシステムを提供します。
	/// 
	/// 使用例：
	/// \code
	/// // 1. アニメーションを作成
	/// SpriteAnimation walkAnim("Walk");
	/// walkAnim.duration = 2.0f;
	/// 
	/// // 2. キーフレームを追加
	/// walkAnim.position->AddKeyframe(0.0f, Vector2(0, 0), InterpolationType::Linear);
	/// walkAnim.position->AddKeyframe(1.0f, Vector2(100, 0), InterpolationType::EaseOutQuad);
	/// walkAnim.position->AddKeyframe(2.0f, Vector2(200, 0), InterpolationType::Linear);
	/// 
	/// // 3. アニメーターに追加して再生
	/// SpriteAnimator animator;
	/// animator.AddAnimation(walkAnim);
	/// animator.PlayAnimation("Walk", true); // ループ再生
	/// 
	/// // 4. 毎フレーム更新
	/// animator.Update(deltaTime);
	/// animator.ApplyToSprite(mySprite);
	/// 
	/// // 5. YAMLで保存
	/// animator.SaveToYaml("Resources/animations/walk.yaml");
	/// \endcode

	/// \brief 補間タイプの列挙
	enum class InterpolationType
	{
		Linear,        // 線形補間
		EaseInQuad,    // イーズイン（2次）
		EaseOutQuad,   // イーズアウト（2次）
		EaseInOutQuad, // イーズイン・アウト（2次）
	};

	/// \brief 補間タイプから文字列への変換
	inline std::string InterpolationTypeToString(InterpolationType type)
	{
		switch (type)
		{
		case InterpolationType::Linear:
			return "Linear";
		case InterpolationType::EaseInQuad:
			return "EaseInQuad";
		case InterpolationType::EaseOutQuad:
			return "EaseOutQuad";
		case InterpolationType::EaseInOutQuad:
			return "EaseInOutQuad";
		default:
			return "Linear";
		}
	}

	/// \brief 文字列から補間タイプへの変換
	inline InterpolationType StringToInterpolationType(const std::string& str)
	{
		if (str == "EaseInQuad")
			return InterpolationType::EaseInQuad;
		if (str == "EaseOutQuad")
			return InterpolationType::EaseOutQuad;
		if (str == "EaseInOutQuad")
			return InterpolationType::EaseInOutQuad;
		return InterpolationType::Linear;
	}

	/// \brief 補間エンジン（スカラー値用）
	class ScalarInterpolation
	{
	public:
		/// \brief スカラー値を補間
		static float Interpolate(float start, float end, float t, InterpolationType type);

	private:
		static float LinearInterp(float t);
		static float EaseInQuadInterp(float t);
		static float EaseOutQuadInterp(float t);
		static float EaseInOutQuadInterp(float t);
	};

	/// \brief 補間エンジン（Vector2用）
	class Vector2Interpolation
	{
	public:
		/// \brief Vector2を補間
		static Vector2 Interpolate(const Vector2& start, const Vector2& end, float t, InterpolationType type);
	};

	/// \brief 補間エンジン（Vector4用）
	class Vector4Interpolation
	{
	public:
		/// \brief Vector4を補間
		static Vector4 Interpolate(const Vector4& start, const Vector4& end, float t, InterpolationType type);
	};

	/// \brief 単一キーフレーム（テンプレート）
	template <typename T>
	struct Keyframe
	{
		float time;                          // 時刻（秒）
		T value;                             // キーフレームの値
		InterpolationType interpolation;     // このキーフレーム→次への補間方法

		Keyframe() : time(0.0f), interpolation(InterpolationType::Linear) {}
		Keyframe(float t, const T& v, InterpolationType interp = InterpolationType::Linear)
			: time(t), value(v), interpolation(interp) {}
	};

	/// \brief アニメーション曲線（テンプレート）
	template <typename T>
	class AnimationCurve
	{
	public:
		/// \brief キーフレームを追加
		void AddKeyframe(float time, const T& value, InterpolationType interpolation = InterpolationType::Linear)
		{
			keyframes_.emplace_back(time, value, interpolation);
			// 時刻でソート
			std::sort(keyframes_.begin(), keyframes_.end(),
				[](const Keyframe<T>& a, const Keyframe<T>& b) { return a.time < b.time; });
		}

		/// \brief キーフレームを削除
		void RemoveKeyframe(size_t index)
		{
			if (index < keyframes_.size())
			{
				keyframes_.erase(keyframes_.begin() + index);
			}
		}

		/// \brief キーフレームを更新
		void SetKeyframe(size_t index, float time, const T& value, InterpolationType interpolation = InterpolationType::Linear)
		{
			if (index < keyframes_.size())
			{
				keyframes_[index].time = time;
				keyframes_[index].value = value;
				keyframes_[index].interpolation = interpolation;
				// 時刻でソート
				std::sort(keyframes_.begin(), keyframes_.end(),
					[](const Keyframe<T>& a, const Keyframe<T>& b) { return a.time < b.time; });
			}
		}

		/// \brief 指定時刻での値を計算（補間）
		T Evaluate(float time) const
		{
			if (keyframes_.empty())
			{
				return T(); // デフォルト値を返す
			}

			// 時刻が最初のキーフレームより前
			if (time <= keyframes_.front().time)
			{
				return keyframes_.front().value;
			}

			// 時刻が最後のキーフレームより後
			if (time >= keyframes_.back().time)
			{
				return keyframes_.back().value;
			}

			// 隣接するキーフレームを探索
			for (size_t i = 0; i < keyframes_.size() - 1; ++i)
			{
				const auto& frame0 = keyframes_[i];
				const auto& frame1 = keyframes_[i + 1];

				if (time >= frame0.time && time <= frame1.time)
				{
					// 補間パラメータ計算（0-1）
					float timeDiff = frame1.time - frame0.time;
					float t = (timeDiff > 0.0f) ? (time - frame0.time) / timeDiff : 0.0f;

					// 補間関数を適用
					return InterpolateValue(frame0.value, frame1.value, t, frame0.interpolation);
				}
			}

			return keyframes_.back().value;
		}

		/// \brief キーフレーム数を取得
		size_t GetKeyframeCount() const { return keyframes_.size(); }

		/// \brief キーフレームを取得
		const Keyframe<T>& GetKeyframe(size_t index) const { return keyframes_[index]; }

		/// \brief すべてのキーフレームを取得
		const std::vector<Keyframe<T>>& GetAllKeyframes() const { return keyframes_; }

		/// \brief クリア
		void Clear() { keyframes_.clear(); }

		/// \brief 総再生時間を取得
		float GetDuration() const
		{
			return keyframes_.empty() ? 0.0f : keyframes_.back().time;
		}

	protected:
		std::vector<Keyframe<T>> keyframes_;

		/// \brief 値を補間（仮想関数）
		virtual T InterpolateValue(const T& start, const T& end, float t, InterpolationType type) const = 0;
	};

	/// \brief スカラー値用アニメーション曲線
	class ScalarAnimationCurve : public AnimationCurve<float>
	{
	protected:
		float InterpolateValue(const float& start, const float& end, float t, InterpolationType type) const override
		{
			return ScalarInterpolation::Interpolate(start, end, t, type);
		}
	};

	/// \brief Vector2用アニメーション曲線
	class Vector2AnimationCurve : public AnimationCurve<Vector2>
	{
	protected:
		Vector2 InterpolateValue(const Vector2& start, const Vector2& end, float t, InterpolationType type) const override
		{
			return Vector2Interpolation::Interpolate(start, end, t, type);
		}
	};

	/// \brief Vector4用アニメーション曲線
	class Vector4AnimationCurve : public AnimationCurve<Vector4>
	{
	protected:
		Vector4 InterpolateValue(const Vector4& start, const Vector4& end, float t, InterpolationType type) const override
		{
			return Vector4Interpolation::Interpolate(start, end, t, type);
		}
	};

} // namespace Kouro
