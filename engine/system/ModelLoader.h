#pragma once
#include <string>
#include <iostream>
#include <vector>
#include<unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp\postprocess.h>

#include "MyMath.h"
#include "ModelDatas.h"

namespace Line
{
	struct Vertex
	{
		Vector4 position;
	};

	struct ModelData
	{
		std::vector<Line::Vertex> vertices;
		std::vector<uint32_t> indices;
	};
}

//struct VertexData
//{
//	Vector4 position;
//	Vector2 texcoord;
//	Vector3 normal;
//};

// \brief ModelLoader
// モデルファイルを読み込み、頂点データを生成するクラス。
// 主にラインモデル用のデータを読み込み、LoadLineModelで頂点配列を返す。
// デフォルトで"Resources/"ディレクトリから読み込む仕様。

class ModelLoader
{
public:

	// モデルデータを読み込む関数
    std::vector<Line::Vertex> LoadLineModel(std::string filePath);

	std::vector<VertexData> LoadModel(std::string fileName);

private:

	std::string directoryPath_ = "Resources/";

};

