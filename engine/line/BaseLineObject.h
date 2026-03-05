#pragma once

#include "GpuResourceUtils.h"
#include "WorldTransform.h"
#include "ViewProjection.h"





namespace Kouro
{
	namespace LineDrawer
	{
		struct VertexData // ライン描画用のVertexData
		{
			Vector4 position; // 頂点位置
			Vector4 color; // 頂点カラー
		};
	}

	// \brief BaseLineObject  
    // ライン描画オブジェクトの基底クラス。  
    // 頂点リソースやバッファビューを管理し、派生クラスで線形オブジェクトの描画処理を共通化する。

	class BaseLineObject // ラインオブジェクトの基底クラス
	{
	public: // 公開メンバ関数

		virtual ~BaseLineObject() = default;

		/**
		 * \brief  初期化
		 * \param  utils GpuResourceUtilsのポインタ
		 */
		virtual void Initialize(GpuResourceUtils* gpuResourceUtils) = 0;

		/// \brief  Update 更新
		virtual void Update() = 0;

		/**
		 * \brief  Draw 描画
		 * \param  viewProjection ビュープロジェクション行列
		 */
		virtual void Draw(const ViewProjection& viewProjection) = 0;

	protected:

		GpuResourceUtils* gpuResourceUtils_ = nullptr; //!< GpuResourceUtilsのポインタ

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr; //!< 頂点リソース

		LineDrawer::VertexData* vertexData_ = nullptr; //!< 頂点データのCPU側コピー

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_; //!< 頂点バッファビュー
	};
}