#pragma once
#include "GpuContext.h"
#include "GpuParticleManager.h"

struct EngineContext
{
	GpuContext gpuContext;
	GpuParticleManager* gpuParticleManager;
};