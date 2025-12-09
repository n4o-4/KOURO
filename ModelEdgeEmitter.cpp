#include "ModelEdgeEmitter.h"

void ModelEdgeEmitter::Initialize(std::string groupName, EngineContext context)
{
	groupName_ = groupName;

	CreateEmitterResource();
}

void ModelEdgeEmitter::Update()
{
	if (emitter_->frequency <= emitter_->frequencyTime)
	{
		emitter_->frequencyTime -= emitter_->frequency;
		emitter_->emit = 1;
	}
	else
	{
		emitter_->emit = 0;
	}
}

void ModelEdgeEmitter::Emit(Matrix4x4 mat)
{
	GpuParticleManager* manager = context_.gpuParticleManager;

	if (manager->GetParticleGroups().find(groupName_) == manager->GetParticleGroups().end()) return;

	GpuParticleManager::ParticleGroup  group = manager->GetParticleGroups().find(groupName_)->second;

	manager->LineEmit(groupName_, srvIndex_, lineCount_, emitterResource_, mat);
}

void ModelEdgeEmitter::CreateLineSegment(std::string filePath)
{
	ModelLoader loader;

	std::vector<Line::Vertex> vertices = loader.LoadLineModel(filePath);

	// ���_������ł���O��iLINELIST�j
	// 2���Ƃɐ�����쐬
	for (size_t i = 0; i + 1 < vertices.size(); i += 2)
	{
		LineSegment seg;
		seg.start = { vertices[i].position.x, vertices[i].position.y, vertices[i].position.z };
		seg.end = { vertices[i + 1].position.x, vertices[i + 1].position.y, vertices[i + 1].position.z };
		seg.pad1 = 0.0f;
		seg.pad2 = 0.0f;

		lineSegments_.push_back(seg);
	}

	// lineSegment
	CreateSegmentResource();
}

void ModelEdgeEmitter::CreateSegmentResource()
{
	lineSegmentResource_ = dxCommon_->CreateBufferResource(sizeof(LineSegment) * lineSegments_.size());
    LineSegment* lineSegmentData;
	lineSegmentResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&lineSegmentData));
	std::memcpy(lineSegmentData, lineSegments_.data(), sizeof(LineSegment) * lineSegments_.size());

	srvIndex_ = context_.gpuContext.srvManager->Allocate();

	context_.gpuContext.srvManager->CreateSRVforStructuredBuffer(srvIndex_, lineSegmentResource_.Get(), static_cast<UINT>(lineSegments_.size()), sizeof(LineSegment));
}

void ModelEdgeEmitter::CreateEmitterResource()
{
	emitterResource_ = dxCommon_->CreateBufferResource(sizeof(Emitter));
	emitter_ = nullptr;
	emitterResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));
	emitter_->emit = 0;
	emitter_->frequency = 0.0f;
	emitter_->frequencyTime = 0.0f;
	emitter_->count = 512;
}