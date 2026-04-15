#include "SpriteAnimationEditor.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_stdlib.h"
#include <cmath>

namespace Kouro
{
	// コンストラクタ
	SpriteAnimationEditor::SpriteAnimationEditor()
		: editingFilePath_("")
	{
		// テスト用のアニメーションデータを追加（中身は空）
		animations_["test1"] = SpriteAnimation();
		animations_["test2"] = SpriteAnimation();

		// 初期のファイルパスをセット
		fullFilePath_ = rootPath_ + editingFilePath_;
	}

	// デストラクタ
	SpriteAnimationEditor::~SpriteAnimationEditor() = default;

	void SpriteAnimationEditor::ShowEditor()
	{
#ifdef _DEBUG
		ImGui::SetNextWindowSize(ImVec2(1200, 700), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Sprite Animation Editor", nullptr, ImGuiWindowFlags_NoMove))
		{
			if (ImGui::InputText("filePath", &editingFilePath_, 
				ImGuiInputTextFlags_EnterReturnsTrue) || 
				ImGui::IsItemDeactivatedAfterEdit())
			{
				// Enter押されたとき、または入力が完了したときにアニメーションをロード
				fullFilePath_ = rootPath_ + editingFilePath_;

				animations_.clear();
			}

			ImGui::Text("editingFilePath_: %s", fullFilePath_.c_str());

			// ファイルパス入力の下に線を引く
			ImGui::Separator();

			// 現在のエディタウィンドウ状態に応じてUIを表示
			switch (state_)
			{
				// アニメーションセレクタウィンドウを表示
				case EditorWindow::AnimationSelector:
					// アニメーションセレクタウィンドウを表示
					ShowAnimationSelector();
					break;

			    // アニメーションエディタウィンドウを表示
				case EditorWindow::AnimationEditor:
					break;	

				// デフォルトのウィンドウを表示
				default:
					break;
			}

			ImGui::End();
		}

#endif // _DEBUG
	}

	void SpriteAnimationEditor::ShowAnimationSelector()
	{
		ImGui::Text("Animations");

		// アニメーションマップをループして表示
		for (const auto& [name, animation] : animations_)
		{
			// アニメーション名を表示
			ImGui::Text("%s", name.c_str());

			ImGui::SameLine();

			// Edit押されたときの処理
			if (ImGui::Button(("Edit##" + name).c_str()))
			{
				// 編集対象のアニメーション名をセット
				editingAnimationName_ = name;

				// アニメーションマップから編集対象のアニメーションを検索
				auto it = animations_.find(name);

				// アニメーションが見つかった場合、editingAnimation_にセット
				if (it != animations_.end())
				{
					// 編集対象のアニメーションデータをセット
					editingAnimation_ = it->second;
				}
			}
		}
	}

	void SpriteAnimationEditor::AddAnimation(const SpriteAnimation& animation)
	{
	}

	void SpriteAnimationEditor::RemoveAnimation(const std::string& name)
	{
	}

	void SpriteAnimationEditor::SaveAnimationsToYaml(const std::string& filePath) const
	{
	}

	void SpriteAnimationEditor::LoadAnimationsFromYaml(const std::string& filePath)
	{
	}
}
