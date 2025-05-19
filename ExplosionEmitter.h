#pragma once
#include "ParticleManager.h"

class ExplosionEmitter
{
private:

public:

    void Initialize(std::string name);

    void Emit();

    // エミッタの位置を設定するメソッドを追加
    void SetPosition(const Vector3& position);

    // パーティクル数を設定するメソッド
    void SetParticleCount(uint32_t count);

    // 放出頻度を設定するメソッド
    void SetFrequency(float frequency);

    void Update();

    void SetStartColorRange(ParticleManager::ColorRange colorRange) { startColorRange = colorRange; }
    void SetFinishColorRange(ParticleManager::ColorRange colorRange) { finishColorRange = colorRange; }

    void SetVelocityRange(ParticleManager::Vec3Range velocityRange) { this->velocityRange = velocityRange; }
    void SetLifeTimeRange(ParticleManager::Range lifeTimeRange) { this->lifeTimeRange = lifeTimeRange; }

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

    std::mt19937 randomEngine;

	ParticleManager::ColorRange startColorRange;
	ParticleManager::ColorRange finishColorRange;

    ParticleManager::Vec3Range velocityRange;
    ParticleManager::Range lifeTimeRange;
};