#pragma once]

#include <d3dx12.h>
#include <wrl.h>

#include "ShaderTypes.h"

// \brief SpriteCommon
// スプライト描画に必要な共通リソースやパイプラインを管理するクラス。
// ルートシグネチャやグラフィックスパイプライン（前景・背景）を生成・保持し、
// スプライト描画時に適切なパイプライン設定を提供。
// シングルトンとして管理され、DirectXCommonを介してDirectX12描画にアクセスする。

namespace Kouro
{
	class SpriteCommon
	{
	public: // メンバ関数

		/**
		* \brief  インスタンス取得
		* \return インスタンス
		*/
		static SpriteCommon* GetInstance();

		/**
		* \brief  初期化
		* \param  dxCommon DirectX共通クラスへのポインタ
		*/
		void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const Shader::GraphicsShader& shader);

		/// \brief 終了処理
		void Finalize();

		/// \brief 前景ビュー設定
		void SetForegroundView();

		/// \brief 背景ビュー設定
		void SetBackgroundView();

		/// \brief 前景描画
		void DrawForeground();

		/// \brief 背景描画
		void DrawBackground();

		/**
		* \brief  背景描画フラグ取得
		* \return 背景描画フラグ
		*/
		bool GetIsDrawBackground() const { return isDrawBackground; }

		/**
		* \brief  前景描画フラグ取得
		* \return 前景描画フラグ
		*/
		bool GetIsDrawForeground() const { return isDrawForeground; }

	private:

		static std::unique_ptr<SpriteCommon> instance;

		friend std::unique_ptr<SpriteCommon> std::make_unique<SpriteCommon>();
		friend std::default_delete<SpriteCommon>;

		SpriteCommon() = default;
		~SpriteCommon() = default;
		SpriteCommon(SpriteCommon&) = delete;
		SpriteCommon& operator=(SpriteCommon&) = delete;

		/// \brief ルートシグネチャ作成
		void CreateRootSignature();

		/// \brief グラフィックスパイプライン作成（前景）
		void CreateGraphicsPipelineForeground();

		/// \brief グラフィックスパイプライン作成（背景）
		void CreateGraphicsPipelineBackground();

	private:

		ID3D12Device* device_ = nullptr; //!< デバイスへのポインタ
		ID3D12GraphicsCommandList* cmdList_ = nullptr; //!< コマンドリストへのポインタ
		Shader::GraphicsShader shader_ = {}; //!< シェーダー

		Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature; //!< ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBackground; //!< グラフィックスパイプラインステート（背景）
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForeground; //!< グラフィックスパイプラインステート（前景）
		Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr; //!< ルートシグネチャのバイナリ
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr; //!< ルートシグネチャのエラー情報

		bool isDrawBackground = false;

		bool isDrawForeground = false;
	};
}