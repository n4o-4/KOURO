#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "engine/2d/KeyFrame.h"
#include "engine/2d/Sprite.h"
#include "externals/yaml-cpp/include/yaml-cpp/yaml.h"

namespace Kouro
{
	/// \brief スプライトアニメーション
	struct SpriteAnimation
	{
		std::string name;                                // アニメーション名
		float duration = 0.0f;                           // 総再生時間（秒）
		std::unique_ptr<Vector2AnimationCurve> position; // 位置アニメーション
		std::unique_ptr<ScalarAnimationCurve> rotation;  // 回転アニメーション
		std::unique_ptr<Vector2AnimationCurve> scale;    // スケールアニメーション
		std::unique_ptr<ScalarAnimationCurve> alpha;     // 透明度アニメーション
		std::unique_ptr<Vector4AnimationCurve> color;    // 色アニメーション

		SpriteAnimation();
		SpriteAnimation(const std::string& animName);
		~SpriteAnimation();

		// コピーコンストラクタと代入演算子
		SpriteAnimation(const SpriteAnimation& other);
		SpriteAnimation& operator=(const SpriteAnimation& other);

		// ムーブセマンティクス
		SpriteAnimation(SpriteAnimation&& other) noexcept;
		SpriteAnimation& operator=(SpriteAnimation&& other) noexcept;
	};

	/// \brief スプライトアニメーションの再生状態
	struct AnimationPlayState
	{
		SpriteAnimation animation;
		float currentTime = 0.0f;  // 現在の再生時刻
		bool isPlaying = false;    // 再生中フラグ
		bool isLooping = false;    // ループフラグ
	};

	/// \brief スプライトアニメーター
	/// スプライトに対するアニメーション再生を管理するクラス
	/// 
	/// 使用方法：
	/// 1. SpriteAnimator インスタンスを作成
	/// 2. SpriteAnimation を定義してキーフレームを追加
	/// 3. AddAnimation() でアニメーターに追加
	/// 4. PlayAnimation() で再生開始
	/// 5. 毎フレーム Update() と ApplyToSprite() を呼び出し
	class SpriteAnimator
	{
	public:
		SpriteAnimator();
		~SpriteAnimator();

		/// \brief アニメーションを追加
		void AddAnimation(const SpriteAnimation& animation);

		/// \brief アニメーションを削除
		void RemoveAnimation(const std::string& name);

		/// \brief アニメーションを再生開始
		void PlayAnimation(const std::string& name, bool loop = false);

		/// \brief アニメーションを停止
		void StopAnimation();

		/// \brief アニメーションを一時停止
		void PauseAnimation();

		/// \brief アニメーションを再開
		void ResumeAnimation();

		/// \brief 更新（フレームごとに呼ぶ）
		void Update(float deltaTime);

		/// \brief 値をスプライトに適用
		void ApplyToSprite(Sprite& sprite);

		/// \brief 再生中かどうか
		bool IsPlaying() const { return playState_.isPlaying; }

		/// \brief 現在の再生時刻を取得
		float GetCurrentTime() const { return playState_.currentTime; }

		/// \brief 現在再生中のアニメーション名を取得
		const std::string& GetCurrentAnimationName() const;

		/// \brief すべてのアニメーション名を取得
		std::vector<std::string> GetAnimationNames() const;

		/// \brief YAML形式で保存
		void SaveToYaml(const std::string& filePath) const;

		/// \brief YAML形式から読み込み
		void LoadFromYaml(const std::string& filePath);

	private:
		std::unordered_map<std::string, SpriteAnimation> animations_; // 保存されたアニメーション
		AnimationPlayState playState_;                                 // 現在の再生状態

		/// \brief YAML::Emitterにキーフレーム曲線を保存（Vector2用）
		void SaveCurveToYaml(YAML::Emitter& out, const std::string& curveName,
							 const Vector2AnimationCurve& curve) const;

		/// \brief YAML::Emitterにキーフレーム曲線を保存（Scalar用）
		void SaveCurveToYaml(YAML::Emitter& out, const std::string& curveName,
							 const ScalarAnimationCurve& curve) const;

		/// \brief YAML::Emitterにキーフレーム曲線を保存（Vector4用）
		void SaveCurveToYaml(YAML::Emitter& out, const std::string& curveName,
							 const Vector4AnimationCurve& curve) const;

		/// \brief YAMLノードからキーフレーム曲線を読み込み（Vector2用）
		void LoadCurveFromYaml(const YAML::Node& node, Vector2AnimationCurve& curve);

		/// \brief YAMLノードからキーフレーム曲線を読み込み（Scalar用）
		void LoadCurveFromYaml(const YAML::Node& node, ScalarAnimationCurve& curve);

		/// \brief YAMLノードからキーフレーム曲線を読み込み（Vector4用）
		void LoadCurveFromYaml(const YAML::Node& node, Vector4AnimationCurve& curve);
	};

} // namespace Kouro