#pragma once
#include "BaseEffect.h"

// \brief Vignette  
// 画面の周辺を暗くぼかして中央を強調するビネット効果を適用するポストエフェクトクラス。  
// 映像の雰囲気づくりや視線誘導に使用される。

namespace Kouro
{
	class Vignette : public BaseEffect
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

		/// \brief  DrawImGui ImGui描画
		void DrawImGui() override;
	};
}