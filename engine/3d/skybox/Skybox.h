#pragma once

#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include "ModelCommon.h"
#include <fstream>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ModelDatas.h"

// \brief Skybox
// 背景として描画される立方体スカイボックスを管理するクラス。
// DirectX12を使用して、頂点・インデックス・マテリアルリソースを生成し描画。

namespace Kouro
{
	class Skybox
	{
	public:

		struct Direction
		{
			Vector3 direction; //!< 方向ベクトル
			float padding[1]; //!< 強度
		};

	public: // 公開メンバ関数

		/**
		* \brief  初期化
		* \param  dxCommon DirectX共通クラスへのポインタ
		* \param  filePath モデルファイルのパス
		*/
		void Initialize(DirectXCommon* dxCommon, const std::string& filePath);

		/**
		* \brief  描画
		* \param  viewProjection ビュープロジェクション情報
		* \param  directionalLight ディレクショナルライト情報
		* \param  pointLight ポイントライト情報
		* \param  spotLight スポットライト情報
		*/
		void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	private:

		/// \brief 頂点バッファの作成
		void CreateRootSignature();

		/// \brief グラフィックスパイプラインの作成
		void CreateGraphicsPipeline();

		/// \brief 頂点バッファの初期化
		void InitializeIndexBuffer();

	private:

		DirectXCommon* dxCommon_; //!< DirectX共通クラスへのポインタ

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_; //!< 頂点リソース

		VertexData* vertexData_; //!< 頂点データ


		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

		//ID3DBlob* signatureBlob = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
		//ID3DBlob* errorBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

		Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_; //!< マテリアルリソース

		Material* materialData_; //!< マテリアルデータ

		Microsoft::WRL::ComPtr<ID3D12Resource> directionResource_; //!< 頂点リソース

		Direction* directionData_; //!< 頂点データ

		std::unique_ptr<WorldTransform> worldTransform_; //!< ワールドトランスフォーム

		std::string filePath_; //!< ファイルパス

		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_; //!< インデックスリソース

		D3D12_INDEX_BUFFER_VIEW indexBufferView_; //!< インデックスバッファビュー

		int32_t* indexData_;
	};
}