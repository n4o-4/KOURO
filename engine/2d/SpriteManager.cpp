#include "SpriteManager.h"
#include <filesystem>

#include "YamlLoader.h"
#include "Vector2Yaml.h"
#include "Vector3Yaml.h"
#include "Vector4Yaml.h"

void Kouro::SpriteManager::LoadSpriteGroupsFromYaml(const std::string& yamlFilePath)
{
    // YAMLファイルの読み込み
    YAML::Node config = KOURO::YamlLoader::LoadYamlFile(yamlFilePath);

    // スプライトグループの読み込み
    for (const auto& groupNode : config)
    {
        // グループ名の取得
        std::string groupName = groupNode["group_name"].as<std::string>();

        // スプライトグループ生成
        SpriteGroup spriteGroup;

        // スプライトの読み込み
        for (const auto& spriteNode : groupNode["sprites"])
        {
            // --- 必須項目 ---
            std::string texturePath = spriteNode["texture_path"].as<std::string>();

            // --- 名前（キー）を取得 ---
            std::string spriteName;
            if (spriteNode["name"]) {
                spriteName = spriteNode["name"].as<std::string>();
            }
            else
            {
                // YAMLにnameが無い場合はファイル名を仮のキーにする
                spriteName = std::filesystem::path(texturePath).stem().string();
            }

            // スプライト生成・初期化
            auto sprite = std::make_unique<Sprite>();
            sprite->Initialize(SpriteCommon::GetInstance(), texturePath);

            // --- 各項目が存在すれば設定 ---
            if (spriteNode["position"]) {
                sprite->SetPosition(spriteNode["position"].as<Vector2>());
            }
            if (spriteNode["size"]) {
                sprite->SetSize(spriteNode["size"].as<Vector2>());
            }
            if (spriteNode["anchor_point"]) {
                sprite->SetAnchorPoint(spriteNode["anchor_point"].as<Vector2>());
            }
            if (spriteNode["texture_size"]) {
                sprite->SetTexSize(spriteNode["texture_size"].as<Vector2>());
            }
            if (spriteNode["color"]) {
                sprite->SetColor(spriteNode["color"].as<Vector4>());
            }

            // 更新（GPUバッファへ反映）
            sprite->Update();

            // --- 名前で登録 ---
            // 重複キーがある場合は上書きを警告できるようにしておくと安全
#ifdef _DEBUG
            if (spriteGroup.sprites.contains(spriteName)) {
                printf("Warning: Duplicate sprite name '%s' in group '%s'\n", spriteName.c_str(), groupName.c_str());
            }
#endif

            // tupleにまとめる: second要素はまだnullptr
            spriteGroup.sprites[spriteName] = std::make_tuple(std::move(sprite), nullptr);
        }

        // グループをマップに登録
        spriteGroups_[groupName] = std::move(spriteGroup);
    }
}

void Kouro::SpriteManager::UpdateVisibleGroups()
{
    for (auto& [groupName, spriteGroup] : spriteGroups_)
    {
        if (!spriteGroup.isVisible) continue; // 非表示グループは先にスキップ

        for (auto& [spriteName, spriteTuple] : spriteGroup.sprites)
        {
            auto& [spritePtr, updateFunc] = spriteTuple;
            if (!spritePtr) continue;

            if (updateFunc) updateFunc(*spritePtr);
            spritePtr->Update();
        }
    }
}

void Kouro::SpriteManager::DrawGroup(const std::string& groupName)
{
	// 指定されたグループ名のスプライトグループを描画
	auto it = spriteGroups_.find(groupName);
	if (it != spriteGroups_.end())
	{
		SpriteGroup& spriteGroup = it->second;
		if (spriteGroup.isVisible)
		{
            // グループ内の全スプライトを描画
            for (auto& [spriteName, spriteTuple] : spriteGroup.sprites)
            {
                auto& [spritePtr, updateFunc] = spriteTuple;
                if (!spritePtr) continue;

                spritePtr->Draw();
            }
		}
	}
}

void Kouro::SpriteManager::SetGroupVisibility(const std::string& groupName, bool isVisible)
{
	// 指定されたグループ名の表示状態を設定
	auto it = spriteGroups_.find(groupName);
	if (it != spriteGroups_.end())
	{
		it->second.isVisible = isVisible;
	}
}
