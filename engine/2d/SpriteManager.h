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
		* \brief  指定したグループ内のスプライトに更新関数を設定する
		* \param  groupName グループ名
		* \param  spriteName スプライト名
		* \param  updateFunc スプライトの更新関数
		*/
		void SetSpriteUpdateFunction(const std::string& groupName, const std::string& spriteName, std::function<void(Kouro::Sprite&)> updateFunc);

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


