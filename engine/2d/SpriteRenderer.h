#pragma once
#include "GpuContext.h"

// \brief SpriteCommon
// スプライト描画に必要な共通リソースやパイプラインを管理するクラス。
// ルートシグネチャやグラフィックスパイプライン（前景・背景）を生成・保持し、
// スプライト描画時に適切なパイプライン設定を提供。
// シングルトンとして管理され、DirectXCommonを介してDirectX12描画にアクセスする。

namespace Kouro
{
	class SpriteRenderer
	{
	public: // メンバ関数

		/**
		* \brief  初期化
		* \param  dxCommon DirectX共通クラスへのポインタ
		*/
		void Initialize(GpuContext& engineContex);

		/// \brief 前景描画
		void DrawForeground();

		/// \brief 背景描画
		void DrawBackground();

	private:

		/// \brief ルートシグネチャ作成
		void CreateRootSignature();

		/// \brief グラフィックスパイプライン作成（前景）
		void CreateGraphicsPipelineForeground();

		/// \brief グラフィックスパイプライン作成（背景）
		void CreateGraphicsPipelineBackground();

	private:
		DirectXCommon* dxCommon_;

		Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBackground;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForeground;

		//ID3DBlob* signatureBlob = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
		//ID3DBlob* errorBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

		GpuContext* gpuContext_;
	};
}