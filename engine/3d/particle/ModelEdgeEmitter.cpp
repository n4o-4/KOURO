#include "ModelEdgeEmitter.h"
#include "PointEmitter.h"

namespace Kouro
{
	void ModelEdgeEmitter::Initialize(std::string groupName, EngineContext context)
	{
		groupName_ = groupName;

		context_ = context;

		// エミッターリソース作成
		CreateEmitterResource();
	}

	void ModelEdgeEmitter::Update()
	{
		// エミット判定
		if (emitter_->frequency <= emitter_->frequencyTime)
		{
			emitter_->frequencyTime -= emitter_->frequency;

			emitter_->emit = 1; // true
		}
		else
		{
			emitter_->emit = 0; // false
		}
	}

	void ModelEdgeEmitter::Emit(Matrix4x4 mat)
	{
		// パーティクルマネージャー取得
		GpuParticleManager* manager = context_.gpuParticleManager;

		// グループが存在しなければ処理を抜ける
		if (manager->GetParticleGroups().find(groupName_) == manager->GetParticleGroups().end()) return;

		// emit呼び出し
		manager->LineEmit(groupName_, srvIndex_, lineCount_, emitterResource_, mat);
	}

	void ModelEdgeEmitter::CreateLineSegment(std::string filePath)
	{
		ModelLoader loader;

		// ラインモデル読み込み
		std::vector<Line::Vertex> vertices = loader.LoadLineModel(filePath);

		// 
		for (size_t i = 0; i + 1 < vertices.size(); i += 2)
		{
			LineSegment seg;
			seg.start = { vertices[i].position.x, vertices[i].position.y, vertices[i].position.z };
			seg.end = { vertices[i + 1].position.x, vertices[i + 1].position.y, vertices[i + 1].position.z };
			seg.pad1 = 0.0f;
			seg.pad2 = 0.0f;

			lineSegments_.push_back(seg);
		}

		lineCount_ = lineSegments_.size();

		// ラインセグメントリソース作成
		CreateSegmentResource();
	}

	void ModelEdgeEmitter::CreateSegmentResource()
	{
		// ラインセグメント用リソース作成
		lineSegmentResource_ = context_.gpuContext.gpuResourceUtils->CreateBufferResource(sizeof(LineSegment) * lineSegments_.size());
		LineSegment* lineSegmentData;
		lineSegmentResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&lineSegmentData));

		// データ転送
		std::memcpy(lineSegmentData, lineSegments_.data(), sizeof(LineSegment) * lineSegments_.size());

		srvIndex_ = context_.gpuContext.srvManager->Allocate();

		context_.gpuContext.srvManager->CreateSRVforStructuredBuffer(srvIndex_, lineSegmentResource_.Get(), static_cast<UINT>(lineSegments_.size()), sizeof(LineSegment));
	}

	void ModelEdgeEmitter::CreateEmitterResource()
	{
		// エミッター用リソース作成
		emitterResource_ = context_.gpuContext.gpuResourceUtils->CreateBufferResource(sizeof(Emitter));
		emitter_ = nullptr;
		emitterResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));

		// エミッターデータ設定
		emitter_->emit = 1; // true
		emitter_->frequency = 0.0f;
		emitter_->frequencyTime = 0.0f;
		emitter_->count = 10;
	}
}