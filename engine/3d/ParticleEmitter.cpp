#include "ParticleEmitter.h"

void ParticleEmitter::Initialize(std::string name)
{
	/*----------------------------------------------------------
	* ランダムエンジンの初期化
	----------------------------------------------------------*/

	std::random_device seedGenerator;
	randomEngine.seed(seedGenerator());

	this->name = name;

	emitter.count = 3;
	emitter.frequency = 0.5f;
	emitter.frequencyTime = 0.0f;
	emitter.transform.scale = { 1.0f,1.0f,1.0f };
	emitter.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter.transform.translate = { 0.0f,0.0f,0.0f };
	emitter.lifeTime = 1.0f;

	startColorRange = { {0.56f,1.0f},{0.0f,0.37f},{0.0f,0.19f},{1.0f,1.0f} };

	finishColorRange = { {1.0f,1.0f},{1.0f,1.0f},{1.0f,1.0f},{1.0f,1.0f} };

	velocityRange = { {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f} };

	scaleRange = { {1.0f,1.0f},{1.0f,1.0f},{1.0f,1.0f} };

	lifeTimeRange = { 1.0f,1.0f };
}

void ParticleEmitter::Emit()
{

	ParticleManager::GetInstance()->Emit(name, emitter.transform.translate, emitter.count,startColorRange,finishColorRange,velocityRange,scaleRange,lifeTimeRange);

}

void ParticleEmitter::Emit(uint32_t count,Vector3 startColor,Vector3 finishColor)
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

void ParticleEmitter::HitEmit()
{
	ParticleManager::GetInstance()->HitEmit(name, emitter.transform.translate, emitter.count, startColorRange, finishColorRange, velocityRange, lifeTimeRange);
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

		Vector3 startColor;

		std::uniform_real_distribution<float> colorR(0.56f, 1.0f); // min , max
		std::uniform_real_distribution<float> colorG(0.0f, 0.37f);
		std::uniform_real_distribution<float> colorB(0.0f, 0.19f);
	
		if (particleGroups.find(name) != particleGroups.end())
		{
			ParticleManager::GetInstance()->HitEmit(name, emitter.transform.translate, emitter.count,startColorRange,finishColorRange,velocityRange,lifeTimeRange);
		}
	}
}