#include "SpriteAnimator.h"
#include <fstream>

namespace Kouro
{
	// ============================================================================
	// SpriteAnimation 実装
	// ============================================================================

	SpriteAnimation::SpriteAnimation()
		: name(""), duration(0.0f)
	{
		position = std::make_unique<Vector2AnimationCurve>();
		rotation = std::make_unique<ScalarAnimationCurve>();
		scale = std::make_unique<Vector2AnimationCurve>();
		alpha = std::make_unique<ScalarAnimationCurve>();
		color = std::make_unique<Vector4AnimationCurve>();
	}

	SpriteAnimation::SpriteAnimation(const std::string& animName)
		: name(animName), duration(0.0f)
	{
		position = std::make_unique<Vector2AnimationCurve>();
		rotation = std::make_unique<ScalarAnimationCurve>();
		scale = std::make_unique<Vector2AnimationCurve>();
		alpha = std::make_unique<ScalarAnimationCurve>();
		color = std::make_unique<Vector4AnimationCurve>();
	}

	SpriteAnimation::~SpriteAnimation() = default;

	SpriteAnimation::SpriteAnimation(const SpriteAnimation& other)
		: name(other.name), duration(other.duration)
	{
		position = std::make_unique<Vector2AnimationCurve>(*other.position);
		rotation = std::make_unique<ScalarAnimationCurve>(*other.rotation);
		scale = std::make_unique<Vector2AnimationCurve>(*other.scale);
		alpha = std::make_unique<ScalarAnimationCurve>(*other.alpha);
		color = std::make_unique<Vector4AnimationCurve>(*other.color);
	}

	SpriteAnimation& SpriteAnimation::operator=(const SpriteAnimation& other)
	{
		if (this != &other)
		{
			name = other.name;
			duration = other.duration;
			position = std::make_unique<Vector2AnimationCurve>(*other.position);
			rotation = std::make_unique<ScalarAnimationCurve>(*other.rotation);
			scale = std::make_unique<Vector2AnimationCurve>(*other.scale);
			alpha = std::make_unique<ScalarAnimationCurve>(*other.alpha);
			color = std::make_unique<Vector4AnimationCurve>(*other.color);
		}
		return *this;
	}

	SpriteAnimation::SpriteAnimation(SpriteAnimation&& other) noexcept
		: name(std::move(other.name)), duration(other.duration),
		  position(std::move(other.position)), rotation(std::move(other.rotation)),
		  scale(std::move(other.scale)), alpha(std::move(other.alpha)),
		  color(std::move(other.color))
	{
	}

	SpriteAnimation& SpriteAnimation::operator=(SpriteAnimation&& other) noexcept
	{
		if (this != &other)
		{
			name = std::move(other.name);
			duration = other.duration;
			position = std::move(other.position);
			rotation = std::move(other.rotation);
			scale = std::move(other.scale);
			alpha = std::move(other.alpha);
			color = std::move(other.color);
		}
		return *this;
	}

	// ============================================================================
	// SpriteAnimator 実装
	// ============================================================================

	SpriteAnimator::SpriteAnimator()
	{
		// playState_は自動初期化
	}

	SpriteAnimator::~SpriteAnimator() = default;

	void SpriteAnimator::PlayAnimation(const std::string& name, bool loop)
	{
		auto it = animations_.find(name);
		if (it != animations_.end())
		{
			playState_.animation = it->second;
			playState_.currentTime = 0.0f;
			playState_.isPlaying = true;
			playState_.isLooping = loop;
		}
	}

	void SpriteAnimator::StopAnimation()
	{
		playState_.isPlaying = false;
		playState_.currentTime = 0.0f;
	}

	void SpriteAnimator::PauseAnimation()
	{
		playState_.isPlaying = false;
	}

	void SpriteAnimator::ResumeAnimation()
	{
		playState_.isPlaying = true;
	}

	void SpriteAnimator::Update(float deltaTime)
	{
		if (!playState_.isPlaying)
			return;

		playState_.currentTime += deltaTime;

		// ループ処理
		if (playState_.isLooping)
		{
			if (playState_.currentTime > playState_.animation.duration)
			{
				playState_.currentTime = std::fmod(playState_.currentTime, playState_.animation.duration);
			}
		}
		else
		{
			// ループなし：最後までいったら停止
			if (playState_.currentTime >= playState_.animation.duration)
			{
				playState_.currentTime = playState_.animation.duration;
				playState_.isPlaying = false;
			}
		}
	}

