#pragma once
#include "ParticleManager.h"

class ParticleEmitter
{
private:

public:

	void Initialize(std::string name);

	void Emit();

	void Update();

private:

	struct Emitter {
		ParticleManager::Transform transform; // エミッタのTransform
		uint32_t count; // 発生数
		float frequency; // 発生頻度
		float frequencyTime; // 頻度用時刻
	};

	Emitter emitter{};

	std::string name;

	std::unordered_map<std::string, ParticleManager::ParticleGroup> particleGroups;

};