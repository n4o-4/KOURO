#pragma once

#include "../../SpriteAnimator.h"
#include <string>
#include <memory>
#include <vector>

namespace Kouro
{
	/// \brief スプライトアニメーションエディタUI
	/// ImGuiを使用したビジュアルエディタ
	class SpriteAnimationEditor
	{
	public:
		SpriteAnimationEditor();
		~SpriteAnimationEditor();

		/// \brief エディタUIを表示
		void ShowEditor();

		/// \brief 現在のアニメーターを取得
		SpriteAnimator& GetAnimator() { return animator_; }

		/// \brief 現在のアニメーターを設定
		void SetAnimator(const SpriteAnimator& animator) { animator_ = animator; }

	private:
		SpriteAnimator animator_;
		std::string currentFilePath_;
		int selectedAnimationIndex_ = -1;
		int selectedKeyframeIndex_ = -1;
		std::string selectedCurveType_; // "position", "rotation", "scale", "alpha", "color"
		std::string newAnimationName_;
		bool showAnimationSelector_ = true;
		bool showCurveEditor_ = true;
		bool showKeyframeList_ = true;
		bool showFileManagement_ = true;
		bool showAnimationPreview_ = true;

		// UI表示関数
		void ShowAnimationSelector();
		void ShowCurveEditor();
		void ShowKeyframeList();
		void ShowFileManagement();
		void ShowAnimationPreview();

		// ユーティリティ関数
		std::vector<std::string> GetAnimationNames() const;
		std::string GetCurveTypeLabel(const std::string& type) const;
	};

} // namespace Kouro