	void SpriteAnimator::ApplyToSprite(Sprite& sprite)
	{
		if (!playState_.animation.position || !playState_.animation.rotation ||
			!playState_.animation.scale || !playState_.animation.alpha ||
			!playState_.animation.color)
		{
			return;
		}

		// 位置を適用
		Vector2 position = playState_.animation.position->Evaluate(playState_.currentTime);
		sprite.SetPosition(position);

		// 回転を適用
		float rotation = playState_.animation.rotation->Evaluate(playState_.currentTime);
		sprite.SetRotation(rotation);

		// スケールを適用
		Vector2 scale = playState_.animation.scale->Evaluate(playState_.currentTime);
		sprite.SetSize(scale);

		// 透明度と色を適用
		float alpha = playState_.animation.alpha->Evaluate(playState_.currentTime);
		Vector4 color = playState_.animation.color->Evaluate(playState_.currentTime);
		color.w = alpha; // アルファチャンネルをセット
		sprite.SetColor(color);
	}

	const std::string& SpriteAnimator::GetCurrentAnimationName() const
	{
		static const std::string empty = "";
		if (playState_.animation.position)
		{
			return playState_.animation.name;
		}
		return empty;
	}

	std::vector<std::string> SpriteAnimator::GetAnimationNames() const
	{
		std::vector<std::string> names;
		for (const auto& pair : animations_)
		{
			names.push_back(pair.first);
		}
		return names;
	}

	void SpriteAnimator::LoadFromYaml(const std::string& filePath)
	{
		YAML::Node config = YAML::LoadFile(filePath);

		if (!config["animations"])
			return;

		animations_.clear();

		for (const auto& animNode : config["animations"])
		{
			std::string name = animNode["name"].as<std::string>();
			float duration = animNode["duration"].as<float>();

			SpriteAnimation anim(name);
			anim.duration = duration;

			// キーフレーム曲線を読み込み
			if (animNode["position"])
				LoadCurveFromYaml(animNode["position"], *anim.position);
			if (animNode["rotation"])
				LoadCurveFromYaml(animNode["rotation"], *anim.rotation);
			if (animNode["scale"])
				LoadCurveFromYaml(animNode["scale"], *anim.scale);
			if (animNode["alpha"])
				LoadCurveFromYaml(animNode["alpha"], *anim.alpha);
			if (animNode["color"])
				LoadCurveFromYaml(animNode["color"], *anim.color);

			animations_[name] = anim;
		}
	}

	void SpriteAnimator::LoadCurveFromYaml(const YAML::Node& node, Vector2AnimationCurve& curve)
	{
		curve.Clear();

		if (!node["keyframes"])
			return;

		for (const auto& kfNode : node["keyframes"])
		{
			float time = kfNode["time"].as<float>();
			auto valueSeq = kfNode["value"];
			Vector2 value{valueSeq[0].as<float>(), valueSeq[1].as<float>()};
			std::string interpStr = kfNode["interpolation"].as<std::string>();
			InterpolationType interp = StringToInterpolationType(interpStr);

			curve.AddKeyframe(time, value, interp);
		}
	}

	void SpriteAnimator::LoadCurveFromYaml(const YAML::Node& node, ScalarAnimationCurve& curve)
	{
		curve.Clear();

		if (!node["keyframes"])
			return;

		for (const auto& kfNode : node["keyframes"])
		{
			float time = kfNode["time"].as<float>();
			float value = kfNode["value"].as<float>();
			std::string interpStr = kfNode["interpolation"].as<std::string>();
			InterpolationType interp = StringToInterpolationType(interpStr);

			curve.AddKeyframe(time, value, interp);
		}
	}

	void SpriteAnimator::LoadCurveFromYaml(const YAML::Node& node, Vector4AnimationCurve& curve)
	{
		curve.Clear();

		if (!node["keyframes"])
			return;

		for (const auto& kfNode : node["keyframes"])
		{
			float time = kfNode["time"].as<float>();
			auto valueSeq = kfNode["value"];
			Vector4 value{valueSeq[0].as<float>(), valueSeq[1].as<float>(),
						  valueSeq[2].as<float>(), valueSeq[3].as<float>()};
			std::string interpStr = kfNode["interpolation"].as<std::string>();
			InterpolationType interp = StringToInterpolationType(interpStr);

			curve.AddKeyframe(time, value, interp);
		}
	}

} // namespace Kouro
