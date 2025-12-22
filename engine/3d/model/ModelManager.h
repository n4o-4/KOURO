#pragma once
#include <map>
#include <string>
#include <memory>
#include "Model.h"
#include "ModelCommon.h"

// \brief ModelManager
// モデルを管理するシングルトンクラス。
// モデルの読み込み、取得、共有を行う。

class ModelManager
{
public:

	/**
	* \brief  インスタンス取得
	* \return インスタンス
	*/
	static ModelManager* GetInstance();

	/**
	* \brief  初期化
	* \param  dxCommon DirectX共通クラスへのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon);

	/// \brief 終了処理
	void Finalize();

	/**
	* \brief  モデルの読み込み
	* \param  filePath モデルファイルのパス
	*/
	void LoadModel(const std::string& filePath);

	/**
	* \brief  モデルの取得
	* \param  filePath モデルファイルのパス
	*/
	Model* FindModel(const std::string& filePath);

private:
	static std::unique_ptr<ModelManager> instance;

	friend std::unique_ptr<ModelManager> std::make_unique<ModelManager>();
	friend std::default_delete<ModelManager>;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;

private:
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models;

	std::unique_ptr<ModelCommon> modelCommon = nullptr;

};

