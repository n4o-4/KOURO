#pragma once
#include "ParticleManager.h"

// \brief ParticleEmitter
// パーティクル放出を管理するクラス。
// 発生位置や頻度、色、スケール、回転、速度、寿命などを設定可能。
// 単発放出(Emit)、放射状放出(RadialEmit)、指定数放出(Emit(count, ...))などの機能を提供。
// 内部でEmitter構造体に状態を保持し、ParticleManagerのParticleGroupを管理。

class ParticleEmitter
{
private:

public:
    
	/**
	* \brief  初期化
	* \param  name エミッタの名前
    */
    void Initialize(std::string name);

	/// \brief パーティクルの単発放出
    void Emit();

	/// \brief 放射状にパーティクルを放出
	void RadialEmit();
    
    /**
	* \brief 指定数のパーティクルを放出
	* \param count 放出するパーティクル数
	* \param startColor 開始色
	* \param finishColor 終了色
    */
    void Emit(uint32_t count,Vector3 startColor,Vector3 finishColor);

    /**
	* \brief  位置設定
	* \param  position 位置
    */
    void SetPosition(const Vector3& position);
    
    /**
	* \brief  発生数設定
	* \param  count 発生数
    */
    void SetParticleCount(uint32_t count);
    
    /**
	* \brief  発生頻度設定
	* \param  frequency 発生頻度
    */
    void SetFrequency(float frequency);

	/// \brief パーティクルの更新
    void Update();

    /**
	* \brief  開始色範囲設定
	* \param  colorRange 色範囲
    */
    void SetStartColorRange(ParticleManager::ColorRange colorRange) { particleStates.startColorRange = colorRange; }

    /**
	* \brief  終了色範囲設定
	* \param  colorRange 色範囲
    */
    void SetFinishColorRange(ParticleManager::ColorRange colorRange) { particleStates.finishColorRange = colorRange; }

    /**
	* \brief  開始スケール範囲設定
	* \param  scaleRange スケール範囲
    */
	void SetStartScaleRange(ParticleManager::Vec3Range scaleRange) { particleStates.startScaleRange = scaleRange; }

    /**
	* \brief  終了スケール範囲設定
	* \param  scaleRange スケール範囲
    */
	void SetFinishScaleRange(ParticleManager::Vec3Range scaleRange) { particleStates.finishScaleRange = scaleRange; }

    /**
	* \brief  回転範囲設定
	* \param  rotateRange 回転範囲
    */
	void SetRotateRange(ParticleManager::Vec3Range rotateRange) { particleStates.rotateRange = rotateRange; }

    /**
	* \brief  発生位置範囲設定
	* \param  translateRange 発生位置範囲
    */
	void SetTranslateRange(ParticleManager::Vec3Range translateRange) { particleStates.translateRange = translateRange; }

    /**
	* \brief  速度範囲設定
	* \param  velocityRange 速度範囲
    */
    void SetVelocityRange(ParticleManager::Vec3Range velocityRange) { particleStates.velocityRange = velocityRange; }

    /**
	* \brief  寿命範囲設定
	* \param  lifeTimeRange 寿命範囲
    */
    void SetLifeTimeRange(ParticleManager::Range lifeTimeRange) { particleStates.lifeTimeRange = lifeTimeRange; }

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

	ParticleManager::ParticleStates particleStates;

};