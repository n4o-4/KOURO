#pragma once
#include <random>

#include "BaseEffect.h"



// \brief Random  
// 画面全体にランダムなノイズや揺らぎ効果を適用するポストエフェクトクラス。  
// ノイズ演出やダメージ表現など、映像にランダム性を加える際に使用される。

namespace Kouro
{
	namespace RandomShader
	{
		struct Material
		{
			float time;
			float padding[3];
		};
	}

	// ==================================================================================

	class Random : public BaseEffect
	{
	public:

		/**
		* \brief	初期化
		* \param	dxCommon DirectXCommonのポインタ
		* \param	srvManager SrvManagerのポインタ
		*/
		void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

		/// \brief  更新
		void Update() override;

		/**
		* \brief  描画
		* \param  renderTargetIndex レンダーターゲットのインデックス
		* \param  renderResourceIndex レンダーリソースのインデックス
		*/
		void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

		/// \brief  リセット
		void Reset() override { resource_.Reset(); }

	private: // 非公開メンバ関数

		/// \brief  CreatePipeline パイプラインの作成
		void CreatePipeline();

		/**
		* \brief  CreateRootSignature ルートシグネチャの作成
		* \param  pipeline パイプライン構造体のポインタ
		*/
		void CreateRootSignature(Pipeline* pipeline);

		/**
		* \brief  CreatePipeLineState パイプラインステートの作成
		* \param  pipeline パイプライン構造体のポインタ
		*/
		void CreatePipeLineState(Pipeline* pipeline);

		/// \brief  CreateMaterial マテリアルの作成
		void CreateMaterial();

		/// \brief  DrawImGui ImGui描画
		void DrawImGui() override;

	private: // メンバ変数

		// 
		Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

		// 
		RandomShader::Material* data_ = nullptr;

		std::mt19937 randomEngine;
	};
}


