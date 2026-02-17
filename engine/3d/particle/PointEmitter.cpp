#include "PointEmitter.h"

void Kouro::PointEmitter::Initialize(std::string groupName, EngineContext context)
{
	groupName_ = groupName;

	context_ = context;

	// エミッターリソース作成
	CreateEmitterResource();

	// ワールド変換用リソース作成
	CreateTransformResource();
}

void Kouro::PointEmitter::Update()
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

void Kouro::PointEmitter::Emit(const Matrix4x4& mat)
{
	*transform_ = mat;

	// パーティクルマネージャー取得
	GpuParticleManager* manager = context_.gpuParticleManager;

	// グループが存在しなければ処理を抜ける
	if (manager->GetParticleGroups().find(groupName_) == manager->GetParticleGroups().end()) return;

	// グループ取得
	GpuParticleManager::ParticleGroup& group = manager->GetParticleGroups().find(groupName_)->second;

	// emit呼び出し
	manager->PointEmit(groupName_, emitterResource_.Get(), transformResource_.Get());
}
void Kouro::PointEmitter::SetEmitterProperties(Vector3 translate, uint32_t count, Vector3 velocityMin, Vector3 velocityMax, float lifeTimeMin, float lifeTimeMax, float frequency)
{
	if (emitter_ == nullptr) return;
	emitter_->translate = translate;
	emitter_->count = count;
	emitter_->velocityRange[0] = velocityMin;
	emitter_->velocityRange[1] = velocityMax;
	emitter_->lifeTimeRange[0] = lifeTimeMin;
	emitter_->lifeTimeRange[1] = lifeTimeMax;
	emitter_->frequency = frequency;
}
void Kouro::PointEmitter::CreateEmitterResource()
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

void Kouro::PointEmitter::CreateTransformResource()
{
	// ワールド変換用リソース作成
	transformResource_ = context_.gpuContext.gpuResourceUtils->CreateBufferResource(sizeof(Matrix4x4));
	transform_ = nullptr;
	transformResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transform_));
}