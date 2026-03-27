#pragma once
#include "BaseRenderer.h"

#include "SpriteTypes.h"

namespace Kouro
{
	/// \brief SpriteRenderer スプライト描画クラス
	class SpriteRenderer : public BaseRenderer
	{
	public: // 公開メンバ関数

		/// \brief 初期化
		void Initialize(ID3D12GraphicsCommandList* cmdList, const GpuResourceUtils* gpuResourceUtils) override;

		/**
		* \brief  描画
		* \param  scene : 描画するシーン
		*/
		void Render(BaseScene* scene) override;

	private: // 非公開メンバ関数
		
		/// \brief 頂点バッファを作成する関数
		void CreateVertexBuffer();

		/// \brief インデックスバッファを作成する関数
		void CreateIndexBuffer();

	private: // 非公開メンバ関数

		ID3D12GraphicsCommandList* cmdList_ = nullptr; //!< コマンドリストへのポインタ

		const GpuResourceUtils* gpuResourceUtils_ = nullptr; //!< GpuResourceUtilsへのポインタ

		static constexpr uint32_t kVertexCount_ = 4; //!< スプライトの頂点数
		static constexpr uint32_t kIndexCount_ = 6;  //!< スプライトのインデックス数

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr; //!< 頂点バッファリソース
		Sprite::VertexData* vertexData_ = nullptr; //!< 頂点バッファリソース内の頂点データを指すポインタ
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {}; //!< 頂点バッファリソースの使い道を補足するバッファビュー

		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr; //!< インデックスバッファリソース
		uint32_t* indexData_ = nullptr; //!< インデックスバッファリソース内のインデックスデータを指すポインタ
		D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {}; //!< インデックスバッファリソースの使い道を補足するバッファビュー
	};
}


