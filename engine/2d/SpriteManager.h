#pragma once
// Standard Library
#include <string>
#include <unordered_map>

// Engine
#include "SpriteGroup.h"

// Forward Declarations
namespace Kouro
{
	class Sprite;
}

namespace Kouro
{
	class SpriteManager
	{
	public:
		 
		/**
		* \brief  YAMLファイルからスプライトグループを読み込む
		* \param  YAMLFilePath YAMLファイルのパス
		*/
		void LoadSpriteGroupsFromYaml(const std::string& YAMLFilePath);

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

		/**
		* \brief  指定したグループ内のスプライトを取得する
		* \param  groupName グループ名
		* \param  spriteName スプライト名
		* \return スプライトのポインタ（見つからない場合はnullptr）
		*/
		Sprite* GetSprite(const std::string& groupName, const std::string& spriteName);

	private:

		std::unordered_map<std::string, SpriteGroup> spriteGroups_; //!< スプライトグループのマップ

	}; // class SpriteManager

} // namespace Kouro