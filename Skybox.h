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

class Skybox
{
public:

	struct Direction
	{
		Vector3 direction; //!< 方向ベクトル
		float padding[1]; //!< 強度
	};

public: // 公開メンバ関数
	
	void Initialize(DirectXCommon* dxCommon, const std::string& filePath);

	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

private:

	void CreateRootSignature();

	void CreateGraphicsPipeline();

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

