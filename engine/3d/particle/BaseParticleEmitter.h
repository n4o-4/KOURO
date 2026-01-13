#pragma once
#include "GpuParticleManager.h"

#include "EngineContext.h"

/**
* \brief ComputeShader用
*/
namespace CS
{
	// ComputeShaderで1パッチあたり処理できる最大スレッド数（=512スレッド）
	constexpr uint32_t kMaxThreadsPerGroup = 512;
}

/**
* \class Gpuパーティクルエミッターの基底クラス
*/

class BaseParticleEmitter
{
public:

	/// \brief デストラクタ
	virtual ~BaseParticleEmitter() = default;

	/**
	* \brief  初期化
	* \param  groupName : パーティクルグループの名前
	* \param  context   : エンジンコンテキスト
	*/
	virtual void Initialize(std::string groupName, EngineContext context) = 0;

	/// \brief 更新
	virtual void Update() = 0;

protected:

	std::string groupName_ = {};

	EngineContext context_ = {};
};

