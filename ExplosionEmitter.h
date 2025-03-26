#pragma once
#include "ParticleManager.h"

class ExplosionEmitter
{
private:

public:

    void Initialize(std::string name);

    void Emit(Vector3 startColor, Vector3 finishColor);

    // �w�肵�����̃p�[�e�B�N������o����I�[�o�[���[�h��ǉ�
    void Emit(uint32_t count, Vector3 startColor, Vector3 finishColor);

    // �G�~�b�^�̈ʒu��ݒ肷�郁�\�b�h��ǉ�
    void SetPosition(const Vector3& position);

    // �p�[�e�B�N������ݒ肷�郁�\�b�h
    void SetParticleCount(uint32_t count);

    // ���o�p�x��ݒ肷�郁�\�b�h
    void SetFrequency(float frequency);

    void Update();

private:

    struct Emitter {
        ParticleManager::Transform transform; // �G�~�b�^��Transform
        uint32_t count; // ������
        float frequency; // �����p�x
        float frequencyTime; // �p�x�p����
    };

    Emitter emitter{};

    std::string name;

    std::unordered_map<std::string, ParticleManager::ParticleGroup> particleGroups;

    std::mt19937 randomEngine;
};