#include "RandomGenerate.h"

namespace Kouro
{
	void RandomGenerate::Initialize()
	{
		std::random_device seedGenerator;
		randomEngine.seed(seedGenerator());
	}

	float RandomGenerate::Random(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);

		return distribution(randomEngine);
	}

	Vector3 RandomGenerate::RandomVector3(float min, float max)
	{
		return Vector3(Random(min, max), Random(min, max), Random(min, max));
	}
}