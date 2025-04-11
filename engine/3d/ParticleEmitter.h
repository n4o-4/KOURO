#pragma once
#include "ParticleManager.h"

class ParticleEmitter
{
private:

public:

    void Initialize(std::string name);

    void Emit();
    
    // 指定した数のパーティクルを放出するオーバーロードを追加
    void Emit(uint32_t count,Vector3 startColor,Vector3 finishColor);
    
	void HitEmit();

    // エミッタの位置を設定するメソッドを追加
    void SetPosition(const Vector3& position);
    
    // パーティクル数を設定するメソッド
    void SetParticleCount(uint32_t count);
    
    // 放出頻度を設定するメソッド
    void SetFrequency(float frequency);

    void Update();

    void SetStartColorRange(ParticleManager::ColorRange colorRange) { startColorRange = colorRange; }
    void SetFinishColorRange(ParticleManager::ColorRange colorRange) { finishColorRange = colorRange; }

	void SetVelocityRange(ParticleManager::VelocityRange velocityRange) { this->velocityRange = velocityRange; }
	void SetLifeTimeRange(ParticleManager::LifeTimeRange lifeTimeRange) { this->lifeTimeRange = lifeTimeRange; }

private:

    struct Emitter {
        ParticleManager::Transform transform; // エミッタのTransform
        uint32_t count; // 発生数
        float frequency; // 発生頻度
        float frequencyTime; // 頻度用時刻
		float lifeTime; // 寿命
		Vector4 startColor; // 開始色
		Vector4 finishColor; // 終了色
    };

    Emitter emitter{};

    std::string name;

    std::unordered_map<std::string, ParticleManager::ParticleGroup> particleGroups;

    std::mt19937 randomEngine;

	// パーティクルの色の範囲を保持するメンバ変数を追加
	ParticleManager::ColorRange startColorRange;
	ParticleManager::ColorRange finishColorRange;   

    ParticleManager::VelocityRange velocityRange;
	ParticleManager::LifeTimeRange lifeTimeRange;
};