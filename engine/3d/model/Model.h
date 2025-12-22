#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include "ModelCommon.h"
#include <fstream>
//#include <string>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp\postprocess.h>
#include "WorldTransform.h"
#include "ViewProjection.h"

#include "ModelDatas.h"
// \brief Model
// 3Dモデルの読み込み・描画を管理するクラス。
// OBJやFBX等のモデルデータを読み込み、マテリアル情報を保持。
// DirectX12の頂点バッファリソースを生成し、ワールド変換に基づく描画をサポート。

class Model
{
public:

	/**
	* \brief  初期化
	* \param  modelCommon モデル共通情報クラスへのポインタ
	* \param  directoryPath モデルファイルのディレクトリパス
	* \param  filename モデルファイル名
	*/
	void Initialize(ModelCommon* modelCommon, const std::string& directoryPath, const std::string& filename);

	/**
	* \brief  描画
	* \param  worldTransform ワールド変換情報
	*/
	void Draw(WorldTransform worldTransform);

private:

	/**
	* \brief  マテリアルテンプレートファイルの読み込み
	* \param  directoryPath ディレクトリパス
	* \param  filename ファイル名
	* \return 読み込んだマテリアルデータ
	*/
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	/**
	* \brief  モデルファイルの読み込み
	* \param  directoryPath ディレクトリパス
	* \param  filename ファイル名
	* \return 読み込んだモデルデータ
	*/
	static ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

public:

	/**
	* \brief  ノード情報の読み込み
	* \param  node aiNodeのポインタ
	* \return 読み込んだノード情報
	*/
	static Node ReadNode(aiNode* node);

private:

private:
	ModelCommon* modelCommon_;

	ModelData modelData;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	/*Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

	Material* materialData = nullptr;*/
};

