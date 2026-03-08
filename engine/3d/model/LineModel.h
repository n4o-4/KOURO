#pragma once

#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <wrl.h>
#include <dxcapi.h>
#include<unordered_map>

#include "Vectors.h"
#include "MyMath.h"
#include "GpuResourceUtils.h"



// \brief LineModel
// ライン描画用のモデルクラス。
// ファイルから頂点データを読み込み、ライン描画に必要なバッファを作成・管理する。
// 内部でDirectXの頂点リソースとバッファビューを保持し、Draw関数で描画を実行する。

namespace Kouro
{
	struct LineVertex
	{
		Vector4 position;
	};

	struct LineModelData {
		std::vector<LineVertex> vertices;
		std::vector<uint32_t> indices;
	};

	class LineModel
	{
	public:

		/**
		* \brief  初期化
		* \param  lienDrawer ラインドロワーベースクラスのポインタ
		* \param  directoryPath ラインモデルファイルのディレクトリパス
		* \param  filePath ラインモデルファイル名
		*/
		void Initialize(ID3D12GraphicsCommandList* commandList, const GpuResourceUtils* gpuResourceUtils, const std::string& filePath);


		/// \brief 描画
		void Draw();

	private:

		/**
		* \brief  ラインモデルファイルの読み込み
		* \param  directoryPath ディレクトリパス
		* \param  filePath ファイル名
		*/
		void LoadLineModelFile(const std::string& filePath);

	private:

		ID3D12GraphicsCommandList* cmdList_ = nullptr;
		const GpuResourceUtils* gpuResourceUtils_ = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
		LineVertex* vertexData_ = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

		const std::string rootDirectoryPath_ = "Resources/model/";

		uint32_t vertexCount_ = 0;

	};
}