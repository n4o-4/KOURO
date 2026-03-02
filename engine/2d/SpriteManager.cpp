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
        SpriteContext::SpriteGroup spriteGroup;

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
            sprite->Initialize(gpuContext_->gpuResourceUtils, texturePath);

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
            // 重複キーがある場合は上書きを警告
#ifdef _DEBUG
            if (spriteGroup.sprites.contains(spriteName)) {
                printf("Warning: Duplicate sprite name '%s' in group '%s'\n", spriteName.c_str(), groupName.c_str());
            }
#endif

            // tupleにまとめる: second要素はまだnullptr
            spriteGroup.sprites[spriteName] = std::make_tuple(std::move(sprite), nullptr);

            std::string layer = spriteNode["render_layer"].as<std::string>();

            if (layer == "foreground")
            {
				foregroundGroups_[groupName] = std::move(spriteGroup);
            }
            else if (layer == "background")
            {
				backgroundGroups_[groupName] = std::move(spriteGroup);
            }
            else
            {
            }
        }
    }
}

void Kouro::SpriteManager::UpdateVisibleGroups()
{
	// 前景グループと背景グループの両方を更新
    for (auto& [groupName, spriteGroup] : foregroundGroups_)
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

    for (auto& [groupName, spriteGroup] : backgroundGroups_)
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

void Kouro::SpriteManager::SetGroupVisibility(const std::string& groupName, bool isVisible)
{
	// 指定されたグループ名の表示状態を設定
	auto it = foregroundGroups_.find(groupName);
	if (it != foregroundGroups_.end())
	{
		it->second.isVisible = isVisible;
	}

	it = backgroundGroups_.find(groupName);
    if (it != backgroundGroups_.end())
    {
        it->second.isVisible = isVisible;
	}
}

void Kouro::SpriteManager::CreateVertexData()
{
    auto utils = gpuContext_->gpuResourceUtils;

    vertexResource_ = utils->CreateBufferResource(sizeof(SpriteContext::VertexData) * 4);

    // リソースの先頭アドレスから使う
    vertexBufferView.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();

    // 使用するリソースのサイズは頂点6つ分のサイズ
    vertexBufferView.SizeInBytes = sizeof(SpriteContext::VertexData) * 4;

    // 1頂点当たりのサイズ
    vertexBufferView.StrideInBytes = sizeof(SpriteContext::VertexData);

    vertexData = nullptr;

    vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Kouro::SpriteManager::CreateIndexData()
{
    auto utils = gpuContext_->gpuResourceUtils;

    indexResource_ = utils->CreateBufferResource(sizeof(uint32_t) * 6);

    // リソースの先頭アドレスから使う
    indexBufferView.BufferLocation = indexResource_.Get()->GetGPUVirtualAddress();

    // 使用するリソースのサイズはインデックス6つ分のサイズ
    indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;

    // インデックスはuint32_tとする
    indexBufferView.Format = DXGI_FORMAT_R32_UINT;

    // インデックスリソースにデータを書き込む
    indexData = nullptr;

    indexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
}
