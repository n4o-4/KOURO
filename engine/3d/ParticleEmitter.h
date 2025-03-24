#pragma once
#include "ParticleManager.h"

class ParticleEmitter
{
private:

public:

    void Initialize(std::string name);

    void Emit(Vector3 startColor,Vector3 finishColor);
    
    // 指定した数のパーティクルを放出するオーバーロードを追加
    void Emit(uint32_t count,Vector3 startColor,Vector3 finishColor);
    
    // エミッタの位置を設定するメソッドを追加
    void SetPosition(const Vector3& position);
    
    // パーティクル数を設定するメソッド
    void SetParticleCount(uint32_t count);
    
    // 放出頻度を設定するメソッド
    void SetFrequency(float frequency);

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

    std::mt19937 randomEngine;
};