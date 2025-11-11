#pragma once

#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <wrl.h>
#include <d3d12.h>
#include<unordered_map>

#include "Vectors.h"
#include "MyMath.h"
#include "LineDrawerBase.h"

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

	void Initialize(LineDrawerBase* lienDrawer, const std::string& directoryPath, const std::string& filePath);

	void Draw();

	DirectXCommon* GetdxCommon() { return dxCommon_; }

private:

	void LoadLineModelFile(const std::string& directoryPath, const std::string& filePath);

private:

	LineDrawerBase* lineDrawerBase_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	LineVertex* vertexData_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	

	uint32_t vertexCount_ = 0;

};

