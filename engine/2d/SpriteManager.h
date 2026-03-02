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

		void CreateVertexData();

		void CreateIndexData();

	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr; //!< 頂点バッファリソース
		SpriteContext::VertexData* vertexData = nullptr; //!< 頂点データへのポインタ
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {}; //!< 頂点バッファビュー

		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr; //!< インデックスバッファリソース
		uint32_t* indexData = nullptr; //!< インデックスデータへのポインタ
		D3D12_INDEX_BUFFER_VIEW indexBufferView = {}; //!< インデックスバッファビュー

		SpriteContext::SpriteGroups foregroundGroups_; //!< 前景スプライトグループのマップ
		SpriteContext::SpriteGroups backgroundGroups_; //!< 背景スプライトグループのマップ

		GpuContext* gpuContext_; //!< GPUリソース管理クラスへのポインタ

	};
}


