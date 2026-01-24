#pragma once
#include "GpuContext.h"
#include "GpuParticleManager.h"

namespace Kouro
{
	// \brief エンジン全体で共有されるコンテキスト情報をまとめた構造体。

	struct EngineContext
	{
		GpuContext gpuContext;
		GpuParticleManager* gpuParticleManager = nullptr;
	};
}