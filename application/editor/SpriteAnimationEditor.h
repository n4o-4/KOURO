#pragma once

#include "../../SpriteAnimator.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace Kouro
{
	/// \brief スプライトアニメーションエディタUI
	/// ImGuiを使用したビジュアルエディタ

	class SpriteAnimationEditor
	{
	private:

		struct SpriteAnimation
		{
			float duration = 0.0f;                           // 総再生時間（秒）
			Vector2AnimationCurve position; // 位置アニメーション
			ScalarAnimationCurve rotation;  // 回転アニメーション
			Vector2AnimationCurve scale;    // スケールアニメーション
			ScalarAnimationCurve alpha;     // 透明度アニメーション
			Vector4AnimationCurve color;    // 色アニメーション
		};

		enum class EditorWindow
		{
			AnimationSelector,
			AnimationEditor
		};

	public:

		/// \brief コンストラクタ
		SpriteAnimationEditor();
		/// \brief デストラクタ
		~SpriteAnimationEditor();

		/// \brief エディタUIを表示
		void ShowEditor();

		/**
		* \brief YAMLファイルからアニメーションデータを読み込む
		* \param filePath 読み込むYAMLファイルのパス
		* \return 読み込んだアニメーションデータのマップ（アニメーション名をキーとする）
		*/
		//std::unordered_map<std::string, SpriteAnimation> LoadAnimationsFromYaml(const std::string& filePath) const;

	private:
		
		// UI表示関数
		void ShowAnimationSelector();
		
		// アニメーション管理関数
		void AddAnimation(const SpriteAnimation& animation);
		void RemoveAnimation(const std::string& name);
		void SaveAnimationsToYaml(const std::string& filePath) const;
		void LoadAnimationsFromYaml(const std::string& filePath);

	private:

		SpriteAnimator previewAnimator_; // プレビュー用アニメーター

		std::string fullFilePath_ = "";

		const std::string rootPath_ = "Resources/Animations/"; // アニメーションファイルのルートパス

		std::string editingFilePath_ = ""; // 現在編集中のファイルパス

		EditorWindow state_ = EditorWindow::AnimationSelector; // 現在のエディタウィンドウ状態

		std::unordered_map<std::string, SpriteAnimation> animations_; // 編集対象のアニメーションマップ

		std::string editingAnimationName_; // 現在編集中のアニメーション名

		SpriteAnimation editingAnimation_; // 現在編集中のアニメーションデータ
	};
}
