#pragma once
#include <map>

#include "LineModel.h"

// \brief LineModelManager
// ラインモデルを管理するクラス。
// モデルの読み込み、検索、および内部保持を行う。
// 内部でstd::mapを用いてファイルパスとLineModelの対応を管理。

namespace Kouro
{
	class LineModelManager
	{
	public:

		/**
		* \brief  初期化
		* \param  lineDrawerBase : LineDrawerBaseのポインタ
		*/
		void Initialize(ID3D12GraphicsCommandList* commandList, const GpuResourceUtils* gpuResourceUtils);

		/**
		* \brief  ラインモデルの読み込み
		* \param  filePath : ラインモデルのファイルパス
		*/
		void LoadLineModel(std::string filePath);

		/**
		* \brief  ラインモデルの検索
		* \param  filePath : ラインモデルのファイルパス
		*/
		LineModel* FindLineModel(const std::string filePath);
	private:

		ID3D12GraphicsCommandList* cmdList_ = nullptr; //!< コマンドリストのポインタ

		const GpuResourceUtils* gpuResourceUtils_ = nullptr; //!< GpuResourceUtilsのポインタ

		std::map<std::string, std::unique_ptr<LineModel>> models_; //!< ファイルパスとLineModelの対応を管理するマップコンテナ

	};
}