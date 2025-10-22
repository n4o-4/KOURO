#pragma once

#include "System.h"

#include "Input.h"

class InputSystem : public System
{
public:

	void Update(ECSCoordinator& ecs) override;
	
};

