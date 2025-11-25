#pragma once
#include <string>
#include <iostream>
#include <vector>
#include<unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp\postprocess.h>

#include "MyMath.h"

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

class ModelLoader
{
public:

	// モデルデータを読み込む関数
    std::vector<Line::Vertex> LoadLineModel(std::string filePath);

private:

	std::string directoryPath_ = "Resources/";

};

