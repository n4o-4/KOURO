#pragma once
#define NOMINMAX
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "ModelDatas.h"


#include <set>

class LineDrawerBase
{
public:

	enum class Type
	{
		AABB,
		Sphere,
		Grid,
		Skeleton,
		CatmullRom,
		Object3D
	};

private:

	struct LineModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
		std::vector<uint32_t> indices;
	};

	struct Triangle {
		int indices[3];   // 頂点のインデックス
		Vector3 normal;   // 三角形の法線
	};

private:

	struct Sphere {
		Vector3 center;
		float radius;
		unsigned int color;
	};

	static const int kMaxLines = 2048;

	struct Pipeline
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	};

	struct LineVertex
	{
		Vector4 position;
	};

	struct LineForGPU
	{
		Matrix4x4 matWorld;
		Vector4 color;
	};

	struct LineObject
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
		LineVertex* vertexData = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		
		Microsoft::WRL::ComPtr<ID3D12Resource> lineResource = nullptr;
		LineForGPU* lineData = nullptr;

		WorldTransform* transform;

		Type type;

		int32_t vertexIndex = 0;

		Skeleton skeleton;
	};

	
public: // メンバ関数

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

	// 描画
	void PreDraw(ViewProjection viewProjection);

	DirectXCommon* GetdxCommon() { return dxCommon_; }

private: // メンバ関数

	void CreateRootSignature();

	void CreatePipellineState();

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	std::unique_ptr<Pipeline> pipeline_ = nullptr;

	float scanTime = 6.0f;

	bool isScanActive = true;
};

