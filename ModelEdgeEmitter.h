#pragma once
#include "BaseParticleEmitter.h"
#include "ModelLoader.h"

// TODO ; DirectXCommonのincludeを不要にする
#include "DirectXCommon.h"

class ModelEdgeEmitter : public BaseParticleEmitter
{
private:

	struct LineSegment
	{
		Vector3 start;
		float pad1 = 0.0f;
		Vector3 end;
		float pad2 = 0.0f;
	};

	struct Emitter
	{
		Vector3 translate;   // 
		float radius;        // 
		uint32_t count;      // 
		float frequency;     // 
		float frequencyTime; // 
		uint32_t emit;       // 
	};

public:

	/// \brief 初期化
	void Initialize(std::string groupName, EngineContext context) override;

	/// \brief 更新
	void Update() override;

	void Emit(Matrix4x4 mat);

	void CreateLineSegment(std::string filePath);

private:

	void CreateSegmentResource();

	void CreateEmitterResource();

private:

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> lineSegmentResource_ = nullptr; //!< 線の頂点ペアリスト用のリソース
	LineSegment* lineSegment_ = nullptr; //!< マップ用のポインタ
	uint32_t srvIndex_ = 0;

	std::vector<LineSegment> lineSegments_;

	uint32_t lineCount_ = 0; //!< 線の数

	Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource_ = nullptr;
	Emitter* emitter_ = nullptr;
};

