#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "GpuContext.h"
#include "SpriteContext.h"

#include "Sprite.h"

namespace Kouro
{
	class SpriteManager
	{
	private:

		/**
		* \brief  スプライトエントリー
		* \param  sprite     : スプライトのポインタ
		* \param  updateFunc : スプライトの更新関数（引数にスプライトを受け取る関数オブジェクト）
		*/
		struct SpriteEntry
		{
			std::unique_ptr<Sprite> sprite;          //!< スプライトのポインタ
			std::function<void(Sprite&)> updateFunc; //!< スプライトの更新関数
		};

		/**
		* \brief  スプライトグループ
		* \param  sprites : スプライトのリスト（名前をキーとしたマップ）
		* \param  isVisible : グループの表示フラグ
		*/
		struct SpriteGroup
		{
			std::unordered_map < std::string, SpriteEntry> sprites; //!< スプライトのリスト
			bool isVisible = false;                                 //!< グループの表示フラグ
		};

		/**
		* \brief  グループ名をキーとしたスプライトグループのマップ
		* \param  string      : グループ名
		* \param  SpriteGroup : スプライトのリストと表示フラグを持つ構造体
		*/
		using SpriteGroups = std::unordered_map<std::string, SpriteGroup>;

	public:
		 
		SpriteManager(GpuContext& context) : gpuContext_(&context) {}

		/**
		* \brief  YAMLファイルからスプライトグループを読み込む
		* \param  yamlFilePath YAMLファイルのパス
		*/
		void LoadSpriteGroupsFromYaml(const std::string& yamlFilePath);

		/// \brief 表示中の全グループのスプライトを更新する
		void UpdateVisibleGroups();

		/**
		* \brief  指定したグループの表示状態を設定する
		* \param  groupName グループ名
		* \param  isVisible 表示状態
		*/
		void SetGroupVisibility(const std::string& groupName, bool isVisible);

	private:

		void CreateIndexData();

	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr; //!< インデックスバッファリソース
		uint32_t* indexData = nullptr; //!< インデックスデータへのポインタ
		D3D12_INDEX_BUFFER_VIEW indexBufferView = {}; //!< インデックスバッファビュー

		SpriteGroups foregroundGroups_; //!< 前景スプライトグループのマップ
		SpriteGroups backgroundGroups_; //!< 背景スプライトグループのマップ

		GpuContext* gpuContext_; //!< GPUリソース管理クラスへのポインタ

	};
}


