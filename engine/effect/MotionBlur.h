#pragma once
#include "BaseEffect.h"



// \brief MotionBlur  
// カメラやオブジェクトの移動量に応じて残像を表現するモーションブラー効果クラス。  
// フレーム間の位置差分を利用し、動きの滑らかさやスピード感を演出する。

namespace Kouro
{
	namespace MotionBlurShader
	{
		struct Material
		{
			int numSamples;
			Vector2 center;
			float blurWidth;

			Vector3 diff;
			float padding[1];
		};
	}

	// ====================== MotionBlur ====================================================

	class MotionBlur : public BaseEffect
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

		CameraManager* cameraManager_ = nullptr;

		// 
		Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

		// 
		MotionBlurShader::Material* data_ = nullptr;

		Vector3 currentPos_{};

		Vector3 prePos_{};

		Vector3 diffPos_{};
	};
}