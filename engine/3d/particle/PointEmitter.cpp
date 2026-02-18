#include "PointEmitter.h"

void Kouro::PointEmitter::Initialize(std::string groupName, EngineContext context)
{
	groupName_ = groupName;

	context_ = context;

	// エミッターリソース作成
	CreateEmitterResource();

	// ワールド変換用リソース作成
	CreateTransformResource();
	
	wTransform_ = std::make_unique<WorldTransform>();
	wTransform_->Initialize();

	wTransform_->SetTranslate({ 0.0f, 0.0f, -6.0f });
}

void Kouro::PointEmitter::Update()
{
	if (emitter_->frequency == 0.0f)
	{
		emitter_->emit = 1;

		return;
	}

	// エミット判定
	if (emitter_->frequency <= emitter_->frequencyTime)
	{
		emitter_->frequencyTime -= emitter_->frequency;

		emitter_->emit = 1; // true
	}
	else
	{
		emitter_->emit = 0; // false

		emitter_->frequencyTime += 1.0f / 60.0f;
	}
}

void Kouro::PointEmitter::Emit(const Matrix4x4& mat)
{
	static Vector3 prevPosition;

	// ワールド変換の更新と転送
	wTransform_->UpdateMatrix();

	*transform_ = wTransform_->GetWorldMatrix();

	

	// ワールド変換の位置をエミッターデータに設定
	emitter_->translate = wTransform_->GetWorldPosition();

	// パーティクルマネージャー取得
	GpuParticleManager* manager = context_.gpuParticleManager;

	// グループが存在しなければ処理を抜ける
	if (manager->GetParticleGroups().find(groupName_) == manager->GetParticleGroups().end()) return;

	// emit呼び出し
	manager->PointEmit(groupName_, emitterResource_.Get(), transformResource_.Get());

	static int emitCount = 0;

	// デバッグ出力（Emit時のみ）
	if (1)
	{
		auto now = std::chrono::system_clock::now();
		auto timeT = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		Kouro::Vector3 currentPosition = wTransform_->GetWorldPosition();

		std::tm localTime;
		localtime_s(&localTime, &timeT);
		std::ostringstream oss;
		oss << std::put_time(&localTime, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();
		OutputDebugStringA(("[" + oss.str() + "] Emit Particle " + std::to_string(emitCount++) + " " + std::to_string(currentPosition.x) + " " + std::to_string(currentPosition.y) + " " + std::to_string(currentPosition.z) + "\n").c_str());
	}

	prevPosition = wTransform_->GetWorldPosition();
}
void Kouro::PointEmitter::SetEmitterProperties(Vector3 translate, uint32_t count, Vector3 velocityMin, Vector3 velocityMax, float lifeTimeMin, float lifeTimeMax, float frequency)
{
	if (emitter_ == nullptr) return;
	emitter_->translate = translate;
	emitter_->count = count;
	emitter_->velocityMin = velocityMin;
	emitter_->velocityMax = velocityMax;
	emitter_->lifeTimeMin = lifeTimeMin;
	emitter_->lifeTimeMax = lifeTimeMax;
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