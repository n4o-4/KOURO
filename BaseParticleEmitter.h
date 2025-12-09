#pragma once
#include "GpuParticleManager.h"

#include "EngineContext.h"

class BaseParticleEmitter
{
public:

	virtual ~BaseParticleEmitter() = default;

	virtual void Initialize(std::string groupName, EngineContext context) = 0;

	virtual void Update() = 0;

protected:

	std::string groupName_ = {};

	EngineContext context_ = {};
};

