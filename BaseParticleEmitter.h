#pragma once
#include "GpuParticleManager.h"

class BaseParticleEmitter
{
public:

	virtual ~BaseParticleEmitter() = default;

	virtual void Initialize();

	virtual void Update();

};

