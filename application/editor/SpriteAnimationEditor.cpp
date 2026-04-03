#include "SpriteAnimationEditor.h"
#include "externals/imgui/imgui.h"
#include <cmath>

namespace Kouro
{
	SpriteAnimationEditor::SpriteAnimationEditor()
		: currentFilePath_(""), selectedAnimationIndex_(-1), selectedKeyframeIndex_(-1)
	{
	}

	SpriteAnimationEditor::~SpriteAnimationEditor() = default;

	void SpriteAnimationEditor::ShowEditor()
	{
		ImGui::SetNextWindowSize(ImVec2(1200, 700), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Sprite Animation Editor", nullptr, ImGuiWindowFlags_NoMove))
		{
			ImGui::BeginTabBar("EditorTabs");

			// タブ1: アニメーション管理
			if (ImGui::BeginTabItem("Animations"))
			{
				ShowAnimationSelector();
				ImGui::EndTabItem();
			}

			// タブ2: キーフレーム編集
			if (ImGui::BeginTabItem("Keyframes"))
			{
				ShowCurveEditor();
				ImGui::Separator();
				ShowKeyframeList();
				ImGui::EndTabItem();
			}

			// タブ3: ファイル管理
			if (ImGui::BeginTabItem("File"))
			{
				ShowFileManagement();
				ImGui::EndTabItem();
			}

			// タブ4: プレビュー
			if (ImGui::BeginTabItem("Preview"))
			{
				ShowAnimationPreview();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();

			ImGui::End();
		}
	}

	void SpriteAnimationEditor::ShowAnimationSelector()
	{
		ImGui::Text("Animations");
		ImGui::Separator();

		// アニメーション一覧
		auto animNames = GetAnimationNames();
		if (ImGui::BeginListBox("##animations", ImVec2(-1, 200)))
		{
			for (int i = 0; i < animNames.size(); ++i)
			{
				bool isSelected = (selectedAnimationIndex_ == i);
				if (ImGui::Selectable(animNames[i].c_str(), isSelected))
				{
					selectedAnimationIndex_ = i;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}

		// アニメーション操作ボタン
		ImGui::Spacing();
		if (ImGui::Button("Add Animation", ImVec2(120, 0)))
		{
			ImGui::OpenPopup("AddAnimationPopup");
		}

		ImGui::SameLine();
		if (ImGui::Button("Remove", ImVec2(120, 0)))
		{
			if (selectedAnimationIndex_ >= 0 && selectedAnimationIndex_ < animNames.size())
			{
				animator_.RemoveAnimation(animNames[selectedAnimationIndex_]);
				selectedAnimationIndex_ = -1;
			}
		}

		// アニメーション追加ダイアログ
		if (ImGui::BeginPopupModal("AddAnimationPopup"))
		{
			static char animNameBuf[256] = "";
			ImGui::InputText("Animation Name", animNameBuf, sizeof(animNameBuf));

			if (ImGui::Button("Create"))
			{
				if (strlen(animNameBuf) > 0)
				{
					SpriteAnimation newAnim(animNameBuf);
					newAnim.duration = 2.0f; // デフォルト値
					animator_.AddAnimation(newAnim);
					animNameBuf[0] = '\0';
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				animNameBuf[0] = '\0';
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		// 選択中のアニメーション情報表示
		if (selectedAnimationIndex_ >= 0 && selectedAnimationIndex_ < animNames.size())
		{
			ImGui::Spacing();
			ImGui::Text("Animation: %s", animNames[selectedAnimationIndex_].c_str());

			// Durationを変更
			static float duration = 2.0f;
			ImGui::DragFloat("Duration (seconds)", &duration, 0.1f, 0.1f, 100.0f);
		}
	}

	void SpriteAnimationEditor::ShowCurveEditor()
	{
		ImGui::Text("Curve Editor");
		ImGui::Separator();

		auto animNames = GetAnimationNames();
		if (selectedAnimationIndex_ < 0 || selectedAnimationIndex_ >= animNames.size())
		{
			ImGui::Text("No animation selected");
			return;
		}

		// カーブタイプセレクタ
		const char* curveTypes[] = {"Position", "Rotation", "Scale", "Alpha", "Color"};
		const char* curveTypeValues[] = {"position", "rotation", "scale", "alpha", "color"};
		static int curveTypeIndex = 0;

		if (ImGui::Combo("Curve Type", &curveTypeIndex, curveTypes, IM_ARRAYSIZE(curveTypes)))
		{
			selectedCurveType_ = curveTypeValues[curveTypeIndex];
		}

		ImGui::Spacing();
		ImGui::Text("Keyframe Values");

		// 補間タイプセレクタ
		const char* interpTypes[] = {"Linear", "EaseInQuad", "EaseOutQuad", "EaseInOutQuad"};
		static int interpIndex = 0;
		ImGui::Combo("Interpolation", &interpIndex, interpTypes, IM_ARRAYSIZE(interpTypes));

		// キーフレーム値入力
		ImGui::Spacing();
		if (selectedCurveType_ == "position" || selectedCurveType_ == "scale")
		{
			static float values[2] = {0.0f, 0.0f};
			ImGui::DragFloat2("Value (X, Y)", values, 0.1f);

			if (ImGui::Button("Add Keyframe##vector2"))
			{
				static float timeVal = 0.0f;
				ImGui::OpenPopup("TimeInputPopup");
			}
		}
		else if (selectedCurveType_ == "rotation" || selectedCurveType_ == "alpha")
		{
			static float value = 0.0f;
			ImGui::DragFloat("Value", &value, 0.1f);

			if (ImGui::Button("Add Keyframe##scalar"))
			{
				ImGui::OpenPopup("TimeInputPopup");
			}
		}
		else if (selectedCurveType_ == "color")
		{
			static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
			ImGui::ColorEdit4("Color", color);

			if (ImGui::Button("Add Keyframe##vector4"))
			{
				ImGui::OpenPopup("TimeInputPopup");
			}
		}

		// タイム入力ダイアログ
		if (ImGui::BeginPopupModal("TimeInputPopup"))
		{
			static float timeVal = 0.0f;
			ImGui::DragFloat("Time (seconds)", &timeVal, 0.01f, 0.0f, 100.0f);

			if (ImGui::Button("OK"))
			{
				// キーフレーム追加処理
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel##time"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void SpriteAnimationEditor::ShowKeyframeList()
	{
		ImGui::Text("Keyframe List");
		ImGui::Separator();

		auto animNames = GetAnimationNames();
		if (selectedAnimationIndex_ < 0 || selectedAnimationIndex_ >= animNames.size())
		{
			ImGui::Text("No animation selected");
			return;
		}

		// キーフレーム一覧表示（テーブル形式）
		if (ImGui::BeginTable("KeyframesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Time");
			ImGui::TableSetupColumn("Value");
			ImGui::TableSetupColumn("Interpolation");
			ImGui::TableHeadersRow();

			// サンプルキーフレーム表示
			for (int i = 0; i < 3; ++i)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%.2f", i * 0.5f);

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("(%.2f, %.2f)", i * 10.0f, i * 5.0f);

				ImGui::TableSetColumnIndex(2);
				ImGui::Text("Linear");
			}

			ImGui::EndTable();
		}

		ImGui::Spacing();
		if (ImGui::Button("Remove Selected Keyframe"))
		{
			// 削除処理
		}
	}

	void SpriteAnimationEditor::ShowFileManagement()
	{
		ImGui::Text("File Management");
		ImGui::Separator();

		// std::stringをimguiで使用するための一時バッファ
		static char filePathBuffer[256] = "";
		
		// currentFilePath_の内容をバッファにコピー
		if (currentFilePath_.size() < sizeof(filePathBuffer))
		{
			strcpy_s(filePathBuffer, sizeof(filePathBuffer), currentFilePath_.c_str());
		}

		// ImGuiで入力受け取り
		if (ImGui::InputText("File Path", filePathBuffer, sizeof(filePathBuffer)))
		{
			currentFilePath_ = filePathBuffer;
		}

		if (ImGui::Button("Save Animation", ImVec2(150, 0)))
		{
			if (!currentFilePath_.empty())
			{
				animator_.SaveToYaml(currentFilePath_);
				ImGui::OpenPopup("SaveSuccessPopup");
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Load Animation", ImVec2(150, 0)))
		{
			if (!currentFilePath_.empty())
			{
				animator_.LoadFromYaml(currentFilePath_);
				ImGui::OpenPopup("LoadSuccessPopup");
			}
		}

		// 成功ダイアログ
		if (ImGui::BeginPopupModal("SaveSuccessPopup"))
		{
			ImGui::Text("Animation saved successfully!");
			if (ImGui::Button("OK"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("LoadSuccessPopup"))
		{
			ImGui::Text("Animation loaded successfully!");
			if (ImGui::Button("OK"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::Spacing();
		ImGui::Text("Recent Files:");
		ImGui::BulletText("animation_01.yaml");
		ImGui::BulletText("animation_02.yaml");
		ImGui::BulletText("animation_03.yaml");
	}

	void SpriteAnimationEditor::ShowAnimationPreview()
	{
		ImGui::Text("Animation Preview");
		ImGui::Separator();

		auto animNames = GetAnimationNames();
		if (selectedAnimationIndex_ < 0 || selectedAnimationIndex_ >= animNames.size())
		{
			ImGui::Text("No animation selected");
			return;
		}

		ImGui::Text("Playing: %s", animNames[selectedAnimationIndex_].c_str());

		static float playbackTime = 0.0f;
		static bool isPreviewPlaying = false;
		static float playbackSpeed = 1.0f;

		if (ImGui::Button("Play"))
		{
			isPreviewPlaying = true;
			playbackTime = 0.0f;
		}

		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			isPreviewPlaying = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			isPreviewPlaying = false;
			playbackTime = 0.0f;
		}

		ImGui::DragFloat("Playback Speed", &playbackSpeed, 0.1f, 0.1f, 5.0f);

		// プレビュー用タイムラインスライダー
		float duration = 2.0f; // アニメーション長を取得
		ImGui::SliderFloat("Time", &playbackTime, 0.0f, duration, "%.2f");

		// 簡易グラフ表示
		ImGui::PlotLines("Preview", nullptr, 0, 0, "", 0.0f, 1.0f, ImVec2(400, 100));
	}

	std::vector<std::string> SpriteAnimationEditor::GetAnimationNames() const
	{
		std::vector<std::string> names;
		// アニメーター内のすべてのアニメーション名を取得
		names = animator_.GetAnimationNames();
		return names;
	}

	std::string SpriteAnimationEditor::GetCurveTypeLabel(const std::string& type) const
	{
		if (type == "position") return "Position";
		if (type == "rotation") return "Rotation";
		if (type == "scale") return "Scale";
		if (type == "alpha") return "Alpha";
		if (type == "color") return "Color";
		return "Unknown";
	}

} // namespace Kouro
