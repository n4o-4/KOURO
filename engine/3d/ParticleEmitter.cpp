#include "ParticleEmitter.h"

void ParticleEmitter::Initialize(std::string name)
{
	this->name = name;

	emitter.count = 3;
	emitter.frequency = 0.5f;
	emitter.frequencyTime = 0.0f;
	emitter.transform.scale = { 1.0f,1.0f,1.0f };
	emitter.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter.transform.translate = { 0.0f,0.0f,0.0f };

}

void ParticleEmitter::Emit()
{

	ParticleManager::GetInstance()->Emit(name, emitter.transform.translate, emitter.count);

}

void ParticleEmitter::Emit(uint32_t count)
{
    // 一時的にカウントを保存
    uint32_t originalCount = emitter.count;
    // 指定されたカウント数に設定
    emitter.count = count;
    // 通常のEmitを呼び出し
    Emit();
    // 元のカウント数に戻す
    emitter.count = originalCount;
}

void ParticleEmitter::SetPosition(const Vector3& position)
{
    emitter.transform.translate = position;
}

void ParticleEmitter::SetParticleCount(uint32_t count)
{
    emitter.count = count;
}

void ParticleEmitter::SetFrequency(float frequency)
{
    emitter.frequency = frequency;
}

void ParticleEmitter::Update()
{

	particleGroups = ParticleManager::GetInstance()->GetParticleGroups();


	emitter.frequencyTime += kDeltaTime;

	if (emitter.frequency <= emitter.frequencyTime) {
		
		emitter.frequencyTime -= emitter.frequency;

		if (particleGroups.find(name) != particleGroups.end())
		{
			ParticleManager::GetInstance()->Emit(name, emitter.transform.translate, emitter.count);
		}

		/*for (std::unordered_map<std::string, ParticleManager::ParticleGroup>::iterator particleGroupIterator = particleGroups.begin(); particleGroupIterator != particleGroups.end();) {

			ParticleManager::ParticleGroup* particleGroup = &(particleGroupIterator->second);

			ParticleManager::GetInstance()->Emit(name, emitter.transform.translate, emitter.count);

			++particleGroupIterator;
		}*/
	}
}