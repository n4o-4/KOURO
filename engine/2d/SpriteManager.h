#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "Sprite.h"

namespace Kouro
{
	class SpriteManager
	{
	public:

		struct SpriteGroup
		{
			std::unordered_map < std::string, std::tuple<std::unique_ptr<Kouro::Sprite>, std::function<void(Kouro::Sprite&)>>> sprites; //!< スプライトのリスト
			bool isVisible = false;                               //!< グループの表示フラグ
		};

	public:
		 
		/**
		* \brief  YAMLファイルからスプライトグループを読み込む
		* \param  yamlFilePath YAMLファイルのパス
		*/
		void LoadSpriteGroupsFromYaml(const std::string& yamlFilePath);

		/// \brief 表示中の全グループのスプライトを更新する
		void UpdateVisibleGroups();

		/**
		* \brief  指定したグループのスプライトを描画する
		* \param  groupName グループ名
		*/
		void DrawGroup(const std::string& groupName);

		/**
		* \brief  指定したグループの表示状態を設定する
		* \param  groupName グループ名
		* \param  isVisible 表示状態
		*/
		void SetGroupVisibility(const std::string& groupName, bool isVisible);

		void SetSpriteUpdateFunction(const std::string& groupName, const std::string& spriteName, std::function<void(Kouro::Sprite&)> updateFunc)
		{
			auto groupIt = spriteGroups_.find(groupName);
			if (groupIt != spriteGroups_.end())
			{
				auto& spriteGroup = groupIt->second;
				auto spriteIt = spriteGroup.sprites.find(spriteName);
				if (spriteIt != spriteGroup.sprites.end())
				{
					auto& spriteTuple = spriteIt->second;
					std::get<1>(spriteTuple) = updateFunc;
				}
			}
		}

	private:

		std::unordered_map<std::string, SpriteGroup> spriteGroups_; //!< スプライトグループのマップ

	};
}


