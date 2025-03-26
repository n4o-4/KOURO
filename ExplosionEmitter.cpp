#include "ExplosionEmitter.h"

void ExplosionEmitter::Initialize(std::string name)
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

}

void ExplosionEmitter::Emit()
{
	Vector3 startColor;

	std::uniform_real_distribution<float> colorR(0.56f, 1.0f); // min , max
	std::uniform_real_distribution<float> colorG(0.0f, 0.37f);
	std::uniform_real_distribution<float> colorB(0.0f, 0.19f);

	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	Vector3 velocity = { dist(randomEngine),dist(randomEngine),dist(randomEngine) };

	velocity = Normalize(velocity);

	ParticleManager::GetInstance()->ExplosionEmit(name, emitter.transform.translate, emitter.count, Vector3(colorR(randomEngine), colorG(randomEngine), colorB(randomEngine)), Vector3(1.0f, 1.0f, 1.0f));
}

void ExplosionEmitter::SetPosition(const Vector3& position)
{
	emitter.transform.translate = position;
}

void ExplosionEmitter::SetParticleCount(uint32_t count)
{
	emitter.count = count;
}

void ExplosionEmitter::SetFrequency(float frequency)
{
	emitter.frequency = frequency;
}

void ExplosionEmitter::Update()
{

	particleGroups = ParticleManager::GetInstance()->GetParticleGroups();


	emitter.frequencyTime += kDeltaTime;

	if (emitter.frequency <= emitter.frequencyTime) {

		emitter.frequencyTime -= emitter.frequency;

		Vector3 startColor;

		std::uniform_real_distribution<float> colorR(0.56f, 1.0f); // min , max
		std::uniform_real_distribution<float> colorG(0.0f, 0.37f);
		std::uniform_real_distribution<float> colorB(0.0f, 0.19f);

		std::uniform_real_distribution<float> dist(-2.0f, 2.0f);

		Vector3 velocity = { dist(randomEngine),dist(randomEngine),dist(randomEngine) };

		if (particleGroups.find(name) != particleGroups.end())
		{
			ParticleManager::GetInstance()->ExplosionEmit(name, emitter.transform.translate, emitter.count, Vector3(colorR(randomEngine), colorG(randomEngine), colorB(randomEngine)), Vector3(1.0f, 1.0f, 1.0f));
		}
	}
}