#pragma once
#include "BaseEffect.h"



// \brief LuminanceBasedOutline  
// 輝度差をもとに物体の輪郭を抽出するポストエフェクトクラス。  
// 明暗の境界を強調し、セル画風やスケッチ風の表現を実現する。

namespace Kouro
{
	namespace LuminanceOutline
	{
		struct Material
		{
			float edgeStrength;
			float padding[3];
		};
	}

	// ================================== LuminanceBasedOutline ==================================

	class LuminanceBasedOutline : public BaseEffect
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

		// メモリリーク発生
		Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

		// 
		LuminanceOutline::Material* data_ = nullptr;

	};
}